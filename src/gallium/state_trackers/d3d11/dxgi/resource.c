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

#include "resource.h"

HRESULT
DXGIResource_ctor( struct DXGIResource *This,
                   struct D3D11UnknownParams *pParams )
{
    HRESULT hr = DXGIDeviceSubObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIResource_dtor( struct DXGIResource *This )
{
    DXGIDeviceSubObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIResource_GetSharedHandle( struct DXGIResource *This,
                              HANDLE *pSharedHandle )
{
    if (!This->handle)
        return Error(DXGI_ERROR_NOT_FOUND, "Resource cannot be shared.\n");
    *pSharedHandle = handle;
    return S_OK;
}

HRESULT WINAPI
DXGIResource_GetUsage( struct DXGIResource *This,
                       DXGI_USAGE *pUsage )
{
    assert(pUsage);
    *pUsage = This->usage;
    return S_OK;
}

HRESULT WINAPI
DXGIResource_SetEvictionPriority( struct DXGIResource *This,
                                  UINT EvictionPriority )
{
    This->priority = EvictionPriority;
    return S_OK;
}

HRESULT WINAPI
DXGIResource_GetEvictionPriority( struct DXGIResource *This,
                                  UINT *pEvictionPriority )
{
    assert(pEvictionPriority);
    *pEvictionPriority = This->priority;
    return S_OK;
}

IDXGIResourceVtbl DXGIResource_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDeviceSubObject_GetDevice,
    (void *)DXGIResource_GetSharedHandle,
    (void *)DXGIResource_GetUsage,
    (void *)DXGIResource_SetEvictionPriority,
    (void *)DXGIResource_GetEvictionPriority
};

static const GUID *DXGIResource_IIDs[] = {
    &IID_IDXGIResource,
    &IID_IDXGIDeviceSubObject,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIResource_new( struct D3D11Device *pDevice,
                  struct DXGIResource **ppOut )
{
    D3D11_NEW(DXGIResource, ppOut, pDevice);
}

