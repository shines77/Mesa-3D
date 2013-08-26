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

#include "keyedmutex.h"

HRESULT
DXGIKeyedMutex_ctor( struct DXGIKeyedMutex *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIDeviceSubObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIKeyedMutex_dtor( struct DXGIKeyedMutex *This )
{
    DXGIDeviceSubObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIKeyedMutex_AcquireSync( struct DXGIKeyedMutex *This,
                            UINT64 Key,
                            DWORD dwMilliseconds )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIKeyedMutex_ReleaseSync( struct DXGIKeyedMutex *This,
                            UINT64 Key )
{
    STUB_return(E_NOTIMPL);
}

IDXGIKeyedMutexVtbl DXGIKeyedMutex_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDeviceSubObject_GetDevice,
    (void *)DXGIKeyedMutex_AcquireSync,
    (void *)DXGIKeyedMutex_ReleaseSync
};

static const GUID *DXGIKeyedMutex_IIDs[] = {
    &IID_IDXGIKeyedMutex,
    &IID_IDXGIDeviceSubObject,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIKeyedMutex_new( struct D3D11Device *pDevice,
struct DXGIKeyedMutex **ppOut )
{
    D3D11_NEW(DXGIKeyedMutex, ppOut, pDevice);
}

