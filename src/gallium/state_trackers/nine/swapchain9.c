/*
 * Copyright 2011 Joakim Sindholt <opensource@zhasha.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "swapchain9.h"
#include "surface9.h"
#include "device9.h"

#include "nine_helpers.h"
#include "nine_pipe.h"
#include "nine_dump.h"

#include "util/u_inlines.h"
#include "util/u_surface.h"

#define DBG_CHANNEL DBG_SWAPCHAIN

HRESULT
NineSwapChain9_ctor( struct NineSwapChain9 *This,
                     struct NineUnknownParams *pParams,
                     struct NineDevice9 *pDevice,
                     BOOL implicit,
                     ID3DPresent *pPresent,
                     PPRESENT_TO_RESOURCE pPTR,
                     HWND hFocusWindow )
{
    D3DPRESENT_PARAMETERS params;
    HRESULT hr;

    DBG("This=%p pDevice=%p pPresent=%p pPTR=%p hFocusWindow=%p\n",
        This, pDevice, pPresent, pPTR, hFocusWindow);

    hr = NineUnknown_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    This->screen = NineDevice9_GetScreen(pDevice);
    This->pipe = NineDevice9_GetPipe(pDevice);
    This->cso = NineDevice9_GetCSO(pDevice);
    This->device = pDevice;
    This->implicit = implicit;
    if (!implicit)
        NineUnknown_AddRef(NineUnknown(This->device));
    This->ptrfunc = pPTR;
    This->present = pPresent;
    ID3DPresent_AddRef(pPresent);
    hr = ID3DPresent_GetPresentParameters(This->present, &params);
    if (FAILED(hr))
        return hr;
    if (!params.hDeviceWindow)
        params.hDeviceWindow = hFocusWindow;

    return NineSwapChain9_Resize(This, &params);
}

HRESULT
NineSwapChain9_Resize( struct NineSwapChain9 *This,
                       D3DPRESENT_PARAMETERS *pParams )
{
    struct NineDevice9 *pDevice = This->device;
    struct NineSurface9 **bufs;
    D3DSURFACE_DESC desc;
    HRESULT hr;
    struct pipe_resource *resource, tmplt;
    unsigned i;

    DBG("This=%p pParams=%p\n", This, pParams);
    if (pParams) {
        DBG("pParams(%p):\n"
            "BackBufferWidth: %u\n"
            "BackBufferHeight: %u\n"
            "BackBufferFormat: %s\n"
            "BackBufferCount: %u\n"
            "MultiSampleType: %u\n"
            "MultiSampleQuality: %u\n"
            "SwapEffect: %u\n"
            "hDeviceWindow: %p\n"
            "Windowed: %i\n"
            "EnableAutoDepthStencil: %i\n"
            "AutoDepthStencilFormat: %s\n"
            "Flags: %s\n"
            "FullScreen_RefreshRateInHz: %u\n"
            "PresentationInterval: %x\n", pParams,
            pParams->BackBufferWidth, pParams->BackBufferHeight,
            d3dformat_to_string(pParams->BackBufferFormat),
            pParams->BackBufferCount,
            pParams->MultiSampleType, pParams->MultiSampleQuality,
            pParams->SwapEffect, pParams->hDeviceWindow, pParams->Windowed,
            pParams->EnableAutoDepthStencil,
            d3dformat_to_string(pParams->AutoDepthStencilFormat),
            nine_D3DPRESENTFLAG_to_str(pParams->Flags),
            pParams->FullScreen_RefreshRateInHz,
            pParams->PresentationInterval);
    }

    if (pParams->BackBufferFormat == D3DFMT_UNKNOWN)
        pParams->BackBufferFormat = This->params.BackBufferFormat;
    if (pParams->EnableAutoDepthStencil &&
        This->params.EnableAutoDepthStencil &&
        pParams->AutoDepthStencilFormat == D3DFMT_UNKNOWN)
        pParams->AutoDepthStencilFormat = This->params.AutoDepthStencilFormat;
    /* NULL means focus window.
    if (!pParams->hDeviceWindow && This->params.hDeviceWindow)
        pParams->hDeviceWindow = This->params.hDeviceWindow;
    */
    if (pParams->BackBufferCount == 0)
        pParams->BackBufferCount = 1; /* ref MSDN */

    if (pParams->BackBufferWidth == 0 || pParams->BackBufferHeight == 0) {
        if (!pParams->Windowed)
            return D3DERR_INVALIDCALL;

        DBG("FIXME: set buffer dimensions to window's client rect.\n");
        if (!pParams->BackBufferWidth)
            pParams->BackBufferWidth = This->params.BackBufferWidth;
        if (!pParams->BackBufferHeight)
            pParams->BackBufferHeight = This->params.BackBufferHeight;
    }

    tmplt.target = PIPE_TEXTURE_2D;
    tmplt.width0 = pParams->BackBufferWidth;
    tmplt.height0 = pParams->BackBufferHeight;
    tmplt.depth0 = 1;
    tmplt.nr_samples = pParams->MultiSampleType;
    tmplt.last_level = 0;
    tmplt.array_size = 1;
    tmplt.usage = PIPE_USAGE_DEFAULT;
    tmplt.bind = PIPE_BIND_TRANSFER_READ | PIPE_BIND_TRANSFER_WRITE;
    tmplt.flags = 0;

    desc.Type = D3DRTYPE_SURFACE;
    desc.Pool = D3DPOOL_DEFAULT;
    desc.MultiSampleType = pParams->MultiSampleType;
    desc.MultiSampleQuality = 0;
    desc.Width = pParams->BackBufferWidth;
    desc.Height = pParams->BackBufferHeight;

    if (pParams->BackBufferCount != This->params.BackBufferCount) {
        for (i = pParams->BackBufferCount; i < This->params.BackBufferCount;
             ++i)
            nine_reference(&This->buffers[i], NULL);

        bufs = REALLOC(This->buffers,
                       This->params.BackBufferCount * sizeof(This->buffers[0]),
                       pParams->BackBufferCount * sizeof(This->buffers[0]));
        if (!bufs)
            return E_OUTOFMEMORY;
        This->buffers = bufs;
        for (i = This->params.BackBufferCount; i < pParams->BackBufferCount;
             ++i)
            This->buffers[i] = NULL;
    }

    for (i = 0; i < pParams->BackBufferCount; ++i) {
        tmplt.format = d3d9_to_pipe_format(pParams->BackBufferFormat);
        tmplt.bind |= PIPE_BIND_RENDER_TARGET;
        resource = This->screen->resource_create(This->screen, &tmplt);
        if (!resource) {
            DBG("Failed to create pipe_resource.\n");
            return D3DERR_OUTOFVIDEOMEMORY;
        }
        if (pParams->Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER)
            resource->flags |= NINE_RESOURCE_FLAG_LOCKABLE;
        if (This->buffers[i]) {
            NineSurface9_SetResourceResize(This->buffers[i], resource);
            pipe_resource_reference(&resource, NULL);
        } else {
            desc.Format = pParams->BackBufferFormat;
            desc.Usage = D3DUSAGE_RENDERTARGET;
            hr = NineSurface9_new(pDevice, NineUnknown(This), resource, 0,
                                  0, 0, &desc, &This->buffers[i]);
            pipe_resource_reference(&resource, NULL);
            if (FAILED(hr)) {
                DBG("Failed to create RT surface.\n");
                return hr;
            }
        }
    }
    if (pParams->EnableAutoDepthStencil) {
        tmplt.format = d3d9_to_pipe_format(pParams->AutoDepthStencilFormat);
        tmplt.bind &= ~PIPE_BIND_RENDER_TARGET;
        tmplt.bind |= PIPE_BIND_DEPTH_STENCIL;

        resource = This->screen->resource_create(This->screen, &tmplt);
        if (!resource) {
            DBG("Failed to create pipe_resource for depth buffer.\n");
            return D3DERR_OUTOFVIDEOMEMORY;
        }
        if (This->zsbuf) {
            NineSurface9_SetResourceResize(This->zsbuf, resource);
            pipe_resource_reference(&resource, NULL);
        } else {
            /* XXX wine thinks the container of this should the the device */
            desc.Format = pParams->AutoDepthStencilFormat;
            desc.Usage = D3DUSAGE_DEPTHSTENCIL;
            hr = NineSurface9_new(pDevice, NineUnknown(This), resource, 0,
                                  0, 0, &desc, &This->zsbuf);
            pipe_resource_reference(&resource, NULL);
            if (FAILED(hr)) {
                DBG("Failed to create ZS surface.\n");
                return hr;
            }
        }
    }

    This->params = *pParams;

    return D3D_OK;
}

void
NineSwapChain9_dtor( struct NineSwapChain9 *This )
{
    unsigned i;

    DBG("This=%p\n", This);

    if (This->buffers) {
        for (i = 0; i < This->params.BackBufferCount; i++)
            nine_reference(&This->buffers[i], NULL);
        FREE(This->buffers);
    }
    nine_reference(&This->zsbuf, NULL);

    if (This->present) {
        ID3DPresent_Release(This->present);
    }

    if (!This->implicit)
        NineUnknown_Release(NineUnknown(This->device));

    NineUnknown_dtor(&This->base);
}

static INLINE HRESULT
present( struct NineSwapChain9 *This,
         const RECT *pSourceRect,
         const RECT *pDestRect,
         HWND hDestWindowOverride,
         const RGNDATA *pDirtyRegion,
         DWORD dwFlags )
{
    struct pipe_resource *resource;
    HRESULT hr;
    RGNDATA *rgndata;
    RECT rect;

    /* get a real backbuffer handle from the windowing system */
    hr = This->ptrfunc(This->present, hDestWindowOverride, &rect,
                       &rgndata, This->screen, &resource);
    if (FAILED(hr)) {
        return hr;
    } else if (hr == D3DOK_WINDOW_OCCLUDED) {
        /* if we present, nothing will show, so don't present */
        return D3D_OK;
    }

    DBG(">>>\npresent: This=%p pSourceRect=%p pDestRect=%p "
        "pDirtyRegion=%p rgndata=%p\n",
        This, pSourceRect, pDestRect, pDirtyRegion, rgndata);
    if (pSourceRect)
        DBG("pSourceRect = (%u..%u)x(%u..%u)\n",
            pSourceRect->left, pSourceRect->right,
            pSourceRect->top, pSourceRect->bottom);
    if (pDestRect)
        DBG("pDestRect = (%u..%u)x(%u..%u)\n",
            pDestRect->left, pDestRect->right,
            pDestRect->top, pDestRect->bottom);

    if (rgndata) {
        /* TODO */
    } else {
        struct pipe_blit_info blit;

        blit.dst.resource = resource;
        blit.dst.level = 0;
        blit.dst.format = resource->format;
        blit.dst.box.z = 0;
        blit.dst.box.depth = 1;
        if (pDestRect) {
            rect_to_pipe_box_xy_only(&blit.dst.box, pDestRect);
            blit.dst.box.x += rect.left;
            blit.dst.box.y += rect.top;
            if (u_box_clip_2d(&blit.dst.box, &blit.dst.box,
                              rect.right - rect.left,
                              rect.bottom - rect.top) < 0) {
                DBG("Dest region clipped.\n");
                return D3D_OK;
            }
        } else {
            rect_to_pipe_box_xy_only(&blit.dst.box, &rect);
        }

        assert(This->buffers[0]->surface);
        blit.src.resource = This->buffers[0]->surface->texture;
        blit.src.level = This->buffers[0]->level;
        blit.src.format = blit.src.resource->format;
        blit.src.box.z = 0;
        blit.src.box.depth = 1;
        if (pSourceRect) {
            rect_to_pipe_box_xy_only(&blit.src.box, pSourceRect);
            u_box_clip_2d(&blit.src.box, &blit.src.box,
                          This->buffers[0]->surface->width,
                          This->buffers[0]->surface->height);
        } else {
            blit.src.box.x = 0;
            blit.src.box.y = 0;
            blit.src.box.width = This->buffers[0]->surface->width;
            blit.src.box.height = This->buffers[0]->surface->height;
        }

        blit.mask = PIPE_MASK_RGBA;
        blit.filter = PIPE_TEX_FILTER_NEAREST;
        blit.scissor_enable = FALSE;

        /* blit (and possibly stretch/convert) pixels from This->buffers[0] to
         * emusurf using u_blit. Windows appears to use NEAREST */
        DBG("Blitting (%u..%u)x(%u..%u) to (%u..%u)x(%u..%u).\n",
            blit.src.box.x, blit.src.box.x + blit.src.box.width,
            blit.src.box.y, blit.src.box.y + blit.src.box.height,
            blit.dst.box.x, blit.dst.box.x + blit.dst.box.width,
            blit.dst.box.y, blit.dst.box.y + blit.dst.box.height);

        This->pipe->blit(This->pipe, &blit);
    }
    This->pipe->flush(This->pipe, NULL, 0);

    /* really present the frame */
    hr = ID3DPresent_Present(This->present, dwFlags);
    pipe_resource_reference(&resource, NULL);
    if (FAILED(hr)) { return hr; }

    return D3D_OK;
}

HRESULT WINAPI
NineSwapChain9_Present( struct NineSwapChain9 *This,
                        const RECT *pSourceRect,
                        const RECT *pDestRect,
                        HWND hDestWindowOverride,
                        const RGNDATA *pDirtyRegion,
                        DWORD dwFlags )
{
    int i;
    struct pipe_surface *temp;
    HRESULT hr = present(This, pSourceRect, pDestRect,
                         hDestWindowOverride, pDirtyRegion, dwFlags);

    switch (This->params.SwapEffect) {
        case D3DSWAPEFFECT_DISCARD:
            /* rotate the queue */
            temp = This->buffers[0]->surface;
            for (i = 1; i < This->params.BackBufferCount; i++) {
                This->buffers[i-1]->surface = This->buffers[i]->surface;
            }
            This->buffers[This->params.BackBufferCount-1]->surface = temp;
            break;

        case D3DSWAPEFFECT_FLIP:
            /* XXX not implemented */
            break;

        case D3DSWAPEFFECT_COPY:
            /* do nothing */
            break;

        case D3DSWAPEFFECT_OVERLAY:
            /* XXX not implemented */
            break;

        case D3DSWAPEFFECT_FLIPEX:
            /* XXX not implemented */
            break;
    }
    This->device->state.changed.group |= NINE_STATE_FB;
    nine_update_state(This->device); /* XXX: expose fb update separately ? */

    return hr;
}

HRESULT WINAPI
NineSwapChain9_GetFrontBufferData( struct NineSwapChain9 *This,
                                   IDirect3DSurface9 *pDestSurface )
{
    /* TODO: GetFrontBuffer() and copy the contents */
    STUB(D3DERR_INVALIDCALL);
}

HRESULT WINAPI
NineSwapChain9_GetBackBuffer( struct NineSwapChain9 *This,
                              UINT iBackBuffer,
                              D3DBACKBUFFER_TYPE Type,
                              IDirect3DSurface9 **ppBackBuffer )
{
    (void)user_error(Type == D3DBACKBUFFER_TYPE_MONO);
    user_assert(iBackBuffer < This->params.BackBufferCount, D3DERR_INVALIDCALL);
    user_assert(ppBackBuffer != NULL, E_POINTER);

    NineUnknown_AddRef(NineUnknown(This->buffers[iBackBuffer]));
    *ppBackBuffer = (IDirect3DSurface9 *)This->buffers[iBackBuffer];
    return D3D_OK;
}

HRESULT WINAPI
NineSwapChain9_GetRasterStatus( struct NineSwapChain9 *This,
                                D3DRASTER_STATUS *pRasterStatus )
{
    user_assert(pRasterStatus != NULL, E_POINTER);
    return ID3DPresent_GetRasterStatus(This->present, pRasterStatus);
}

HRESULT WINAPI
NineSwapChain9_GetDisplayMode( struct NineSwapChain9 *This,
                               D3DDISPLAYMODE *pMode )
{
    D3DDISPLAYMODEEX mode;
    HRESULT hr;

    user_assert(pMode != NULL, E_POINTER);

    hr = ID3DPresent_GetDisplayMode(This->present, &mode);
    if (SUCCEEDED(hr)) {
        pMode->Width = mode.Width;
        pMode->Height = mode.Height;
        pMode->RefreshRate = mode.RefreshRate;
        pMode->Format = mode.Format;
    }
    return hr;
}

HRESULT WINAPI
NineSwapChain9_GetDevice( struct NineSwapChain9 *This,
                          IDirect3DDevice9 **ppDevice )
{
    user_assert(ppDevice != NULL, E_POINTER);
    *ppDevice = (IDirect3DDevice9 *)This->device;
    return D3D_OK;
}

HRESULT WINAPI
NineSwapChain9_GetPresentParameters( struct NineSwapChain9 *This,
                                     D3DPRESENT_PARAMETERS *pPresentationParameters )
{
    user_assert(pPresentationParameters != NULL, E_POINTER);
    *pPresentationParameters = This->params;
    return D3D_OK;
}

IDirect3DSwapChain9Vtbl NineSwapChain9_vtable = {
    (void *)NineUnknown_QueryInterface,
    (void *)NineUnknown_AddRef,
    (void *)NineUnknown_Release,
    (void *)NineSwapChain9_Present,
    (void *)NineSwapChain9_GetFrontBufferData,
    (void *)NineSwapChain9_GetBackBuffer,
    (void *)NineSwapChain9_GetRasterStatus,
    (void *)NineSwapChain9_GetDisplayMode,
    (void *)NineSwapChain9_GetDevice,
    (void *)NineSwapChain9_GetPresentParameters
};

static const GUID *NineSwapChain9_IIDs[] = {
    &IID_IDirect3DSwapChain9,
    &IID_IUnknown,
    NULL
};

HRESULT
NineSwapChain9_new( struct NineDevice9 *pDevice,
                    BOOL implicit,
                    ID3DPresent *pPresent,
                    PPRESENT_TO_RESOURCE pPTR,
                    HWND hFocusWindow,
                    struct NineSwapChain9 **ppOut )
{
    NINE_NEW(NineSwapChain9, ppOut, /* args */
             pDevice, implicit, pPresent, pPTR, hFocusWindow);
}
