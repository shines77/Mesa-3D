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

#include "adapter.h"

HRESULT
DXGIAdapter_ctor( struct DXGIAdapter *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIAdapter_dtor( struct DXGIAdapter *This )
{
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIAdapter_EnumOutputs( struct DXGIAdapter *This,
                         UINT Output,
                         IDXGIOutput **ppOutput )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIAdapter_GetDesc( struct DXGIAdapter *This,
                     DXGI_ADAPTER_DESC *pDesc )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIAdapter_CheckInterfaceSupport( struct DXGIAdapter *This,
                                   REFGUID InterfaceName,
                                   LARGE_INTEGER *pUMDVersion )
{
    STUB_return(E_NOTIMPL);
}

IDXGIAdapterVtbl DXGIAdapter_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIAdapter_EnumOutputs,
    (void *)DXGIAdapter_GetDesc,
    (void *)DXGIAdapter_CheckInterfaceSupport
};

static const GUID *DXGIAdapter_IIDs[] = {
    &IID_IDXGIAdapter,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIAdapter_new( struct D3D11Device *pDevice,
struct DXGIAdapter **ppOut )
{
    D3D11_NEW(DXGIAdapter, ppOut, pDevice);
}

