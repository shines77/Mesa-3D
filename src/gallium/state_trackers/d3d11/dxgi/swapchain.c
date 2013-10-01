/*
 * Copyright 2013 Christoph Bumiller
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the Software),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "swapchain.h"

HRESULT
DXGISwapChain_ctor( struct DXGISwapChain *This,
                    struct D3D11UnknownParams *pParams,
                    struct DXGIDevice *pDevice,
                    const DXGI_SWAPCHAIN_DESC *pDesc )
{
    HRESULT hr;

    hr = DXGIDeviceSubObject_ctor(&This->base, pParams, pDevice);
    if (FAILED(hr))
        return hr;

    hr = DXGISwapChain_ResizeBuffers(This,
                                     This->desc.BufferCount,
                                     This->desc.BufferDesc.Width,
                                     This->desc.BufferDesc.Height,
                                     This->desc.BufferDesc.Format,
                                     This->desc.Flags);
    if (FAILED(hr))
        return hr;
    This->desc = *pDesc;

    return S_OK;
}

void
DXGISwapChain_dtor( struct DXGISwapChain *This )
{
    unsigned i;

    for (i = 0; i < This->num_buffers; ++i)
        com_ref(&This->buffer[i], NULL);

    com_ref(&This->output, NULL);

    DXGIDeviceSubObject_dtor(&This->base);
}

HRESULT WINAPI
DXGISwapChain_Present( struct DXGISwapChain *This,
                       UINT SyncInterval,
                       UINT Flags )
{
    struct native_present_control ctrl;
    HRESULT hr;
    struct pipe_context *pipe;

    STUB_return(E_NOTIMPL);

    {
        struct pipe_blit_info blit;

        blit.dst.resource = ;
        blit.dst.level = ;
        blit.dst.format = ;
        u_box_2d(x, y, w, h, &blit.dst.box);

        blit.src.resource = ;
        blit.src.level = ;
        blit.src.format = ;
        u_box_2d(x, y, w, h, &blit.src.box);

        blit.mask = PIPE_MASK_RGBA;
        blit.filter = PIPE_TEX_FILTER_LINEAR;
        blit.scissor_enable = FALSE;
        blit.alpha_blend = FALSE;
    }

    ctrl.natt = ;
    ctrl.preserve = x != DXGI_SWAP_EFFECT_DISCARD;
    ctrl.swap_interval = SyncInterval;
    ctrl.premultiplied_alpha = TRUE;
    ctrl.num_rects = 0;
    ctrl.rects = NULL;

    if (Flags & DXGI_PRESENT_TEST)
        return S_OK;
    hr = surf->present(surf, &ctrl) ? S_OK : E_FAIL;

    if (FAILED(hr))
        Error(hr, "Present failed.\n");
    return hr;
}

HRESULT WINAPI
DXGISwapChain_GetBuffer( struct DXGISwapChain *This,
                         UINT Buffer,
                         REFIID riid,
                         void **ppSurface )
{
    user_assert(Buffer < This->num_buffers, DXGI_ERROR_INVALID_CALL);

    return IUnknown_QueryInterface(This->buffer[Buffer], riid, ppSurface);
}

HRESULT WINAPI
DXGISwapChain_SetFullscreenState( struct DXGISwapChain *This,
                                  BOOL Fullscreen,
                                  IDXGIOutput *pTarget )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetFullscreenState( struct DXGISwapChain *This,
                                  BOOL *pFullscreen,
                                  IDXGIOutput **ppTarget )
{
    if (pFullscreen)
        *pFullscreen = This->fullscreen;
    if (ppTarget)
        com_ref(ppTarget, This->target);
    return S_OK;
}

HRESULT WINAPI
DXGISwapChain_GetDesc( struct DXGISwapChain *This,
                       DXGI_SWAP_CHAIN_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
    return S_OK;
}

HRESULT WINAPI
DXGISwapChain_ResizeBuffers( struct DXGISwapChain *This,
                             UINT BufferCount,
                             UINT Width,
                             UINT Height,
                             DXGI_FORMAT NewFormat,
                             UINT SwapChainFlags )
{
    unsigned i;

    STUB_return(E_NOTIMPL);

    for (i = 0; i < This->desc.BufferCount; ++i)
        if (This->buffer[i]->base.base.ref)
            return Error(DXGI_ERROR_INVALID_CALL, "Buffer still referenced.");

    user_assert(BufferCount <= DXGI_MAX_SWAP_CHAIN_BUFFERS,
                DXGI_ERROR_INVALID_CALL);
    if (!BufferCount)
        BufferCount = This->desc.BufferCount;

    if (!Width || !Height) {
        UINT TargetWidth, TargetHeight;
        if (!Width)
            Width = TargetWidth;
        if (!Height)
            Height = TargetHeight;
    }

    if (NewFormat == DXGI_FORMAT_UNKNOWN)
        NewFormat = This->desc.BufferDesc.Format;

    for (i = 0; i < BufferCount; ++i) {
        if (This->buffer[i]) {
            struct pipe_resource templ;
            templ.target = PIPE_TEXTURE_2D;
            templ.format = dxgi_to_pipe_format(NewFormat);
            templ.width0 = Width;
            templ.height0 = Height;
            templ.depth0 = 1;
            templ.nr_samples = 0;
            templ.last_level = 0;
            templ.array_size = 1;
            templ.usage = PIPE_USAGE_DEFAULT;
            templ.bind =
                PIPE_BIND_SAMPLER_VIEW |
                PIPE_BIND_RENDER_TARGET;
        } else {
            hr = DXGISurface_new(device, &This->buffer[i]);
            if (FAILED(hr))
                return Error(hr, "Failed to create DXGISurface.\n");
        }
    }

    This->desc.BufferDesc.Width = Width;
    This->desc.BufferDesc.Height = Height;
    This->desc.BufferDesc.Format = NewFormat;
    This->desc.Flags = SwapChainFlags;

    return Success(S_OK);
}

HRESULT WINAPI
DXGISwapChain_ResizeTarget( struct DXGISwapChain *This,
                            Int *pNewTargetParameters )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetContainingOutput( struct DXGISwapChain *This,
                                   IDXGIOutput **ppOutput )
{
    assert(ppOutput);
    com_set(ppOutput, This->output);
    return S_OK;
}

HRESULT WINAPI
DXGISwapChain_GetFrameStatistics( struct DXGISwapChain *This,
                                  DXGI_FRAME_STATISTICS *pStats )
{
    memset(pStats, 0, sizeof(*pStats));

    pStats->PresentCount = This->present_count;
    pStats->PresentRefreshCount = This->present_count;
    pStats->SyncRefreshCount = This->present_count;

    return S_OK;
}

HRESULT WINAPI
DXGISwapChain_GetLastPresentCount( struct DXGISwapChain *This,
                                   UINT *pLastPresentCount )
{
    assert(pLastPresentCount);
    *pLastPresentCount = This->present_count;
    return S_OK;
}

IDXGISwapChainVtbl DXGISwapChain_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDeviceSubObject_GetDevice,
    (void *)DXGISwapChain_Present,
    (void *)DXGISwapChain_GetBuffer,
    (void *)DXGISwapChain_SetFullscreenState,
    (void *)DXGISwapChain_GetFullscreenState,
    (void *)DXGISwapChain_GetDesc,
    (void *)DXGISwapChain_ResizeBuffers,
    (void *)DXGISwapChain_ResizeTarget,
    (void *)DXGISwapChain_GetContainingOutput,
    (void *)DXGISwapChain_GetFrameStatistics,
    (void *)DXGISwapChain_GetLastPresentCount
};

static const GUID *DXGISwapChain_IIDs[] = {
    &IID_IDXGISwapChain,
    &IID_IDXGIDeviceSubObject,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGISwapChain_new( struct D3D11Device *pDevice,
                   const DXGI_SWAPCHAIN_DESC *pDesc,
                   struct DXGISwapChain **ppOut )
{
    return DXGI_NEW(DXGISwapChain, ppOut, pDevice, pDesc);
}

