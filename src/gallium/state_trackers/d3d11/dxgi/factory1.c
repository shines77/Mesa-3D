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

#include "factory1.h"

HRESULT
DXGIFactory1_ctor( struct DXGIFactory1 *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIFactory_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIFactory1_dtor( struct DXGIFactory1 *This )
{
    DXGIFactory_dtor(&This->base);
}

HRESULT WINAPI
DXGIFactory1_EnumAdapters1( struct DXGIFactory1 *This,
                            UINT Adapter,
                            IDXGIAdapter1 **ppAdapter )
{
    STUB_return(E_NOTIMPL);
}

BOOL WINAPI
DXGIFactory1_IsCurrent( struct DXGIFactory1 *This )
{
    STUB_return(0);
}

IDXGIFactory1Vtbl DXGIFactory1_vtable = {
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
    (void *)DXGIFactory_CreateSoftwareAdapter,
    (void *)DXGIFactory1_EnumAdapters1,
    (void *)DXGIFactory1_IsCurrent
};

static const GUID *DXGIFactory1_IIDs[] = {
    &IID_IDXGIFactory1,
    &IID_IDXGIFactory,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIFactory1_new( struct D3D11Device *pDevice,
struct DXGIFactory1 **ppOut )
{
    D3D11_NEW(DXGIFactory1, ppOut, pDevice);
}

