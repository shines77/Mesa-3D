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

#include "unorderedaccessview.h"

HRESULT
D3D11UnorderedAccessView_ctor( struct D3D11UnorderedAccessView *This,
                               struct D3D11Resource *pResource,
                               onst D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
                               struct D3D11UnknownParams *pParams)
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_resource *res = pResource->resource;
    struct pipe_surface templ;
    HRESULT hr;

    user_assert(res->bind & PIPE_BIND_SHADER_RESOURCE, D3DERR_INVALIDCALL);

    memset(&templ, 0, sizeof(templ));

    if (!pDesc) {
        This->desc.Format = pipe_to_dxgi_format(res->format);
        switch (res->target) {
        case PIPE_TEXTURE_1D:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
            break;
        case PIPE_TEXTURE_1D_ARRAY:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
            This->desc.Texture2DArray.ArraySize = res->array_size;
            break;
        case PIPE_TEXTURE_2D:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            break;
        case PIPE_TEXTURE_2D_ARRAY:
        case PIPE_TEXTURE_CUBE:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
            This->desc.Texture2DArray.ArraySize = res->array_size;
            break;
        case PIPE_TEXTURE_3D:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
            This->desc.Texture3D.WSize = res->depth0;
            break;
        case PIPE_BUFFER:
            This->desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            This->desc.Buffer.NumElements =
                res->width0 / util_format_get_blocksize(res->format);
            break;
        }
    } else {
        This->desc = *pDesc;
    }
    pDesc = &This->desc;

    templ.format = dxgi_to_pipe_format(pDesc->Format);
    templ.writable = 1;
    switch (pDesc->ViewDimension) {
    case D3D11_UAV_DIMENSION_BUFFER:
        user_assert(!(pDesc->Buffer.Flags & D3D11_BUFFER_UAV_FLAG_RAW) ||
                    pDesc->Format == DXGI_FORMAT_R32_TYPELESS, E_INVALIDARG);
        templ.u.buf.first_element = pDesc->Buffer.FirstElement;
        templ.u.buf.last_element =
            pDesc->Buffer.FirstElement + pDesc->Buffer.NumElements - 1;
        break;
    case D3D11_UAV_DIMENSION_TEXTURE1D:
    case D3D11_UAV_DIMENSION_TEXTURE2D:
        templ.u.tex.level = pDesc->Texture1D.MipSlice;
        break;
    case D3D11_UAV_DIMENSION_TEXTURE1DARRAY:
    case D3D11_UAV_DIMENSION_TEXTURE2DARRAY:
        templ.u.tex.level = pDesc->Texture1DArray.MipSlice;
        templ.u.tex.first_layer = pDesc->Texture1DArray.FirstArraySlice;
        templ.u.tex.last_layer = pDesc->Texture1DArray.FirstArraySlice +
            pDesc->Texture1DArray.ArraySize - 1;
        break;
    case D3D11_UAV_DIMENSION_TEXTURE3D:
        templ.u.tex.level = pDesc->Texture3D.MipSlice;
        templ.u.tex.first_layer = pDesc->Texture3D.FirstWSlice;
        templ.u.tex.last_layer =
            pDesc->Texture3D.FirstWSlice + pDesc->Texture3D.WSize - 1;
        break;
    default:
        return_error(E_INVALIDARG);
    }

    hr = D3D11View_ctor(&This->base, pParams, pResource);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11UnorderedAccessView_dtor( struct D3D11UnorderedAccessView *This )
{
    pipe_surface_reference(&This->surface, NULL);
    D3D11View_dtor(&This->base);
}

void WINAPI
D3D11UnorderedAccessView_GetDesc( struct D3D11UnorderedAccessView *This,
                                  D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11UnorderedAccessViewVtbl D3D11UnorderedAccessView_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11View_GetResource,
    (void *)D3D11UnorderedAccessView_GetDesc
};

static const GUID *D3D11UnorderedAccessView_IIDs[] = {
    &IID_ID3D11UnorderedAccessView,
    &IID_ID3D11View,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11UnorderedAccessView_new( struct D3D11Device *pDevice,
struct D3D11UnorderedAccessView **ppOut )
{
    D3D11_NEW(D3D11UnorderedAccessView, ppOut, pDevice);
}

