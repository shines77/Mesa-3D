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

#include "factory.h"

HRESULT
DXGIFactory_ctor( struct DXGIFactory *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIFactory_dtor( struct DXGIFactory *This )
{
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIFactory_EnumAdapters( struct DXGIFactory *This,
                          UINT Adapter,
                          IDXGIAdapter **ppAdapter )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIFactory_MakeWindowAssociation( struct DXGIFactory *This,
                                   HWND WindowHandle,
                                   UINT Flags )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIFactory_GetWindowAssociation( struct DXGIFactory *This,
                                  HWND *pWindowHandle )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIFactory_CreateSwapChain( struct DXGIFactory *This,
                             IUnknown *pDevice,
                             DXGI_SWAP_CHAIN_DESC *pDesc,
                             IDXGISwapChain **ppSwapChain )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIFactory_CreateSoftwareAdapter( struct DXGIFactory *This,
                                   Int Module,
                                   IDXGIAdapter **ppAdapter )
{
    STUB_return(E_NOTIMPL);
}

IDXGIFactoryVtbl DXGIFactory_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIFactory_EnumAdapters,
    (void *)DXGIFactory_MakeWindowAssociation,
    (void *)DXGIFactory_GetWindowAssociation,
    (void *)DXGIFactory_CreateSwapChain,
    (void *)DXGIFactory_CreateSoftwareAdapter
};

static const GUID *DXGIFactory_IIDs[] = {
    &IID_IDXGIFactory,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIFactory_new( struct D3D11Device *pDevice,
struct DXGIFactory **ppOut )
{
    D3D11_NEW(DXGIFactory, ppOut, pDevice);
}

