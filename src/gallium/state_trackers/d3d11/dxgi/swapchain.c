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

#include "swapchain.h"

HRESULT
DXGISwapChain_ctor( struct DXGISwapChain *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIDeviceSubObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGISwapChain_dtor( struct DXGISwapChain *This )
{
    DXGIDeviceSubObject_dtor(&This->base);
}

HRESULT WINAPI
DXGISwapChain_Present( struct DXGISwapChain *This,
                       UINT SyncInterval,
                       UINT Flags )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetBuffer( struct DXGISwapChain *This,
                         UINT Buffer,
                         REFIID riid,
                         void **ppSurface )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_SetFullscreenState( struct DXGISwapChain *This,
                                  BOOL Fullscreen,
                                  IDXGIOutput *pTarget )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetFullscreenState( struct DXGISwapChain *This,
                                  BOOL *pFullscreen,
                                  IDXGIOutput **ppTarget )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetDesc( struct DXGISwapChain *This,
                       DXGI_SWAP_CHAIN_DESC *pDesc )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_ResizeBuffers( struct DXGISwapChain *This,
                             UINT BufferCount,
                             UINT Width,
                             UINT Height,
                             Int NewFormat,
                             UINT SwapChainFlags )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_ResizeTarget( struct DXGISwapChain *This,
                            Int *pNewTargetParameters )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetContainingOutput( struct DXGISwapChain *This,
                                   IDXGIOutput **ppOutput )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetFrameStatistics( struct DXGISwapChain *This,
                                  DXGI_FRAME_STATISTICS *pStats )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGISwapChain_GetLastPresentCount( struct DXGISwapChain *This,
                                   UINT *pLastPresentCount )
{
    STUB_return(E_NOTIMPL);
}

IDXGISwapChainVtbl DXGISwapChain_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIDeviceSubObject_GetDevice,
    (void *)DXGISwapChain_Present,
    (void *)DXGISwapChain_GetBuffer,
    (void *)DXGISwapChain_SetFullscreenState,
    (void *)DXGISwapChain_GetFullscreenState,
    (void *)DXGISwapChain_GetDesc,
    (void *)DXGISwapChain_ResizeBuffers,
    (void *)DXGISwapChain_ResizeTarget,
    (void *)DXGISwapChain_GetContainingOutput,
    (void *)DXGISwapChain_GetFrameStatistics,
    (void *)DXGISwapChain_GetLastPresentCount
};

static const GUID *DXGISwapChain_IIDs[] = {
    &IID_IDXGISwapChain,
    &IID_IDXGIDeviceSubObject,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGISwapChain_new( struct D3D11Device *pDevice,
struct DXGISwapChain **ppOut )
{
    D3D11_NEW(DXGISwapChain, ppOut, pDevice);
}

