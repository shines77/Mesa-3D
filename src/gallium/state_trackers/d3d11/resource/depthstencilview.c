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

#include "depthstencilview.h"

HRESULT
D3D11DepthStencilView_ctor( struct D3D11DepthStencilView *This,
                            struct D3D11Resource *pResource,
                            struct D3D11UnknownParams *pParams,
                            const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_resource *res;
    struct pipe_surface templ;
    HRESULT hr;

    res = pResource->resource;
    user_assert(res->bind & PIPE_BIND_DEPTH_STENCIL, D3DERR_INVALIDCALL);

    memset(&templ, 0, sizeof(templ));
    if (pDesc) {
        templ.format = d3d11_to_pipe_format(pDesc->Format);
        switch (pDesc->ViewDimension) {
        case D3D11_DSV_DIMENSION_TEXTURE1D:
        case D3D11_DSV_DIMENSION_TEXTURE2D:
            user_assert(res->target == PIPE_TEXTURE_1D ||
                        res->target == PIPE_TEXTURE_2D, E_INVALIDARG);
            templ.u.tex.level = pDesc->Texture1D.MipSlice;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:
        case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
            user_assert(res->target == PIPE_TEXTURE_1D ||
                        res->target == PIPE_TEXTURE_1D_ARRAY ||
                        res->target == PIPE_TEXTURE_2D
                        res->target == PIPE_TEXTURE_2D_ARRAY, E_INVALIDARG);
            templ.u.tex.level = pDesc->Texture1DArray.MipSlice;
            templ.u.tex.first_layer = pDesc->Texture1DArray.FirstArraylice;
            templ.u.tex.last_layer = pDesc->Texture1DArray.FirstArraySlice +
                pDesc->Texture1DArray.ArraySize - 1;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMS:
            user_assert(res->target == PIPE_TEXTURE_2D, E_INVALIDARG);
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY:
            user_assert(res->target == PIPE_TEXTURE_2D_ARRAY, E_INVALIDARG);
            templ.u.tex.first_layer = pDesc->Texture2DMSArray.FirstArraySlice;
            templ.u.tex.last_layer = pDesc->Texture2DMSArray.FirstArraySlice +
                pDesc->Texture2DMSArray.ArraySize - 1;
            break;
        default:
            return_error(E_INVALIDARG);
        }
    } else {
        templ.format = res->format;
        templ.u.tex.last_layer = res->array_size - 1;
    }

    This->surface = pipe->create_surface(pipe, res, &templ);
    if (!This->surface)
        return_error(D3DERR_DRIVERINTERNALERROR);

    This->desc = *pDesc;

    hr = D3D11View_ctor(&This->base, pParams, pResource);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11DepthStencilView_dtor( struct D3D11DepthStencilView *This )
{
    pipe_surface_reference(&This->surface, NULL);
    D3D11View_dtor(&This->base);
}

void WINAPI
D3D11DepthStencilView_GetDesc( struct D3D11DepthStencilView *This,
                               D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;}

ID3D11DepthStencilViewVtbl D3D11DepthStencilView_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11View_GetResource,
    (void *)D3D11DepthStencilView_GetDesc
};

static const GUID *D3D11DepthStencilView_IIDs[] = {
    &IID_ID3D11DepthStencilView,
    &IID_ID3D11View,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11DepthStencilView_new( struct D3D11Device *pDevice,
struct D3D11DepthStencilView **ppOut )
{
    D3D11_NEW(D3D11DepthStencilView, ppOut, pDevice);
}

