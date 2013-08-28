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

#include "rasterizerstate.h"

HRESULT
D3D11RasterizerState_ctor( struct D3D11RasterizerState *This,
                           struct D3D11UnknownParams *pParams,
                           const D3D11_RASTERIZER_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    struct pipe_rasterizer_state rast;
    HRESULT hr;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    if (!pDesc) {
        This->desc.FillMode = D3D11_FILL_SOLID;
        This->desc.CullMode = D3D11_CULL_BACK;
        This->desc.FrontCounterClockwise = FALSE;
        This->desc.DepthBias = 0.0f;
        This->desc.SlopeScaledDepthBias = 0.0f;
        This->desc.DepthBiasClamp = 0.0f;
        This->desc.DepthClipEnable = TRUE;
        This->desc.ScissorEnable = FALSE;
        This->desc.MultisampleEnable = FALSE;
        This->desc.AntialiasedLineEnable = FALSE;
    } else {
        This->desc = *pDesc;
    }
    pDesc = &This->desc;

    memset(&rast, 0, sizeof(rast));

    rast.front_ccw = !!pDesc->FrontCounterClockwise;
    rast.cull_face = d3d11_to_pipe_cull_mode(pDesc->CullMode);
    rast.fill_front = d3d11_to_pipe_polygon_mode(pDesc->FillMode);
    rast.fill_back = rast.fill_front;
    rast.offset_tri = pDesc->DepthBias || pDesc->SlopeScaledDepthBias;
    rast.offset_line = pDesc->offset_tri;
    rast.offset_units = pDesc->DepthBias;
    rast.offset_scale = pDesc->SlopeScaledDepthBias;
    rast.offset_clamp = pDesc->DepthBiasClamp;
    rast.scissor = !!pDesc->ScissorEnable;
    rast.sprite_coord_mode = PIPE_SPRITE_COORD_UPPER_LEFT;
    rast.point_quad_rasterization = 0; /* XXX */
    rast.point_size_per_vertex = 1; /* XXX */
    rast.point_size = 1.0f;
    rast.line_width = 1.0f;
    rast.multisample = !!pDesc->MultisampleEnable;
    rast.line_smooth = !!pDesc->AntialiasedLineEnable;
    rast.flatshade_first = 1;
    rast.half_pixel_center = 1;
    rast.depth_clip = !!pDesc->DepthClipEnable;
    rast.clip_halfz = 1;
    rast.clip_plane_enable = (1 << PIPE_MAX_CLIP_PLANES) - 1;

    This->cso = pipe->create_rasterizer_state(pipe, &rast);
    if (!This->cso)
        return_error(D3DERR_DRIVERINTERNALERROR);

    util_hash_table_set(This->base.device->ht_rs, &This->desc, This);
    ++This->base.device->rs_count;

    return S_OK;
}

void
D3D11RasterizerState_dtor( struct D3D11RasterizerState *This )
{
    struct D3D11Device *dev = This->base.device;

    if (This->cso) {        
        D3D11Device_Lock(dev);
        {
            util_hash_table_remove(dev->ht_rs, &This->desc);
            --dev->rs_count;
            dev->pipe->delete_rasterizer_state(dev->pipe, This->cso);
        }
        D3D11DeviceUnlock(dev);
    }
    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11RasterizerState_GetDesc( struct D3D11RasterizerState *This,
                              D3D11_RASTERIZER_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11RasterizerStateVtbl D3D11RasterizerState_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11RasterizerState_GetDesc
};

static const GUID *D3D11RasterizerState_IIDs[] = {
    &IID_ID3D11RasterizerState,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11RasterizerState_new( struct D3D11Device *pDevice,
                          const D3D11_RASTERIZER_DESC *pDesc,
                          struct D3D11RasterizerState **ppOut )
{
    D3D11_NEW(D3D11RasterizerState, ppOut, pDevice);
}


static int ht_RS_compare(void *key1, void *key2)
{
    return memcmp(key1, key2, sizeof(D3D11_RASTERIZER_DESC));
}

static unsigned ht_RS_hash(void *key)
{
    const uint32_t *u32 = key;
    unsigned i;
    unsigned hash;
    hash = u32[0];
    for (i = 1; i < sizeof(D3D11_RASTERIZER_DESC) / 4; ++i)
        hash ^= u32[i];
    return hash;
}

HRESULT
D3D11DeviceInit_RasterizerState( struct D3D11Device *This )
{
    This->ht_rs = util_hash_table_create(ht_RS_hash, ht_RS_compare);
    if (!This->ht_rs)
        return_error(E_OUTOFMEMORY);

    return D3D11RasterizerState_new(This, NULL, &This->rs_default);
}

