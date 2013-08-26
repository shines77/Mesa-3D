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

#include "device.h"

HRESULT
DXGIDevice_ctor( struct DXGIDevice *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIDevice_dtor( struct DXGIDevice *This )
{
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIDevice_GetAdapter( struct DXGIDevice *This,
                       IDXGIAdapter **pAdapter )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIDevice_CreateSurface( struct DXGIDevice *This,
                          DXGI_SURFACE_DESC *pDesc,
                          UINT NumSurfaces,
                          DXGI_USAGE Usage,
                          DXGI_SHARED_RESOURCE *pSharedResource,
                          IDXGISurface **ppSurface )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIDevice_QueryResourceResidency( struct DXGIDevice *This,
                                   FunctionNoProto __in_ecount )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIDevice_SetGPUThreadPriority( struct DXGIDevice *This,
                                 INT Priority )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIDevice_GetGPUThreadPriority( struct DXGIDevice *This,
                                 INT *pPriority )
{
    STUB_return(E_NOTIMPL);
}

IDXGIDeviceVtbl DXGIDevice_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDevice_GetAdapter,
    (void *)DXGIDevice_CreateSurface,
    (void *)DXGIDevice_QueryResourceResidency,
    (void *)DXGIDevice_SetGPUThreadPriority,
    (void *)DXGIDevice_GetGPUThreadPriority
};

static const GUID *DXGIDevice_IIDs[] = {
    &IID_IDXGIDevice,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIDevice_new( struct D3D11Device *pDevice,
struct DXGIDevice **ppOut )
{
    D3D11_NEW(DXGIDevice, ppOut, pDevice);
}

