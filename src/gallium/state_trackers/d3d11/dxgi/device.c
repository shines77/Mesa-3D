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
                 struct D3D11UnknownParams *pParams )
{
    HRESULT hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

HRESULT WINAPI
DXGIDevice_QueryInterface( struct DXGIDevice *This,
                           REFIID riid,
                           void *ppvObject )
{
    HRESULT hr = D3D11Unknown_QueryInterface((void *)This, riid, ppvObject);
    if (FAILED(hr))
        hr = D3D11Unknown_QueryInterface((void *)This->device, riid, ppvObject);
    return hr;
}

void
DXGIDevice_dtor( struct DXGIDevice *This )
{
    com_ref(&This->adapter, NULL);
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIDevice_GetAdapter( struct DXGIDevice *This,
                       IDXGIAdapter **pAdapter )
{
    assert(pAdapter);
    com_set(pAdapter, This->adapter);
    return S_OK;
}

HRESULT WINAPI
DXGIDevice_CreateSurface( struct DXGIDevice *This,
                          DXGI_SURFACE_DESC *pDesc,
                          UINT NumSurfaces,
                          DXGI_USAGE Usage,
                          DXGI_SHARED_RESOURCE *pSharedResource,
                          IDXGISurface **ppSurface )
{
    struct DXGISurface *surf;
    UINT i;

    user_assert(NumSurfaces, DXGI_ERROR_INVALID_CALL);

    for (i = NumSurfaces - 1; i >= 0; --i) {
        DXGISurface_new(This, pDesc);
    }
    *ppSurface = (IDXGISurface *)surf;
    return S_OK;
}

HRESULT WINAPI
DXGIDevice_QueryResourceResidency( struct DXGIDevice *This,
                                   Unknown *const *ppResources,
                                   DXGI_RESIDENCY *pResidencyStatus,
                                   UINT NumResources )
{
    unsigned i;

    if (!NumResources)
        return S_OK;
    user_assert(ppResources && pResidencyStats, E_POINTER);

    /* TODO: return actual status */
    for (i = 0; i < NumResources; ++i)
        pResidencyStatus[i] = DXGI_RESIDENCY_FULLY_RESIDENT;

    return S_OK;
}

HRESULT WINAPI
DXGIDevice_SetGPUThreadPriority( struct DXGIDevice *This,
                                 INT Priority )
{
    user_assert(Priority >= -7 && Priority <= +7, DXGI_ERROR_INVALID_CALL);
    This->priority = priority;
    return S_OK;
}

HRESULT WINAPI
DXGIDevice_GetGPUThreadPriority( struct DXGIDevice *This,
                                 INT *pPriority )
{
    assert_pointer(pPriority);
    *pPriority = This->priority;
    return S_OK;
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

