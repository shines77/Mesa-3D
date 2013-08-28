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

#include "blendstate.h"

HRESULT
D3D11BlendState_ctor( struct D3D11BlendState *This,
                      struct D3D11UnknownParams *pParams,
                      const D3D11_BLEND_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_blend_state so;
    HRESULT hr;
    unsigned i;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    if (!pDesc) {
        This->desc.AlphaToCoverageEnable = FALSE;
        This->desc.IndependentBlendEnable = FALSE;
        This->desc.RenderTarget[0].BlendEnable = FALSE;
        This->desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        This->desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        This->desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        This->desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        This->desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        This->desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        This->desc.RenderTarget[0].RenderTargetWriteMask = 0xf;
    } else {
        This->desc = *pDesc;
    }
    pDesc = &This->desc;

    memset(&so, 0, sizeof(so));

    so.alpha_to_coverage = !!pDesc->AlphaToCoverageEnable;

    for (i = 0; i < pDesc->IndependentBlendEnable ? 8 : 1; ++i) {
        const D3D11_RENDER_TARGET_BLEND_DESC *pRT = &pDesc->RenderTarget[i];

        so.rt[i].blend_enable = !!pRT->BlendEnable;
        if (so.rt[i].blend_enable) {
            so.rt[i].rgb_func = d3d11_to_pipe_blend_op(pRT->BlendOp);
            so.rt[i].rgb_src_factor = d3d11_to_pipe_blend_factor(pRT->SrcBlend);
            so.rt[i].rgb_dst_factor = d3d11_to_pipe_blend_factor(pRT->DestBlend);
            so.rt[i].alpha_func = d3d11_to_pipe_blend_op(pRT->BlendOpAlpha);
            so.rt[i].alpha_src_factor = d3d11_to_pipe_blend_factor(pRT->SrcBlendAlpha);
            so.rt[i].alpha_dst_factor = d3d11_to_pipe_blend_factor(pRT->DestBlendAlpha);
        }
        so.rt[i].colormask = pRT->RenderTargetWriteMask;
    }
    if (pDesc->IndependentBlendEnable) {
        for (i = 1; i < 8; ++i)
            if (memcmp(&so.rt[i], &so.rt[0], sizeof(so.rt[0]))
                break;
        so.independent_blend_enable = (i == 8);
        if (!so.independent_blend_enable)
            memset(&so.rt[1], 0, 7 * sizeof(so.rt[1]));
    }

    cso = pipe->create_blend_state(pipe, &so);
    if (!cso)
        return_error(D3DERR_DRIVERINTERNALERROR);

    util_hash_table_set(This->base.device->ht_bs, &This->desc, This);
    ++This->base.device->bs_count;

    return S_OK;
}

void
D3D11BlendState_dtor( struct D3D11BlendState *This )
{
    struct D3D11Device *dev = This->base.device;

    if (This->cso) {
        D3D11Device_Lock(dev);
        {
            util_hash_table_remove(dev->ht_bs, &This->desc);
            --dev->bs_count;
            dev->pipe->delete_blend_state(dev->pipe, This->cso);
        }
        D3D11Device_Unlock(dev);
    }
    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11BlendState_GetDesc( struct D3D11BlendState *This,
                         D3D11_BLEND_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11BlendStateVtbl D3D11BlendState_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11BlendState_GetDesc
};

static const GUID *D3D11BlendState_IIDs[] = {
    &IID_ID3D11BlendState,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11BlendState_new( struct D3D11Device *pDevice,
                     struct D3D11BlendState **ppOut, )
{
    D3D11_NEW(D3D11BlendState, ppOut, pDevice);
}


static int ht_BLEND_compare(void *key1, void *key2)
{
    return memcmp(key1, key2, sizeof(D3D11_BLEND_DESC));
}

static unsigned ht_BLEND_hash(void *key)
{
    const uint32_t *u32 = key;
    unsigned i;
    unsigned hash;
    hash = u32[0];
    for (i = 1; i < sizeof(D3D11_BLEND_DESC) / 4; ++i)
        hash ^= u32[i];
    return hash;
}

HRESULT
D3D11DeviceInit_BlendState( struct D3D11Device *This )
{
    This->ht_bs = util_hash_table_create(ht_BLEND_hash, ht_BLEND_compare);
    if (!This->ht_bs)
        return_error(E_OUTOFMEMORY);

    return D3D11BlendState_new(This, NULL, &This->bs_default);
}
