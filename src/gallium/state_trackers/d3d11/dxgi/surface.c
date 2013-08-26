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
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIDeviceSubObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGISurface_dtor( struct DXGISurface *This )
{
    DXGIDeviceSubObject_dtor(&This->base);
}

HRESULT WINAPI
DXGISurface_GetDesc( struct DXGISurface *This,
                     DXGI_SURFACE_DESC *pDesc )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISurface_Map( struct DXGISurface *This,
                 DXGI_MAPPED_RECT *pLockedRect,
                 UINT MapFlags )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISurface_Unmap( struct DXGISurface *This )
{
    STUB_return(E_NOTIMPL);
}

IDXGISurfaceVtbl DXGISurface_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
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

