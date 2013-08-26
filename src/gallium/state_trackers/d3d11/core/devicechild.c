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

#include "devicechild.h"

HRESULT
D3D11DeviceChild_ctor( struct D3D11DeviceChild *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = Unknown_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11DeviceChild_dtor( struct D3D11DeviceChild *This )
{
    Unknown_dtor(&This->base);
}

void WINAPI
D3D11DeviceChild_GetDevice( struct D3D11DeviceChild *This,
                            ID3D11Device **ppDevice )
{
    STUB();
}

HRESULT WINAPI
D3D11DeviceChild_GetPrivateData( struct D3D11DeviceChild *This,
                                 REFGUID guid,
                                 UINT *pDataSize,
                                 void *pData )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11DeviceChild_SetPrivateData( struct D3D11DeviceChild *This,
                                 REFGUID guid,
                                 UINT DataSize,
                                 void *pData )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11DeviceChild_SetPrivateDataInterface( struct D3D11DeviceChild *This,
                                          REFGUID guid,
                                          IUnknown *pData )
{
    STUB_return(E_NOTIMPL);
}

ID3D11DeviceChildVtbl D3D11DeviceChild_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface
};

static const GUID *D3D11DeviceChild_IIDs[] = {
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11DeviceChild_new( struct D3D11Device *pDevice,
struct D3D11DeviceChild **ppOut )
{
    D3D11_NEW(D3D11DeviceChild, ppOut, pDevice);
}

