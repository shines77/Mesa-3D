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

#include "depthstencilstate.h"

HRESULT
D3D11DepthStencilState_ctor( struct D3D11DepthStencilState *This,
                             struct D3D11UnknownParams *pParams,
                             const D3D11_DEPTH_STENCIL_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_depth_stencil_alpha_state dsa;
    HRESULT hr;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    if (!pDesc) {
        This->desc.DepthEnable = TRUE;
        This->desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        This->desc.DepthFunc = D3D11_COMPARISON_LESS;
        This->desc.StencilEnable = FALSE;
        This->desc.StencilReadMask = 0xff;
        This->desc.StencilWriteMask = 0xff;
        This->desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        This->desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        This->desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        This->desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        This->desc.BackFace = This->desc.FrontFace;
    } else {
        This->desc = *pDesc;
    }
    pDesc = &This->desc;

    memset(&dsa, 0, sizeof(dsa));

    dsa.depth.enabled = !!pDesc->DepthEnable;
    dsa.depth.writemask = pDesc->DepthWriteMask;
    dsa.depth.func = d3d11_to_pipe_func(pDesc->DepthFunc);

    if (pDesc->StencilEnable) {
        dsa.stencil[0].enabled = 1;
        dsa.stencil[0].valuemask = pDesc->StencilReadMask;
        dsa.stencil[0].writemask = pDesc->StencilWriteMask;
        dsa.stencil[0].func = d3d11_to_pipe_func(pDesc->FrontFace.StencilFunc);
        dsa.stencil[0].fail_op = d3d11_to_pipe_stencil_op(pDesc->FrontFace.StencilFailOp);
        dsa.stencil[0].zfail_op = d3d11_to_pipe_stencil_op(pDesc->FrontFace.StencilDepthFailOp);
        dsa.stencil[0].zpass_op = d3d11_to_pipe_stencil_op(pDesc->FrontFace.StencilPassOp);

        if (memcmp(&pDesc->FrontFace, &pDesc->BackFace, sizeof(pDesc->FrontFace))) {
            dsa.stencil[1].enabled = 1;
            dsa.stencil[1].valuemask = pDesc->StencilReadMask;
            dsa.stencil[1].writemask = pDesc->StencilWriteMask;
            dsa.stencil[1].func = d3d11_to_pipe_func(pDesc->BackFace.StencilFunc);
            dsa.stencil[1].fail_op = d3d11_to_pipe_stencil_op(pDesc->BackFace.StencilFailOp);
            dsa.stencil[1].zfail_op = d3d11_to_pipe_stencil_op(pDesc->BackFace.StencilDepthFailOp);
            dsa.stencil[1].zpass_op = d3d11_to_pipe_stencil_op(pDesc->BackFace.StencilPassOp);
        }
    }

    This->cso = pipe->create_depth_stencil_alpha_state(pipe, &dsa);
    if (!This->cso)
        return_error(D3DERR_DRIVERINTERNALERROR);

    util_hash_table_set(This->base.device->ht_ds, &This->desc, This);
    ++This->base.device->ds_count;

    return S_OK;
}

void
D3D11DepthStencilState_dtor( struct D3D11DepthStencilState *This )
{
    struct D3D11Device *dev = This->base.device;

    if (This->cso) {
        D3D11Device_Lock(dev);
        {
            util_hash_table_remove(dev->ht_ds, &This->desc);
            --dev->ds_count;
            dev->pipe->delete_depth_stencil_alpha_state(dev->pipe, This->cso);
        }
        D3D11Device_Unlock(dev);
    }

    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11DepthStencilState_GetDesc( struct D3D11DepthStencilState *This,
                                D3D11_DEPTH_STENCIL_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11DepthStencilStateVtbl D3D11DepthStencilState_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11DepthStencilState_GetDesc
};

static const GUID *D3D11DepthStencilState_IIDs[] = {
    &IID_ID3D11DepthStencilState,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11DepthStencilState_new( struct D3D11Device *pDevice,
                            const D3D11_DEPTH_STENCIL_DESC *pDesc,
                            struct D3D11DepthStencilState **ppOut )
{
    D3D11_NEW(D3D11DepthStencilState, ppOut, pDevice, pDesc);
}


static int ht_DSS_compare(void *key1, void *key2)
{
    return memcmp(key1, key2, sizeof(D3D11_DEPTH_STENCIL_DESC));
}

static unsigned ht_DSS_hash(void *key)
{
    const uint32_t *u32 = key;
    unsigned i;
    unsigned hash;
    hash = u32[0];
    for (i = 1; i < sizeof(D3D11_DEPTH_STENCIL_DESC) / 4; ++i)
        hash ^= u32[i];
    return hash;
}

static HRESULT
D3D11DeviceInit_DepthStencilState( struct D3D11Device *This )
{
    This->ht_ds = util_hash_table_create(ht_DSS_hash, ht_DSS_compare);
    if (!This->ht_ds)
        return_error(E_OUTOFMEMORY);

    return D3D11DepthStencilState_new(This, NULL, &This->ds_default);
}
