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

#include "samplerstate.h"

HRESULT
D3D11SamplerState_ctor( struct D3D11SamplerState *This,
                        const D3D11_SAMPLER_DESC *pDesc,
                        struct D3D11UnknownParams *pParams )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_sampler_state so;
    HRESULT hr;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    if (!pDesc) {
        This->desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        This->desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        This->desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        This->desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        This->desc.MinLOD = -FLT_MAX;
        This->desc.MaxLOD = +FLT_MAX;
     /* This->desc.MipLODBias = 0.0f; */
        This->desc.MaxAnisotropy = 1;
        This->desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        This->desc.BorderColor[0] = 1.0f;
        This->desc.BorderColor[1] = 1.0f;
        This->desc.BorderColor[2] = 1.0f;
        This->desc.BorderColor[3] = 1.0f;
    } else {
        This->desc = *pDesc;
    }
    pDesc = This->desc;

    memset(&so, 0, sizeof(so));

    so.wrap_s = d3d11_to_pipe_tex_address_mode(pDesc->AddressU);
    so.wrap_t = d3d11_to_pipe_tex_address_mode(pDesc->AddressV);
    so.wrap_r = d3d11_to_pipe_tex_address_mode(pDesc->AddressW);

    so.min_img_filter = (pDesc->Filter & 0x10) ? PIPE_TEX_FILTER_LINEAR : PIPE_TEX_FILTER_NEAREST;
    so.mag_img_filter = (pDesc->Filter & 0x04) ? PIPE_TEX_FILTER_LINEAR : PIPE_TEX_FILTER_NEAREST;
    so.min_mip_filter = (pDesc->Filter & 0x01) ? PIPE_TEX_MIPFILTER_LINEAR : PIPE_TEX_MIPFILTER_NEAREST;

    so.max_anisotropy = (pDesc->Filter & 0x40) ? pDesc->MaxAnisotropy : 0;

    so.compare_mode = (Filter & 0x80) ? PIPE_TEX_COMPARE_R_TO_TEXTURE : PIPE_TEX_COMPARE_NONE;
    if (so.compare_mode != PIPE_TEX_COMPARE_NONE)
        so.compare_func = d3d11_to_pipe_func(pDesc->ComparisonFunc);
    else
        so.compare_func = PIPE_FUNC_NEVER;

    so.lod_bias = pDesc->MipLODBias;
    so.min_lod = pDesc->MinLOD;
    so.max_lod = pDesc->MaxLOD;
    if (pDesc->MaxLOD == 0.0f)
        so.min_mip_filter = PIPE_TEX_MIPFILTER_NONE;

    if (pDesc->AddressU == D3D11_TEXTURE_ADDRESS_BORDER ||
        pDesc->AddressV == D3D11_TEXTURE_ADDRESS_BORDER ||
        pDesc->AddressW == D3D11_TEXTURE_ADDRESS_BORDER) {
        so.border_color.f[0] = pDesc->BorderColor[0];
        so.border_color.f[1] = pDesc->BorderColor[1];
        so.border_color.f[2] = pDesc->BorderColor[2];
        so.border_color.f[3] = pDesc->BorderColor[3];
    }

    so.normalized_coords = 1;
    so.seamless_cube_map = 1;

    cso = pipe->create_sampler_state(pipe, &so);
    if (!cso)
        return_error(D3DERR_DRIVERINTERNALERROR);

    util_hash_table_set(This->base.device->ht_ss, &This->desc, This);
    ++This->base.device->ss_count;

    return S_OK;
}

void
D3D11SamplerState_dtor( struct D3D11SamplerState *This )
{
    struct D3D11Device *dev = This->base.device;

    if (This->cso) {
        D3D11Device_Lock(dev);
        {
            util_hash_table_remove(dev->ht_ss, &This->desc);
            --dev->ss_count;
            dev->pipe->delete_sampler_state(dev->pipe, This->cso);
        }
        D3D11Device_Unlock(dev);
    }
    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11SamplerState_GetDesc( struct D3D11SamplerState *This,
                           D3D11_SAMPLER_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11SamplerStateVtbl D3D11SamplerState_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11SamplerState_GetDesc
};

static const GUID *D3D11SamplerState_IIDs[] = {
    &IID_ID3D11SamplerState,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11SamplerState_new( struct D3D11Device *pDevice,
                       const D3D11_SAMPLER_DESC *pDesc,
                       struct D3D11SamplerState **ppOut )
{
    D3D11_NEW(D3D11SamplerState, ppOut, pDevice, pDesc);
}


static int ht_SS_compare(void *key1, void *key2)
{
    return memcmp(key1, key2, sizeof(D3D11_SAMPLER_DESC));
}

static unsigned ht_SS_hash(void *key)
{
    const uint32_t *u32 = key;
    unsigned i;
    unsigned hash;
    hash = u32[0];
    for (i = 1; i < sizeof(D3D11_SAMPLER_DESC) / 4; ++i)
        hash ^= u32[i];
    return hash;
}

HRESULT
D3D11DeviceInit_SamplerState( struct D3D11Device *This )
{
    This->ht_ss = util_hash_table_create(ht_SS_hash, ht_SS_compare);
    if (!This->ht_ss)
        return_error(E_OUTOFMEMORY);

    return D3D11SamplerState_new(This, NULL, &This->ss_default);
}
