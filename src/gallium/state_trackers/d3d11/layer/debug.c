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

#include "debug.h"

HRESULT
D3D11Debug_ctor( struct D3D11Debug *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = Unknown_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11Debug_dtor( struct D3D11Debug *This )
{
    Unknown_dtor(&This->base);
}

HRESULT WINAPI
D3D11Debug_SetFeatureMask( struct D3D11Debug *This,
                           UINT Mask )
{
    STUB_return(E_NOTIMPL);
}

UINT WINAPI
D3D11Debug_GetFeatureMask( struct D3D11Debug *This )
{
    STUB_return(0);
}

HRESULT WINAPI
D3D11Debug_SetPresentPerRenderOpDelay( struct D3D11Debug *This,
                                       UINT Milliseconds )
{
    STUB_return(E_NOTIMPL);
}

UINT WINAPI
D3D11Debug_GetPresentPerRenderOpDelay( struct D3D11Debug *This )
{
    STUB_return(0);
}

HRESULT WINAPI
D3D11Debug_SetSwapChain( struct D3D11Debug *This,
                         Int *pSwapChain )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Debug_GetSwapChain( struct D3D11Debug *This,
                         Int **ppSwapChain )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Debug_ValidateContext( struct D3D11Debug *This,
                            Int *pContext )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Debug_ReportLiveDeviceObjects( struct D3D11Debug *This,
                                    D3D11_RLDO_FLAGS Flags )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Debug_ValidateContextForDispatch( struct D3D11Debug *This,
                                       Int *pContext )
{
    STUB_return(E_NOTIMPL);
}

ID3D11DebugVtbl D3D11Debug_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11Debug_SetFeatureMask,
    (void *)D3D11Debug_GetFeatureMask,
    (void *)D3D11Debug_SetPresentPerRenderOpDelay,
    (void *)D3D11Debug_GetPresentPerRenderOpDelay,
    (void *)D3D11Debug_SetSwapChain,
    (void *)D3D11Debug_GetSwapChain,
    (void *)D3D11Debug_ValidateContext,
    (void *)D3D11Debug_ReportLiveDeviceObjects,
    (void *)D3D11Debug_ValidateContextForDispatch
};

static const GUID *D3D11Debug_IIDs[] = {
    &IID_ID3D11Debug,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11Debug_new( struct D3D11Device *pDevice,
struct D3D11Debug **ppOut )
{
    D3D11_NEW(D3D11Debug, ppOut, pDevice);
}

