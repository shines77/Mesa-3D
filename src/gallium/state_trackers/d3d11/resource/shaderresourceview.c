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

#include "shaderresourceview.h"

static HRESULT
D3D11ShaderResourceView_AdjustMipLevels( struct D3D11ShaderResourceView *This,
                                         const unsigned Levels )
{
    switch (This->desc.ViewDimension) {
    case D3D11_SRV_DIMENSION_TEXTURE1D:
    case D3D11_SRV_DIMENSION_TEXTURE2D:
    case D3D11_SRV_DIMENSION_TEXTURECUBE:
    case D3D11_SRV_DIMENSION_TEXTURE3D:
    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
    case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
        if (This->desc.Texture1D.MipLevels == -1)
            This->desc.Texture1D.MipLevels = Levels;
        if (This->desc.Texture1D.MipLevels > Levels)
            return E_INVALIDARG;
        break;
    default:
        break;
    }
    return S_OK;
}

HRESULT
D3D11ShaderResourceView_ctor( struct D3D11ShaderResourceView *This,
                              struct D3D11UnknownParams *pParams,
                              struct D3D11Resource *pResource,
                              const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_resource *res = pResource->resource;
    struct pipe_sampler_view templ;
    HRESULT hr;

    user_assert(res->bind & PIPE_BIND_SAMPLER_VIEW, D3DERR_INVALIDCALL);

    if (pDesc) {
        This->desc = *pDesc;
    } else {
        This->desc.Format = pipe_to_dxgi_format(res->format);
        switch (res->target) {
        case PIPE_TEXTURE_1D:
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
            This->desc.Texture1D.MipLevels = res->last_level + 1;
            break;
        case PIPE_TEXTURE_2D:
            if (res->nr_samples > 1) {
                This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            } else {
                This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                This->desc.Texture2D.MipLevels = res->last_level + 1;
            }
            break;
        case PIPE_TEXTURE_3D:
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
            This->desc.Texture3D.MipLevels = res->last_level + 1;
            break;
        case PIPE_TEXTURE_1D_ARRAY:
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
            This->desc.Texture1DArray.MipLevels = res->last_level + 1;
            break;
        case PIPE_TEXTURE_2D_ARRAY:
            if (res->nr_samples > 1) {
                This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            } else {
                This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                This->desc.Texture2DArray.MipLevels = res->last_level + 1;
            }
            break;
        case PIPE_TEXTURE_CUBE:
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
            This->desc.TextureCube.MipLevels = res->last_level + 1;
            break;
        case PIPE_TEXTURE_CUBE_ARRAY:
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
            This->desc.TextureCubeArray.MipLevels = res->last_level + 1;
            This->desc.TextureCubeArray.NumCubes = res->array_size / 6;
            break;
        case PIPE_BUFFER:
            assert(util_format_get_blockwidth(res->format) == 1);
            This->desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            This->desc.Buffer.NumElements =
                res->width0 / util_format_get_blocksize(res->format);
            break;
        default:
            assert(0);
            return_error(D3DERR_DRIVERINTERNALERROR);
        }
    }
    pDesc = &This->desc;

    hr = D3D11ShaderResourceView_AdjustMipLevels(This, res->last_level + 1);
    if (FAILED(hr))
        return_error(hr);

    memset(&templ, 0, sizeof(templ));

    switch (pDesc->ViewDimension) {
    case D3D11_SRV_DIMENSION_TEXTURE1D:
        templ.u.tex.first_level = pDesc->Texture1D.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->Texture1D.MipLevels - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURE2D:
        templ.u.tex.first_level = pDesc->Texture2D.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->Texture2D.MipLevels - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURE2DMS:
        break;
    case D3D11_SRV_DIMENSION_TEXTURE3D:
        templ.u.tex.first_level = pDesc->Texture3D.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->Texture3D.MipLevels - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
        templ.u.tex.first_level = pDesc->Texture1DArray.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->Texture1DArray.MipLevels - 1;
        templ.u.tex.first_layer = pDesc->Texture1DArray.FirstArraySlice;
        templ.u.tex.last_layer =
            templ.u.tex.first_layer + pDesc->Texture1DArray.ArraySize - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
        templ.u.tex.first_level = pDesc->Texture2DArray.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->Texture2DArray.MipLevels - 1;
        templ.u.tex.first_layer = pDesc->Texture2DArray.FirstArraySlice;
        templ.u.tex.last_layer =
            templ.u.tex.first_layer + pDesc->Texture2DArray.ArraySize - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
        templ.u.tex.first_layer = pDesc->Texture2DMSArray.FirstArraySlice;
        templ.u.tex.last_layer =
            templ.u.tex.first_layer + pDesc->Texture2DMSArray.ArraySize - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURECUBE:
        templ.u.tex.first_level = pDesc->TextureCube.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->TextureCube.MipLevels - 1;
        break;
    case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
        templ.u.tex.first_level = pDesc->TextureCubeArray.MostDetailedMip;
        templ.u.tex.last_level =
            templ.u.tex.first_level + pDesc->TextureCubeArray.MipLevels - 1;
        templ.u.tex.first_layer = pDesc->TextureCubeArray.First2DArrayFace * 6;
        templ.u.tex.last_layer =
            templ.u.tex.first_layer + pDesc->TextureCubeArray.NumCubes * 6;
        break;
    case D3D11_SRV_DIMENSION_BUFFER:
        templ.u.buf.first_element = pDesc->Buffer.FirstElement;
        templ.u.buf.last_element =
            templ.u.buf.first_element + pDesc->Buffer.NumElements - 1;
        break;
    default:
        return_error(E_INVALIDARG);
    }
    templ.format = dxgi_to_pipe_format(pDesc->Format);

    templ.swizzle_r = PIPE_SWIZZLE_RED;
    templ.swizzle_g = PIPE_SWIZZLE_GREEN;
    templ.swizzle_b = PIPE_SWIZZLE_BLUE;
    templ.swizzle_a = PIPE_SWIZZLE_ALPHA;

    This->sv = pipe->create_sampler_view(pipe, res, &templ);
    if (!This->sv)
        return_error(D3DERR_DRIVERINTERNALERROR);

    hr = D3D11View_ctor(&This->base, pParams, pResource);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11ShaderResourceView_dtor( struct D3D11ShaderResourceView *This )
{
    pipe_sampler_view_reference(&This->sv, NULL);
    D3D11View_dtor(&This->base);
}

void WINAPI
D3D11ShaderResourceView_GetDesc( struct D3D11ShaderResourceView *This,
                                 D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11ShaderResourceViewVtbl D3D11ShaderResourceView_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11View_GetResource,
    (void *)D3D11ShaderResourceView_GetDesc
};

static const GUID *D3D11ShaderResourceView_IIDs[] = {
    &IID_ID3D11ShaderResourceView,
    &IID_ID3D11View,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11ShaderResourceView_new( struct D3D11Device *pDevice,
                             struct D3D11Resource *pResource,
                             const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
                             struct D3D11ShaderResourceView **ppOut )
{
    D3D11_NEW(D3D11ShaderResourceView, ppOut, pDevice, pResource, pDesc);
}

