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

#include "output.h"

HRESULT
DXGIOutput_ctor( struct DXGIOutput *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
DXGIOutput_dtor( struct DXGIOutput *This )
{
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIOutput_GetDesc( struct DXGIOutput *This,
                    DXGI_OUTPUT_DESC *pDesc )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_GetDisplayModeList( struct DXGIOutput *This,
                               Int EnumFormat,
                               UINT Flags,
                               UINT *pNumModes,
                               FunctionProto __out_ecount_part_opt )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_FindClosestMatchingMode( struct DXGIOutput *This,
                                    Int *pModeToMatch,
                                    Int *pClosestMatch,
                                    IUnknown *pConcernedDevice )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_WaitForVBlank( struct DXGIOutput *This )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_TakeOwnership( struct DXGIOutput *This,
                          IUnknown *pDevice,
                          BOOL Exclusive )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
DXGIOutput_ReleaseOwnership( struct DXGIOutput *This )
{
    STUB();
}

HRESULT WINAPI
DXGIOutput_GetGammaControlCapabilities( struct DXGIOutput *This,
                                        Int *pGammaCaps )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_SetGammaControl( struct DXGIOutput *This,
                            Int *pArray )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_GetGammaControl( struct DXGIOutput *This,
                            Int *pArray )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_SetDisplaySurface( struct DXGIOutput *This,
                              IDXGISurface *pScanoutSurface )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_GetDisplaySurfaceData( struct DXGIOutput *This,
                                  IDXGISurface *pDestination )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIOutput_GetFrameStatistics( struct DXGIOutput *This,
                               DXGI_FRAME_STATISTICS *pStats )
{
    STUB_return(E_NOTIMPL);
}

IDXGIOutputVtbl DXGIOutput_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIOutput_GetDesc,
    (void *)DXGIOutput_GetDisplayModeList,
    (void *)DXGIOutput_FindClosestMatchingMode,
    (void *)DXGIOutput_WaitForVBlank,
    (void *)DXGIOutput_TakeOwnership,
    (void *)DXGIOutput_ReleaseOwnership,
    (void *)DXGIOutput_GetGammaControlCapabilities,
    (void *)DXGIOutput_SetGammaControl,
    (void *)DXGIOutput_GetGammaControl,
    (void *)DXGIOutput_SetDisplaySurface,
    (void *)DXGIOutput_GetDisplaySurfaceData,
    (void *)DXGIOutput_GetFrameStatistics
};

static const GUID *DXGIOutput_IIDs[] = {
    &IID_IDXGIOutput,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIOutput_new( struct D3D11Device *pDevice,
struct DXGIOutput **ppOut )
{
    D3D11_NEW(DXGIOutput, ppOut, pDevice);
}

