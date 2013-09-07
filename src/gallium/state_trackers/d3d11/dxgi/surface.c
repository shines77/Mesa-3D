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

#include "surface.h"

HRESULT
DXGISurface_ctor( struct DXGISurface *This,
                  struct D3D11UnknownParams *pParams,
                  struct D3D11Texture2D *pTex,
                  const DXGI_SURFACE_DESC *pDesc )
{
    D3D11_TEXTURE2D_DESC desc;
    HRESULT hr;

    hr = DXGIDeviceSubObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;
    This->desc = *pDesc;

    if (!pTex) {
        desc.Width = pDesc->Width;
        desc.Height = pDesc->Height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = pDesc->Format;
        desc.SampleDesc = pDesc->SampleDesc;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags =
            D3D11_RESOURCE_MISC_SHARED |
            D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

        hr = D3D11Texture2D_new(&desc, &This->tex);
        if (FAILED(hr))
            return hr;
    } else {
        This->tex = pTex;
    }

    return S_OK;
}

void
DXGISurface_dtor( struct DXGISurface *This )
{
    DXGIDeviceSubObject_dtor(&This->base);
    D3D11Texture2D_dtor(This->tex);
}

HRESULT WINAPI
DXGISurface_GetDesc( struct DXGISurface *This,
                     DXGI_SURFACE_DESC *pDesc )
{
    user_assert(pDesc, E_POINTER);
    *pDesc = This->desc;
    return S_OK;
}

HRESULT WINAPI
DXGISurface_Map( struct DXGISurface *This,
                 DXGI_MAPPED_RECT *pLockedRect,
                 UINT MapFlags )
{
    struct D3D11Device *dev = This->tex->base.base.device;
    HRESULT hr;

    hr = D3D11DeviceContext_Map(dev->immediate, MapFlags);
    if (SUCCEEDED(hr)) {
        pLockedRect->x = ;
    }
    return hr;
}

HRESULT WINAPI
DXGISurface_Unmap( struct DXGISurface *This )
{
    struct D3D11Device *dev = This->tex->base.base.device;
    return D3D11DeviceContext_Unmap(dev->immediate, This->tex);
}


/* COM crazyness: */

HRESULT WINAPI
DXGISurface_QueryInterface( struct DXGISurface *This,
                            REFIID riid,
                            void **ppvObject )
{
    HRESULT hr = D3D11Unknown_QueryInterface((void *)This, riid, ppvObject);
    if (FAILED(hr))
        hr = D3D11Unknown_QueryInterface((void *)This->tex, riid, ppvObject);
    return hr;
}
ULONG WINAPI
DXGISurface_AddRef( struct DXGISurface *This )
{
    ULONG r = D3D11Unknown_AddRef((void *)This);
    ULONG s = D3D11Unknown_AddRef((void *)This->tex);
    assert(r == s);
    return r;
}
ULONG WINAPI
DXGISurface_Release( struct DXGISurface *This )
{
    ULONG r = D3D11Unknown_Release((void *)This);
    ULONG s = D3D11Unknown_Release((void *)This->tex);
    assert(r == s);
    return r;
}

IDXGISurfaceVtbl DXGISurface_vtable = {
    (void *)DXGISurface_QueryInterface,
    (void *)DXGISurface_AddRef,
    (void *)DXGISurface_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDeviceSubObject_GetDevice,
    (void *)DXGISurface_GetDesc,
    (void *)DXGISurface_Map,
    (void *)DXGISurface_Unmap
};

static const GUID *DXGISurface_IIDs[] = {
    &IID_IDXGISurface,
    &IID_IDXGIDeviceSubObject,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGISurface_new( struct D3D11Device *pDevice,
struct DXGISurface **ppOut )
{
    D3D11_NEW(DXGISurface, ppOut, pDevice);
}

