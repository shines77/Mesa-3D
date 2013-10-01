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
                 struct D3D11UnknownParams *pParams,
                 const char *name,
                 struct native_connector *connector )
{
    HRESULT hr;
    unsigned i;

    hr = DXGIObject_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    for (i = 0; name[i] && i < Elements(This->desc.DeviceName); ++i)
        This->desc.DeviceName[i] = name[i];
    desc.AttachedToDesktop = TRUE;

    if (connector) {
        modes = parent->display->modeset->get_modes(parent->display, connector, &This->num_modes);
        if (modes && num_modes) {
            This->modes = CALLOC(This->num_modes, sizeof(DXGI_MODE_DESC));
            if (!This->modes)
                return Error(E_OUTOFMEMORY, "");
            for (i = 0; i < This->num_modes; ++i) {
                struct native_mode *m = This->mode[i];
                This->m.Width = m->width;
                This->m.Height = m->height;
                This->m.RefreshRate.Numerator = m->refresh_rate;
                This->modes[i].RefreshRate.Denominator = 1;
                This->modes[i].Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
                This->modes[i].ScanlineOrdering =
                    DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            }
        } else
        if (modes) {
            FREE(modes);
            modes = NULL;
        }
    }
    if (!modes) {
        This->modes = CALLOC_STRUCT(DXGI_MODE_DESC);
        This->modes[0].Width = 1024;
        This->modes[0].Height = 768;
        This->modes[0].RefreshRate.Numerator = 60;
        This->modes[0].RefreshRate.Denominator = 1;
        This->modes[0].Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        This->modes[0].ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    }

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
    assert(pDesc);
    *pDesc = This->desc;
    return S_OK;
}

HRESULT WINAPI
DXGIOutput_GetDisplayModeList( struct DXGIOutput *This,
                               DXGI_FORMAT EnumFormat,
                               UINT Flags,
                               UINT *pNumModes,
                               DXGI_MODE_DESC *pDesc )
{
    unsigned i;
    unsigned n = 0;

    user_assert(pNumModes, DXGI_ERROR_INVALID_CALL);

    if (!pDesc) {
        for (i = 0; i < This->num_modes; ++i) {
            if (This->modes[i].Format == EnumFormat)
                ++n;
        }
        *pNumModes = n;
        return S_OK;
    }

    for (i = 0; i < This->num_modes && n < *pNumModes; ++i) {
        if (This->modes[i].Format != EnumFormat)
            continue;
        pDesc[n].Width = This->modes[i]->width;
        pDesc[n].Height = This->modes[i]->width;
        pDesc[n].RefreshRate.Numerator = This->modes[i]->refresh_rate;
        pDesc[n].RefreshRate.Denominator = 1;
        pDesc[n].Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        pDesc[n].ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        ++n;
    }
    *pNumModes = n;
    return S_OK;
}

HRESULT WINAPI
DXGIOutput_FindClosestMatchingMode( struct DXGIOutput *This,
                                    const DXGI_MODE_DESC *pModeToMatch,
                                    DXGI_MODE_DESC *pClosestMatch,
                                    IUnknown *pConcernedDevice )
{
    float close = 1e9f;
    unsigned i;
    unsigned m = 0;

    user_assert(pModeToMatch && pClosestMatch, E_POINTER);
    user_assert(pModeToMatch->Width != 0 ||
                pModeToMatch->Height == 0, DXGI_ERROR_INVALID_CALL);
    user_assert(pConcernedDevice || pModeToMatch->Format != DXGI_FORMAT_UNKNOWN,
                DXGI_ERROR_INVALID_CALL);
    user_assert(pModeToMatch->RefreshRate.Denominator != 0 ||
                pModeToMatch->RefreshRate.Numerator == 0,
                DXGI_ERROR_INVALID_CALL);

    for (i = 0; i < This->num_modes; ++i) {
        float f, r;
        if (This->modes[i]->Format != pModeToMatch->Format)
            continue;
        f  = pModeToMatch->RefreshRate.Numerator;
        f /= pModeToMatch->RefreshRate.Denominator;
        f /= This->modes[i]->refresh_rate;
        f *= pModeToMatch->Width - This->modes[i]->width;
        f *= pModeToMatch->Height - This->modes[i]->height;

        if (close > fabsf(f)) {
            m = i;
            close = fabsf(f);
        }
    }

    pClosestMatch->Width = This->modes[m]->width;
    pClosestMatch->Height = This->modes[m]->height;
    pClosestMatch->RefreshRate.Numerator = This->modes[m].refresh_rate;
    pClosestMatch->RefreshRate.Denominator = 1;
    pClosestMatch->Format = This->modes[m]->Format;
    pClosestMatch->Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    pClosestMatch->ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    return S_OK;
}

HRESULT WINAPI
DXGIOutput_WaitForVBlank( struct DXGIOutput *This )
{
    STUB_return(S_OK);
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
                            const DXGI_GAMMA_CONTROL *pArray )
{
    assert(pArray);
    This->gamma = *pArray;
    return S_OK;
}

HRESULT WINAPI
DXGIOutput_GetGammaControl( struct DXGIOutput *This,
                            DXGI_GAMMA_CONTROL *pArray )
{
    assert(pArray);
    *pArray = This->gamma;
    return S_OK;
}

HRESULT WINAPI
DXGIOutput_SetDisplaySurface( struct DXGIOutput *This,
                              IDXGISurface *pScanoutSurface )
{
    com_ref(&This->scanout, pScanoutSurface);
    return S_OK;
}

HRESULT WINAPI
DXGIOutput_GetDisplaySurfaceData( struct DXGIOutput *This,
                                  IDXGISurface *pDestination )
{
    return DXGISurface_Copy(pDestination, This->scanout);
}

HRESULT WINAPI
DXGIOutput_GetFrameStatistics( struct DXGIOutput *This,
                               DXGI_FRAME_STATISTICS *pStats )
{
    assert(pStats);
    memset(pStats, 0, sizeof(*pStats));
    STUB_return(S_OK);
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
DXGIOutput_new( struct DXGIAdapter *pAdapter,
                struct DXGIOutput **ppOut )
{
    D3D11_NEW_CHILD(DXGIOutput, ppOut, pAdapter);
}

