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

#include "devicecontext.h"

HRESULT
D3D11DeviceContext_ctor( struct D3D11DeviceContext *This,
struct D3D11UnknownParams *pParams)
{
    unsigned i;
    HRESULT hr;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    for (i = 0; i < Elements(This->ia.vport); ++i)
        This->ia.vport[i].scale[3] = 1.0f;

    return S_OK;
}

void
D3D11DeviceContext_dtor( struct D3D11DeviceContext *This )
{
    D3D11DeviceChild_dtor(&This->base);
}


#define IMPLEMENT_SETCGETONSTANTBUFFERS(n, s) \
void WINAPI \
D3D11DeviceContext_##n##SetConstantBuffers( struct D3D11DeviceContext *This, \
                                            UINT StartSlot, \
                                            UINT NumBuffers, \
                                            ID3D11Buffer **ppConstantBuffers ) \
{ \
    struct pipe_context *pipe = This->pipe; \
    unsigned i, k; \
 \
    for (i = 0; i < NumBuffers; ++i) { \
        struct D3D11Buffer *buf = D3D11Buffer(ppConstantBuffers[i]); \
        k = StartSlot + i; \
 \
        com_ref(&This->cb[s][k].buffer, buf); \
        if (buf) { \
            This->cb[s][k].cb.buffer = buf->resource; \
            This->cb[s][k].cb.buffer_offset = 0; \
            This->cb[s][k].cb.buffer_size = buf->desc.ByteWidth; \
        } \
        pipe->set_constant_buffer(pipe, s, &This->cb[s][k]); \
    } \
} \
void WINAPI \
D3D11DeviceContext_##n##GetConstantBuffers( struct D3D11DeviceContext *This, \
                                            UINT StartSlot, \
                                            UINT NumBuffers, \
                                            ID3D11Buffer **ppConstantBuffers ) \
{ \
    unsigned i, b; \
 \
    for (i = 0; i < NumBuffers; ++i) { \
        b = StartSlot + i; \
        com_set(&ppConstantBuffers[i], This->cb[s][b].buffer); \
    } \
}
IMPLEMENT_SETGETCONSTANTBUFFERS(VS, PIPE_SHADER_VERTEX);
IMPLEMENT_SETGETCONSTANTBUFFERS(HS, PIPE_SHADER_HULL);
IMPLEMENT_SETGETCONSTANTBUFFERS(DS, PIPE_SHADER_DOMAIN);
IMPLEMENT_SETGETCONSTANTBUFFERS(GS, PIPE_SHADER_GEOMETRY);
IMPLEMENT_SETGETCONSTANTBUFFERS(PS, PIPE_SHADER_FRAGMENT);
IMPLEMENT_SETGETCONSTANTBUFFERS(CS, PIPE_SHADER_COMPUTE);


#define IMPLEMENT_SETGETSHADER(n, sp, iface, pt) \
void WINAPI \
D3D11DeviceContext_##n##SetShader( struct D3D11DeviceContext *This, \
                                   ID3D11##iface##Shader *pShader, \
                                   ID3D11ClassInstance **ppClassInstances, \
                                   UINT NumClassInstances ) \
{ \
    com_ref(&This->sp, pShader); \
    if (This->sp != pShader) \
        This->pipe->bind_##pt##_state(This->pipe, This->sp->cso); \
 \
    if (!NumClassInstances) \
        return; \
    if (NumClassInstances > 256) \
        ERROR("more than 256 class instances\n"); \
    WARN("ignoring unexpected class instances\n"); \
} \
void WINAPI \
D3D11DeviceContext_##n##GetShader( struct D3D11DeviceContext *This, \
                                   ID3D11##iface##Shader **ppShader, \
                                   ID3D11ClassInstance **ppClassInstances, \
                                   UINT *pNumClassInstances ) \
{ \
    if (ppShader) \
        com_set(ppShader, This->sp); \
    if (ppClassInstances) \
        *pNumClassInstances = 0; /* TODO */ \
}
IMPLEMENT_SETGETSHADER(VS, vs, Vertex, PIPE_SHADER_VERTEX);
IMPLEMENT_SETGETSHADER(HS, hs, Hull, PIPE_SHADER_HULL);
IMPLEMENT_SETGETSHADER(DS, ds, Domain, PIPE_SHADER_DOMAIN);
IMPLEMENT_SETGETSHADER(GS, gs, Geometry, PIPE_SHADER_GEOMETRY);
IMPLEMENT_SETGETSHADER(PS, ps, Pixel, PIPE_SHADER_FRAGMENT);


#define IMPLEMENT_SETGETSAMPLERS(n, t, s) \
void WINAPI \
D3D11DeviceContext_##n##SetSamplers( struct D3D11DeviceContext *This, \
                                     UINT StartSlot, \
                                     UINT NumSamplers, \
                                     ID3D11SamplerState **ppSamplers ) \
{ \
    void *cso[PIPE_MAX_SAMPLERS]; \
    unsigned i, k; \
    struct pipe_context *pipe = This->pipe; \
 \
    assert(ppSamplers); \
 \
    for (i = 0; i < NumSamplers; ++i) { \
        k = StartSlot + i; \
        if (ppSamplers[i]) \
            com_ref(&This->ss[s][k], ppSamplers[i]); \
        else \
            com_ref(&This->ss[s][k], This->base.device->ss_default); \
        cso[i] = This->ss[s][k]->cso; \
    } \
    pipe->set_##t##_samplers(pipe, StartSlot, NumSamplers, cso); \
} \
void WINAPI \
D3D11DeviceContext_##n##GetSamplers( struct D3D11DeviceContext *This, \
                                     UINT StartSlot, \
                                     UINT NumSamplers, \
                                     ID3D11SamplerState **ppSamplers ) \
{ \
    unsigned i, k; \
 \
    assert(ppSamplers); \
    for (i = 0; i < NumSamplers; ++i) { \
        k = StartSlot + i; \
        com_set(&ppSamplers[i], This->ss[s][k]->cso); \
    } \
}
IMPLEMENT_SETGETSAMPLERS(VS, vertex, PIPE_SHADER_VERTEX);
IMPLEMENT_SETGETSAMPLERS(HS, hull, PIPE_SHADER_HULL);
IMPLEMENT_SETGETSAMPLERS(DS, domain, PIPE_SHADER_DOMAIN);
IMPLEMENT_SETGETSAMPLERS(GS, geometry, PIPE_SHADER_GEOMETRY);
IMPLEMENT_SETGETSAMPLERS(PS, fragment, PIPE_SHADER_FRAGMENT);
IMPLEMENT_SETGETSAMPLERS(CS, compute, PIPE_SHADER_COMPUTE);


#define IMPLEMENT_SETGETSHADERRESOURCES(n, t, s) \
void WINAPI \
D3D11DeviceContext_##n##SetShaderResources( struct D3D11DeviceContext *This, \
                                            UINT StartSlot, \
                                            UINT NumViews, \
                                            ID3D11ShaderResourceView **ppShaderResourceViews ) \
{ \
    struct pipe_sampler_view *cso[PIPE_MAX_SHADER_SAMPLER_VIEWS]; \
    unsigned i, k; \
    struct pipe_context *pipe = This->pipe; \
 \
    for (i = 0; i < NumViews; ++i) { \
        k = StartSlot + i; \
        com_ref(&This->srv[s][k], ppShaderResourceViews[i]); \
        cso[i] = This->srv[s][k] ? \
            This->srv[s][k]->sv : NULL; \
    } \
    pipe->set_##t##_sampler_views(pipe, StartSlot, NumViews, cso); \
} \
void WINAPI \
D3D11DeviceContext_##n##GetShaderResources( struct D3D11DeviceContext *This, \
                                            UINT StartSlot, \
                                            UINT NumViews, \
                                            ID3D11ShaderResourceView **ppShaderResourceViews ) \
{ \
    unsigned i, k; \
 \
    assert(ppShaderResourceViews || NumViews == 0); \
    for (i = 0; i < NumViews; ++i) { \
        k = StartSlot + i; \
        com_set(&ppShaderResourceViews[i], This->srv[s][k]); \
    } \
}
IMPLEMENT_SETGETSHADERRESOURCES(VS, vertex, PIPE_SHADER_VERTEX);
IMPLEMENT_SETGETSHADERRESOURCES(HS, hull, PIPE_SHADER_HULL);
IMPLEMENT_SETGETSHADERRESOURCES(DS, domain, PIPE_SHADER_DOMAIN);
IMPLEMENT_SETGETSHADERRESOURCES(GS, geometry, PIPE_SHADER_GEOMETRY);
IMPLEMENT_SETGETSHADERRESOURCES(PS, fragment, PIPE_SHADER_FRAGMENT);
IMPLEMENT_SETGETSHADERRESOURCES(CS, compute, PIPE_SHADER_COMPUTE);


void WINAPI
D3D11DeviceContext_DrawIndexed( struct D3D11DeviceContext *This,
                                UINT IndexCount,
                                UINT StartIndexLocation,
                                INT BaseVertexLocation )
{
    struct pipe_draw_info *info = &This->ia.draw;

    info->indexed = TRUE;
    info->start = StartIndexLocation;
    info->count = IndexCount;
    info->start_instance = 0;
    info->instance_count = 1;
    info->index_bias = BaseVertexLocation;
    info->primitive_restart = TRUE;

    This->pipe->draw(This->pipe, info);
}

void WINAPI
D3D11DeviceContext_Draw( struct D3D11DeviceContext *This,
                         UINT VertexCount,
                         UINT StartVertexLocation )
{
    struct pipe_draw_info *info = &This->ia.draw;

    info->indexed = FALSE;
    info->start = StartVertexLocation;
    info->count = VertexCount;
    info->start_instance = 0;
    info->instance_count = 1;
    info->index_bias = 0;
    info->primitive_restart = FALSE;

    This->pipe->draw(This->pipe, info);
}

void WINAPI
D3D11DeviceContext_DrawIndexedInstanced( struct D3D11DeviceContext *This,
                                         UINT IndexCountPerInstance,
                                         UINT InstanceCount,
                                         UINT StartIndexLocation,
                                         INT BaseVertexLocation,
                                         UINT StartInstanceLocation )
{
    struct pipe_draw_info *info = &This->ia.draw;

    info->indexed = TRUE;
    info->start = StartIndexLocation;
    info->count = IndexCountPerInstance;
    info->start_instance = StartInstanceLocation;
    info->instance_count = InstanceCount;
    info->index_bias = BaseVertexLocation;
    info->primitive_restart = TRUE;

    This->pipe->draw(This->pipe, info);
}

void WINAPI
D3D11DeviceContext_DrawInstanced( struct D3D11DeviceContext *This,
                                  UINT VertexCountPerInstance,
                                  UINT InstanceCount,
                                  UINT StartVertexLocation,
                                  UINT StartInstanceLocation )
{
    struct pipe_draw_info *info = &This->ia.draw;

    info->indexed = FALSE;
    info->start = StartVertexLocation;
    info->count = VertexCountPerInstance;
    info->start_instance = StartInstanceLocation;
    info->instance_count = InstanceCount;
    info->index_bias = 0;
    info->primitive_restart = FALSE;

    This->pipe->draw(This->pipe, info);
}

void WINAPI
D3D11DeviceContext_DrawAuto( struct D3D11DeviceContext *This )
{
    struct pipe_draw_info info;

    assert(This->ia.buffer[0] && This->ia.buffer[0]->so_target);

    memset(&info, 0, sizeof(info));
    info.mode = This->ia.draw.mode;
    info.instance_count = 1;
    info.max_index = ~0;
    info.count_from_stream_output = This->ia.buffer[0]->so_target;

    This->pipe->draw(This->pipe, &info);
}

void WINAPI
D3D11DeviceContext_DrawIndexedInstancedIndirect( struct D3D11DeviceContext *This,
                                                 ID3D11Buffer *pBufferForArgs,
                                                 UINT AlignedByteOffsetForArgs )
{
    struct D3D11Buffer *buf = D3D11Buffer(pBufferForArgs);
    struct pipe_draw_info info;

    memset(&info, 0, sizeof(info));
    info.indexed = TRUE;
    info.indirect = buf->resource;
    info.indirect_offset = AlignedByteOffsetForArgs;

    This->pipe->draw(This->pipe, &info);
}

void WINAPI
D3D11DeviceContext_DrawInstancedIndirect( struct D3D11DeviceContext *This,
                                          ID3D11Buffer *pBufferForArgs,
                                          UINT AlignedByteOffsetForArgs )
{
    struct D3D11Buffer *buf = D3D11Buffer(pBufferForArgs);
    struct pipe_draw_info info;

    memset(&info, 0, sizeof(info));
    info.indirect = buf->resource;
    info.indirect_offset = AlignedByteOffsetForArgs;

    This->pipe->draw(This->pipe, &info);
}

HRESULT WINAPI
D3D11DeviceContext_Map( struct D3D11DeviceContext *This,
                        ID3D11Resource *pResource,
                        UINT Subresource,
                        D3D11_MAP MapType,
                        UINT MapFlags,
                        D3D11_MAPPED_SUBRESOURCE *pMappedResource )
{
    struct pipe_context *pipe = This->pipe;
    struct D3D11Resource *res = D3D11Resource(pResource);
    struct pipe_box box;
    unsigned usage;

    user_assert(Subresource < res->num_sub, E_INVALIDARG);

    user_assert(!res->sub[Subresource].transfer, D3DERR_INVALIDCALL);

    box.x = 0;
    box.y = 0;
    box.z = res->sub[Subresource].layer;
    box.width = u_minify(res->resource->width0, res->sub[Subresource].level);
    box.height = u_minify(res->resource->height0, res->sub[Subresource].level);
    box.depth = u_minify(res->resource->depth0, res->sub[Subresource].level);

    switch (MapType) {
    case D3D11_MAP_READ: usage = PIPE_TRANSFER_READ; break;
    case D3D11_MAP_WRITE: usage = PIPE_TRANSFER_WRITE; break;
    case D3D11_MAP_READ_WRITE: usage = PIPE_TRANSFER_READ_WRITE; break;
    case D3D11_MAP_WRITE_DISCARD: usage = PIPE_TRANSFER_WRITE | PIPE_TRANSFER_DISCARD_WHOLE_RESOURCE ; break;
    case D3D11_MAP_WRITE_NO_OVERWRITE: usage = PIPE_TRANSFER_WRITE | PIPE_TRANSFER_UNSYNCHRONIZED; break;
    default:
        return_error(E_INVALIDARG);
        break;
    }
    if (MapFlags & D3D11_MAP_FLAG_DO_NOT_WAIT)
        usage |= PIPE_TRANSFER_DONTBLOCK;

    assert(pMappedResource);
    pMappedResource->pData = pipe->transfer_map(pipe, res,
                                                res->sub[Subresource].level,
                                                usage,
                                                &box,
                                                &res->sub[Subresource].transfer);
    if (!pMappedResource->pData) {
        if (usage & PIPE_TRANSFER_DONTBLOCK)
            return_error(DXGI_ERROR_WAS_STILL_DRAWING);
        return_error(E_FAIL);
    }
    pMappedResource->RowPitch = res->sub[Subresource].transfer->stride;
    pMappedResource->DepthPitch = res->sub[Subresource].transfer->layer_stride;

    return S_OK;
}

void WINAPI
D3D11DeviceContext_Unmap( struct D3D11DeviceContext *This,
                          ID3D11Resource *pResource,
                          UINT Subresource )
{
    struct D3D11Resource *res = D3D11Resource(pResource);

    if (Subresource >= res->num_sub)
        return;
    if (!res->sub[Subresource].transfer)
        return;
    This->pipe->transfer_unmap(This->pipe, res->sub[Subresource].transfer);
}

void WINAPI
D3D11DeviceContext_IASetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout *pInputLayout )
{
    com_ref(&This->ia.so, pInputLayout);
    if (This->ia.so)
        This->pipe->bind_vertex_elements_state(This->pipe, This->ia.so->cso);
}

void WINAPI
D3D11DeviceContext_IAGetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout **ppInputLayout )
{
    assert(ppInputLayout);
    com_set(ppInputLayout, This->ia.so);
}

void WINAPI
D3D11DeviceContext_IASetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets )
{
    struct pipe_context *pipe = This->pipe;
    unsigned i, b;

    for (i = 0; i < NumBuffers; ++i) {
        struct D3D11Buffer *buf = D3D11Buffer(ppVertexBuffers[i]);
        b = StartSlot + i;

        com_ref(&This->ia.buffer[b], buf);
        if (buf) {
            This->ia.vtxbuf[b].buffer = buf->resource;
            This->ia.vtxbuf[b].buffer_offset = pOffsets[i];
            This->ia.vtxbuf[b].stride = pStrides[i];
        } else {
            This->ia.vtxbuf[b].buffer = NULL;
        }
        This->pipe->set_vertex_buffers(
            This->pipe, StartSlot, NumBuffers, &This->ia.vtxbuf[StartSlot]);
    }
}

void WINAPI
D3D11DeviceContext_IAGetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets )
{
    unsigned i;

    for (i = 0; i < NumBuffers; ++i) {
        const unsigned b = StartSlot + i;

        if (ppVertexBuffers)
            com_set(&ppVertexBuffers[i], This->ia.buffer[b]);
        if (pStrides)
            pStrides[i] = This->ia.vtxbuf[b].stride;
        if (pOffsets)
            pOffsets[i] = This->ia.vtxbuf[b].buffer_offset;
        
    }
}

void WINAPI
D3D11DeviceContext_IASetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer *pIndexBuffer,
                                     Int Format,
                                     UINT Offset )
{
    struct D3D11Buffer *buf = D3D11Buffer(pIndexBuffer);

    com_ref(&This->ia.buffer[PIPE_MAX_ATTRIBS], buf);
    if (buf) {
        switch (Format) {
        case DXGI_FORMAT_R8_UINT:
            This->ia.idxbuf.index_size = 1;
            This->ia.draw.restart_index = 0xff;
            break;
        case DXGI_FORMAT_R16_UINT:
            This->ia.idxbuf.index_size = 2;
            This->ia.draw.restart_index = 0xffff;
            break;
        default:
            assert(Format == DXGI_FORMAT_R32_UINT);
            This->ia.idxbuf.index_size = 4;
            This->ia.draw.restart_index = 0xffffffff;
            break;
        }
        This->ia.idxbuf.buffer = buf->resource;
        This->ia.idxbuf.offset = Offset;
    } else {
        This->ia.idxbuf.buffer = NULL;
    }
}

void WINAPI
D3D11DeviceContext_IAGetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer **pIndexBuffer,
                                     Int *Format,
                                     UINT *Offset )
{
    if (ppIndexBuffer)
        com_set(ppIndexBuffer, This->ia.buffers[PIPE_MAX_ATTRIBS]);
    if (Format) {
        switch (This->ia.idxbuf.index_size) {
        case 1: *Format = DXGI_FORMAT_R8_UINT; break;
        case 2: *Format = DXGI_FORMAT_R16_UINT; break;
        case 4: *Format = DXGI_FORMAT_R32_UINT; break;
        default:
            assert(0);
            break;
        }
    }
    if (Offset)
        *Offset = This->ia.idxbuf.offset;
}

void WINAPI
D3D11DeviceContext_IASetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY Topology )
{
    if (This->ia.topology == Topology)
        return;
    This->ia.topology = Topology;

    switch (Topology) {
    case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
        This->ia.draw._mode = PIPE_PRIM_POINTS;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
        This->ia.draw.mode = PIPE_PRIM_LINES;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
        This->ia.draw.mode = PIPE_PRIM_LINE_STRIP;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
        This->ia.draw.mode = PIPE_PRIM_TRIANGLES;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
        This->ia.draw.mode = PIPE_PRIM_TRIANGLE_STRIP;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
        This->ia.draw.mode = PIPE_PRIM_LINES_ADJACENCY;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
        This->ia.draw.mode = PIPE_PRIM_LINE_STRIP_ADJACENCY;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
        This->ia.draw.mode = PIPE_PRIM_TRIANGLES_ADJACENCY;
        break;
    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
        This->ia.draw.mode = PIPE_PRIM_TRIANGLE_STRIP_ADJACENCY;
        break;
    default:
        if (Topology >= D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST &&
            Topology <= D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST) {
            This->ia.draw.mode = PIPE_PRIM_PATCHES;
            This->ia.prim_size = Topology - D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + 1;
        } else {
            user_error("invalid primitive topology");
        }
    }
}

void WINAPI
D3D11DeviceContext_IAGetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY *pTopology )
{
    assert(pTopology);
    *pTopology = This->ia.topology;
}

void WINAPI
D3D11DeviceContext_OMSetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView *pDepthStencilView )
{
    struct pipe_framebuffer_state fb;
    unsigned i;

    fb.nr_cbufs = NumViews;

    for (i = 0; i < NumViews; ++i) {
        com_ref(&This->om.rtv[i], ppRenderTargetViews[i]);
        if (This->om.rtv[i])
            fb.cbufs[i] = This->om.rtv[i]->surface;
        else
            fb.cbufs[i] = NULL;
    }
    com_ref(This->om.dsv, pDepthStencilView);
    if (This->om.dsv)
        fb.zsbuf = This->om.dsv->surface;

    for (; i < This->om.num_rtvs)
        com_ref(&This->om.rtv[i], NULL);
    This->om.num_rtvs = NumViews;

    This->pipe->set_framebuffer_state(This->pipe, &fb);
}

void WINAPI
D3D11DeviceContext_OMGetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView **ppDepthStencilView )
{
    if (ppRenderTargetViews) {
        unsigned i;
        assert(NumViews < Elements(This->om.rtv));
        for (i = 0; i < NumViews; ++i)
            com_set(&ppRenderTargetViews[i], This->om.rtv[i]);
    }
    if (ppDepthStencilView)
        com_set(ppDepthStencilView, This->om.dsv);
}

void WINAPI
D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews( struct D3D11DeviceContext *This,
                                                              UINT NumRTVs,
                                                              ID3D11RenderTargetView **ppRenderTargetViews,
                                                              ID3D11DepthStencilView *pDepthStencilView,
                                                              UINT UAVStartSlot,
                                                              UINT NumUAVs,
                                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews,
                                                              UINT *pUAVInitialCounts )
{
    struct pipe_surface *surf[16];
    unsigned i, b;

    if (NumRTVs != D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL)
        D3D11DeviceContext_OMSetRenderTargets(This,
                                              NumRTVs, ppRenderTargetViews,
                                              pDepthStencilView);

    for (i = 0; i < NumUAVs; ++i) {
        b = UAVStartSlot + i;

        com_ref(&This->om.uav[b], ppUnorderedAccessViews[i]);
        if (This->om.uav[b])
            surf[i] = This->om.uav[b]->surface;
        else
            surf[i] = NULL;
    }
    This->pipe->set_shader_resources(This->pipe, UAVStartSlot, NumUAVs, surf);
}

void WINAPI
D3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews( struct D3D11DeviceContext *This,
                                                              UINT NumRTVs,
                                                              ID3D11RenderTargetView **ppRenderTargetViews,
                                                              ID3D11DepthStencilView **ppDepthStencilView,
                                                              UINT UAVStartSlot,
                                                              UINT NumUAVs,
                                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews )
{
    unsigned i, b;

    if (ppRenderTargetViews) {
        assert(NumRTVs < Elements(This->om.rtv));
        for (i = 0; i < NumRTVs; ++i)
            com_set(ppRenderTargetViews[i], This->om.rtv[i]);
    }
    if (ppDepthStencilView)
        com_set(ppDepthStencilView, This->om.dsv);

    if (ppUnorderedAccessViews) {
        for (i = 0; i < NumUAVs; ++i) {
            b = UAVStartSlot + i;
            com_set(&ppUnorderedAccesViews[i], This->om.uav[b]);
        }
    }
}

void WINAPI
D3D11DeviceContext_OMSetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState *pBlendState,
                                    ConstantArray BlendFactor,
                                    UINT SampleMask )
{
    struct pipe_context *pipe = This->pipe;

    if (This->om.bs != pBlendState) {
        com_ref(&This->om.bs, pBlendState);
        if (!This->om.bs)
            com_ref(&This->om.bs, This->base.device->bs_default);
        pipe->bind_blend_state(This->om.bs->cso);
    }
    if (This->om.sample_mask != SampleMask) {
        This->om.sample_mask = SampleMask;
        pipe->set_sample_mask(pipe, SampleMask);
    }
    if (memcmp(This->om.blend_color.color, BlendFactor, sizeof(BlendFactor))) {
        memcpy(&This->om.blend_color.color, BlendFactor, sizeof(BlendFactor));
        pipe->set_blend_color(pipe, &This->om.blend_color);
    }
}

void WINAPI
D3D11DeviceContext_OMGetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState **ppBlendState,
                                    ConstantArray BlendFactor,
                                    UINT *pSampleMask )
{
    if (ppBlendState)
        com_set(ppBlendState, This->om.bs);
    if (BlendFactor)
        memcpy(BlendFactor, This->om.blend_color.color, sizeof(BlendFactor));
    if (pSampleMask)
        *pSampleMask = This->om.sample_mask;
}

void WINAPI
D3D11DeviceContext_OMSetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState *pDepthStencilState,
                                           UINT StencilRef )
{
    struct pipe_context *pipe = This->pipe;

    if (This->om.ds != D3D11DepthStencilState(pDepthStencilState)) {
        com_ref(&This->om.ds, pDepthStencilState);
        if (!This->om.ds)
            com_ref(&This->om.ds, This->base.device->ds_default);
        pipe->bind_depth_stencil_alpha_state(pipe, This->om.ds->cso);
    }
    if (This->om.stencil_ref != StencilRef) {
        struct pipe_stencil_ref ref;
        ref.ref_value[0] = StencilRef;
        ref.ref_value[1] = StencilRef;
        pipe->set_stencil_ref(pipe, &ref);
    }
}

void WINAPI
D3D11DeviceContext_OMGetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState **ppDepthStencilState,
                                           UINT *pStencilRef )
{
    if (ppDepthStencilState)
        com_set(ppDepthStencilState, This->om.ds);
    if (pStencilRef)
        *pStencilRef = This->om.stencil_ref;
}

void WINAPI
D3D11DeviceContext_Dispatch( struct D3D11DeviceContext *This,
                             UINT ThreadGroupCountX,
                             UINT ThreadGroupCountY,
                             UINT ThreadGroupCountZ )
{
    struct pipe_context *pipe = This->pipe;
    uint grid_layout[3];

    grid_layout[0] = ThreadGroupCountX;
    grid_layout[1] = ThreadGroupCountY;
    grid_layout[2] = ThreadGroupCountZ;

    pipe->launch_grid(pipe, This->cs.group_size, grid_layout, NULL, NULL);
}

void WINAPI
D3D11DeviceContext_DispatchIndirect( struct D3D11DeviceContext *This,
                                     ID3D11Buffer *pBufferForArgs,
                                     UINT AlignedByteOffsetForArgs )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_RSSetState( struct D3D11DeviceContext *This,
                               ID3D11RasterizerState *pRasterizerState )
{
    com_ref(This->rs.so, pRasterizerState);
    This->dirty |= D3D11_NEW_RASTERIZER;
}

void WINAPI
D3D11DeviceContext_RSGetState( struct D3D11DeviceContext *This,
                               ID3D11RasterizerState **ppRasterizerState )
{
    assert(ppRasterizerState);
    com_set(ppRasterizerState, This->rs.so);
}

void WINAPI
D3D11DeviceContext_RSSetViewports( struct D3D11DeviceContext *This,
                                   UINT NumViewports,
                                   D3D11_VIEWPORT *pViewports )
{
    unsigned i;

    for (i = 0; i < NumViewports; ++i) {
        struct pipe_viewport_state *vp = &This->ia.vport[i];

        vp->scale[0] = pViewports[i].Width * 0.5f;
        vp->scale[1] = pViewports[i].Height * -0.5f;
        vp->scale[2] = pViewports[i].MaxDepth - pViewports[i].MinDepth;

        vp->translate[0] = pViewports[i].TopLeftX + vp->scale[0];
        vp->translate[1] = pViewports[i].TopLeftY + vp->scale[1];
        vp->translate[2] = pViewports[i].MinDepth;
    }
    This->rs.num_vports = NumViewports;

    This->pipe->set_viewport_state(This->pipe, 0, NumViewports, This->ia.vport);
}

void WINAPI
D3D11DeviceContext_RSGetViewports( struct D3D11DeviceContext *This,
                                   UINT *pNumViewports,
                                   D3D11_VIEWPORT *pViewports )
{
    user_assert(pNumViewports);

    if (pViewports) {
        unsigned n = MIN2(*pNumViewports, This->rs.num_vports);
        unsigned i;
        user_assert(*pNumViewports < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);
        for (i = 0; i < n; ++i) {
            const struct pipe_viewport_state *vp = &This->ia.vport[i];
            pViewports[i].Width = vp->scale[0] * 2.0f;
            pViewports[i].Height = vp->scale[1] * -2.0f;
            pViewports[i].TopLeftX = vp->translate[0] - vp->scale[0];
            pViewports[i].TopLeftY = vp->translate[1] - vp->scale[1];
            pViewports[i].MinDepth = vp->translate[2];
            pViewports[i].MaxDepth = vp->scale[2] + vp->translate[2];
        }
        if (i < *pNumViewports)
            memset(&pViewports[i], 0, (*pNumViewports - i) * sizeof(pViewports[0]));
    }
    *pNumViewports = This->rs.num_vports;
}

void WINAPI
D3D11DeviceContext_RSSetScissorRects( struct D3D11DeviceContext *This,
                                      UINT NumRects,
                                      D3D11_RECT *pRects )
{
    unsigned i;

    user_assert(NumRects < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);

    This->rs.num_scissors = NumRects;
    for (i = 0; i < NumRects; ++i) {
        This->rs.scissor[i].minx = pRects[i].left;
        This->rs.scissor[i].maxx = pRects[i].right;
        This->rs.scissor[i].miny = pRects[i].top;
        This->rs.scissor[i].maxy = pRects[i].bottom;
    }
    pipe->set_scissor_states(pipe, 0, NumRects, &This->rs.scissor[0]);
}

void WINAPI
D3D11DeviceContext_RSGetScissorRects( struct D3D11DeviceContext *This,
                                      UINT *pNumRects,
                                      D3D11_RECT *pRects )
{
    user_assert(pNumRects);

    if (pRects) {
        unisnged n = MIN2(*pNumRects, This->rs.num_scissors);
        unsigned i;
        user_assert(*pNumRects < D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);
        for (i = 0; i < n; ++i) {
            pRects[i].left = This->rs.scissor[i].minx;
            pRects[i].right = This->rs.scissor[i].maxx;
            pRects[i].top = This->rs.scissor[i].miny;
            pRects[i].bottom = This->rs.scissor[i].maxy;
        }
        if (i < *pNumRects)
            memset(&pRects[i], 0, (*pNumRects - i) * sizeof(pRects[0]));
    }
    *pNumRects = This->rs.num_scissors;
}

void WINAPI
D3D11DeviceContext_CopySubresourceRegion( struct D3D11DeviceContext *This,
                                          ID3D11Resource *pDstResource,
                                          UINT DstSubresource,
                                          UINT DstX,
                                          UINT DstY,
                                          UINT DstZ,
                                          ID3D11Resource *pSrcResource,
                                          UINT SrcSubresource,
                                          D3D11_BOX *pSrcBox )
{
    struct D3D11Resource *dst = D3D11Resource(pDstResource);
    struct D3D11Resource *src = D3D11Resource(pDstResource);
    struct pipe_context *pipe = This->pipe;
    unsigned dst_level;
    unsigned src_level;
    unsigned dst_w, dst_h, dst_d;
    unsigned src_w, src_h, src_d;
    struct pipe_box box;

    if (dst == src && DstSubresource == SrcSubresource)
        return;
    if (dst->resource->target == PIPE_BUFFER &&
        src->resource->target != PIPE_BUFFER)
        return;

    dst_level = d3d11_subresource_to_level(dst->resource, DstSubresource);
    src_level = d3d11_subresource_to_level(src->resource, SrcSubresource);

    dst_w = u_minify(dst->resource->width0, dst_level);
    dst_h = u_minify(dst->resource->height0, dst_level);
    dst_d = u_minify(dst->resource->depth0, dst_level);

    src_w = u_minify(src->resource->width0, src_level);
    src_h = u_minify(src->resource->height0, src_level);
    src_d = u_minify(src->resource->depth0, src_level);

    if (DstX >= dst_w || DstY >= dst_h || DstZ >= dst_d)
        return;
    if (pSrcBox->right < src_w ||
        pSrcBox->bottom < src_h ||
        pSrcBox->back < src_d)
        return;

    if (pSrcBox) {
        if (!d3d11_to_pipe_box(&box, pSrcBox))
            return;
    } else {
        box.x = 0;
        box.y = 0;
        box.z = 0;
        box.width = MIN2(dst_w - DstX, src_w);
        box.height = MIN2(dst_h - DstY, src_h);
        box.depth = MIN2(dst_d - DstZ, src_d);
    }
    if ((DstX + box.width < dst_w) ||
        (DstY + box.height < dst_h) ||
        (DstZ + box.depth < dst_d))
        return;

    pipe->resource_copy_region(pipe,
                               dst->resource, dst_level, DstX, DstY, DstZ,
                               src->resource, src_level, &box);
}

void WINAPI
D3D11DeviceContext_CopyResource( struct D3D11DeviceContext *This,
                                 ID3D11Resource *pDstResource,
                                 ID3D11Resource *pSrcResource )
{
    struct D3D11Resource *dst = D3D11Resource(pDstResource);
    struct D3D11Resource *src = D3D11Resource(pSrcResource);
    unsigned last_level = MIN2(dst->resource->last_level, src->resource->last_level);
    unsigned l;
    unsigned w = MIN2(dst->resource->width0, src->resource->width0);
    unsigned h = MIN2(dst->resource->height0, src->resource->height0);
    unsigned d = MIN2(dst->resource->depth0, src->resource->depth0);
    struct pipe_box box;
    box.x = 0;
    box.y = 0;
    box.z = 0;

    for (l = 0; l <= last_level; ++l) {
        box.width = u_minify(w, l);
        box.height = u_minify(h, l);
        box.depth = u_minify(d, l);
        pipe->resource_copy_region(pipe,
                                   dst->resource, l, 0, 0, 0,
                                   src->resource, l, &box);
    }
}

void WINAPI
D3D11DeviceContext_UpdateSubresource( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pDstResource,
                                      UINT DstSubresource,
                                      D3D11_BOX *pDstBox,
                                      void *pSrcData,
                                      UINT SrcRowPitch,
                                      UINT SrcDepthPitch )
{
    struct D3D11Resource *dst = D3D11Resource(pDstResource);
    struct pipe_box box;
    struct pipe_context *pipe = This->pipe;
    unsigned level;

    if (!d3d11_to_pipe_box(&box, pDstBox))
        return;
    level = d3d11_subresource_to_level(dst, DstSubresource);

    pipe->transfer_inline_write(pipe,
                                dst->resource, level, 0,
                                &box,
                                pSrcData, SrcRowPitch, SrcDepthPitch);
}

void WINAPI
D3D11DeviceContext_CopyStructureCount( struct D3D11DeviceContext *This,
                                       ID3D11Buffer *pDstBuffer,
                                       UINT DstAlignedByteOffset,
                                       ID3D11UnorderedAccessView *pSrcView )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_ClearRenderTargetView( struct D3D11DeviceContext *This,
                                          ID3D11RenderTargetView *pRenderTargetView,
                                          ConstantArray ColorRGBA )
{
    struct D3D11RenderTargetView *rtv = D3D11RenderTargetView(pRenderTargetView);

    This->pipe->clear_render_target(This->pipe, rtv->surface,
                                    (union pipe_color_union *)&ColorRGBA,
                                    0, 0,
                                    rtv->surface->width, rtv->surface->height);
}

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewUint( struct D3D11DeviceContext *This,
                                                 ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                 ConstantArray Values )
{
    struct D3D11UnorderedAccessView *uav = D3D11UnorderedAccessView(pUnorderedAccessView);

    This->pipe->clear_render_target(This->pipe, uav->surface,
                                    (union pipe_color_union *)&Values,
                                    0, 0,
                                    uav->surface->width, uav->surface->height);
}

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewFloat( struct D3D11DeviceContext *This,
                                                  ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                  ConstantArray Values )
{
    struct D3D11UnorderedAccessView *uav = D3D11UnorderedAccessView(pUnorderedAccessView);

    This->pipe->clear_render_target(This->pipe, uav->surface,
                                    (union pipe_color_union *)&Values,
                                    0, 0,
                                    uav->surface->width, uav->surface->height);
}

void WINAPI
D3D11DeviceContext_ClearDepthStencilView( struct D3D11DeviceContext *This,
                                          ID3D11DepthStencilView *pDepthStencilView,
                                          UINT ClearFlags,
                                          FLOAT Depth,
                                          Int Stencil )
{
    struct D3D11DepthStencilView *dsv = D3D11DepthStencilView(pDepthStencilView);
    unsigned bufs;

    bufs = 0;
    if (ClearFlags & D3D11_CLEAR_DEPTH)   bufs  = PIPE_CLEAR_DEPTH;
    if (ClearFlags & D3D11_CLEAR_STENCIL) bufs |= PIPE_CLEAR_STENCIL;

    This->pipe->clear_depth_stencil(This->pipe, dsv->surface,
                                    bufs,
                                    Depth, Stencil,
                                    0, 0,
                                    dsv->surface->width, dst->surface->height);
}

void WINAPI
D3D11DeviceContext_GenerateMips( struct D3D11DeviceContext *This,
                                 ID3D11ShaderResourceView *pShaderResourceView )
{
    struct D3D11ShaderResourceView *srv = D3D11ShaderResourceView(pShaderResourceView);
    unsigned z;

    for (z = srv->sv->u.tex.first_layer; z <= srv->sv->u.tex.last_layer, ++z)
        util_gen_mipmap(This->gen_mipmap, srv->sv, z,
                        srv->sv->u.tex.first_level, srv->sv->u.tex.last_level,
                        PIPE_TEX_FILTER_LINEAR);
}

void WINAPI
D3D11DeviceContext_SetResourceMinLOD( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pResource,
                                      FLOAT MinLOD )
{
    STUB();
}

FLOAT WINAPI
D3D11DeviceContext_GetResourceMinLOD( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pResource )
{
    STUB_return(0);
}

void WINAPI
D3D11DeviceContext_ResolveSubresource( struct D3D11DeviceContext *This,
                                       ID3D11Resource *pDstResource,
                                       UINT DstSubresource,
                                       ID3D11Resource *pSrcResource,
                                       UINT SrcSubresource,
                                       DXGI_FORMAT Format )
{
    struct D3D11Resource *dst = D3D11Resource(pDstResource);
    struct D3D11Resource *src = D3D11Resource(pSrcResource);
    struct pipe_context *pipe = This->pipe;
    struct pipe_blit_info blit;
    enum pipe_format pf = dxgi_to_pipe_format(Format);

    if (dst->resource->nr_samples > 1 ||
        src->resource->nr_samples < 2)
        return;

    blit.dst.resource = dst->resource;
    blit.dst.level = d3d11_subresource_to_level(dst, DstSubresource);
    blit.dst.box.x = 0;
    blit.dst.box.y = 0;
    blit.dst.box.z = d3d11_subresource_to_layer(dst, DstSubresource);
    blit.dst.box.width = u_minify(dst->resource->width0, blit.dst.level);
    blit.dst.box.height = u_minify(dst->resource->height0, blit.dst.level);
    blit.dst.box.depth = 1;
    blit.dst.format = dxgi_to_pipe_format(Format);

    blit.src.resource = src->resource;
    blit.src.level = d3d11_subresource_to_level(src, SrcSubresource);
    blit.src.box.x = 0;
    blit.src.box.y = 0;
    blit.src.box.z = d3d11_subresource_to_layer(src, SrcSubresource);
    blit.src.box.width = u_minify(src->resource->width0, blit.src.level);
    blit.src.box.height = u_minify(src->resource->height0, blit.src.level);
    blit.src.box.depth = 1;
    blit.src.format = blit.dst.format;

    blit.mask = util_format_is_depth_or_stencil(blit.dst.format) ?
        PIPE_MASK_ZS : PIPE_MASK_RGBA;
    blit.filter = PIPE_TEX_FILTER_LINEAR;
    blit.scissor_enable = FALSE;

    pipe->blit(pipe, &blit);
}

void WINAPI
D3D11DeviceContext_ExecuteCommandList( struct D3D11DeviceContext *This,
                                       ID3D11CommandList *pCommandList,
                                       BOOL RestoreContextState )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSSetUnorderedAccessViews( struct D3D11DeviceContext *This,
                                              UINT StartSlot,
                                              UINT NumUAVs,
                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews,
                                              UINT *pUAVInitialCounts )
{
    struct pipe_surface *surf[16];
    unsigned i, b;

    for (i = 0; i < NumUAVs; ++i) {
        b = StartSlot + i;
        assert(b < Elements(This->uav));
        com_ref(&This->uav[b], ppUnorderedAccessViews[i]);
        surf[i] = This->uav[b] ?
            This->uav[b]->surface : NULL;
    }
    This->pipe->set_compute_resources(This->pipe, surf);
}

void WINAPI
D3D11DeviceContext_CSGetUnorderedAccessViews( struct D3D11DeviceContext *This,
                                              UINT StartSlot,
                                              UINT NumUAVs,
                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews )
{
    unsigned i, b;

    assert(ppUnorderedAccessViews || NumUAVs == 0);
    for (i = 0; i < NumUAVs; ++i) {
        b = StartSlot + i;
        com_set(&ppUnorderedAccessViews[i], This->uav[b]);
    }
}

void WINAPI
D3D11DeviceContext_CSSetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader *pComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    com_ref(&This->cs, pComputeShader);
    if (This->cs)
        This->pipe->bind_compute_state(This->pipe, This->cs.so.prog);
    else
        This->pipe->bind_compute_state(This->pipe, NULL);
}

void WINAPI
D3D11DeviceContext_CSGetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader **ppComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    if (ppComputeShader)
        com_set(ppComputeShader, This->cs);
    if (pNumClassInstances)
        *pNumClassInstances = 0;
}

void WINAPI
D3D11DeviceContext_SOSetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets,
                                 UINT *pOffsets )
{
    struct pipe_context *pipe = This->pipe;
    unsigned i;
    unsigned append = 0;

    for (i = 0; i < NumBuffers; ++i) {
        D3D11Buffer *buf = D3D11Buffer(ppSOTargets[i]);

        com_ref(&This->so.buffer[i], buf);
        This->so.target[i] = buf ? buf->so_target : NULL;
        if (!buf)
            continue;
        if (pOffsets[i] == -1) {
            append |= 1 << i;
            assert(This->so.target[i]);
        } else
        if (!buf->so_target || buf->so_target->buffer_offset != pOffsets[i]) {
            buf->so_target = pipe->create_stream_output_target(pipe,
                buf->resource, pOffsets[i], buf->desc.ByteWidth - pOffset[i]);
            This->so.target[i] = buf->so_target;
        }
    }
    for (; i < 4; ++i) {
        com_ref(&This->so.buffer[i], NULL);
        This->so.target[i] = NULL;
    }
    pipe->set_stream_output_targets(pipe, NumBuffers, This->so.target, append);
}

void WINAPI
D3D11DeviceContext_SOGetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets )
{
    unsigned i;
    assert(ppSOTargets);
    for (i = 0; i < NumBuffers; ++i)
        com_set(&ppSOTargets[i], This->so.buffer[i]);
}

static void
D3D11DeviceContext_UpdatePipeState( struct D3D11DeviceContext *This )
{
    struct pipe_context *pipe = This->pipe;
    struct pipe_stencil_ref ref;
    unsigned s, i;

    for (s = 0; s < 6; ++s)
        for (i = 0; i < This->base.device->max_constbufs; ++i)
            pipe->set_constant_buffer(pipe, s, i, NULL);

    pipe->bind_vs_state(pipe, NULL);
    pipe->bind_hs_state(pipe, NULL);
    pipe->bind_ds_state(pipe, NULL);
    pipe->bind_gs_state(pipe, NULL);
    pipe->bind_fs_state(pipe, NULL);

    pipe->set_vertex_sampler_views(pipe, 0, NULL);
    pipe->set_hull_sampler_views(pipe, 0, NULL);
    pipe->set_domain_sampler_views(pipe, 0, NULL);
    pipe->set_geometry_sampler_views(pipe, 0, NULL);
    pipe->set_fragment_sampler_views(pipe, 0, NULL);
    pipe->set_compute_sampler_views(pipe, 0, NULL);

    pipe->set_shader_resources(pipe, 0, 0, NULL);
    pipe->set_compute_resources(pipe, 0, 0, NULL);

    pipe->set_scissor_states(pipe, 0, This->rs.num_scissors, This->rs.scissor);
    pipe->set_viewport_states(pipe, 0, This->rs.num_vports, This->rs.vport);

    pipe->bind_rasterizer_state(pipe, This->rs.so->cso);
    pipe->bind_depth_stencil_alpha_state(pipe, This->om.ds->cso);
    pipe->bind_blend_state(pipe, This->om.bs->cso);

    ref.ref_value[0] = This->om.stencil_ref;
    ref.ref_value[1] = This->om.stencil_ref;
    pipe->set_stencil_ref(pipe, &ref);

    pipe->set_blend_color(pipe, &This->om.blend_color);
    pipe->set_sample_mask(pipe, This->om.sample_mask);

    pipe->set_stream_output_targets(pipe, 0, NULL, 0);

    pipe->set_index_buffer(pipe, &This->ia.idxbuf);
    pipe->set_vertex_buffers(pipe, 0, This->base.device->max_vtxbufs,
                             This->ia.vtxbuf);
    if (This->ia.so)
        pipe->bind_vertex_elements_state(pipe, This->ia.so->cso);
}

void WINAPI
D3D11DeviceContext_ClearState( struct D3D11DeviceContext *This )
{
    unsigned s, i;

    com_ref(&This->ia.so, NULL);
    com_ref(&This->rs.so, This->base.device->rs_default);
    com_ref(&This->om.ds, This->base.device->ds_default);
    com_ref(&This->om.bs, This->base.device->bs_default);

    This->om.sample_mask = 0xffffffff;
    This->om.stencil_ref = 0x00;
    for (i = 0; i < 4; ++i)
        This->om.blend_color.color[i] = 1.0f;

    memset(&This->rs.vport[0], 0, sizeof(This->rs.vport[0]));
    memset(&This->rs.scissor[0], 0, sizeof(This->rs.scissor[0]));
    This->rs.num_vports = 1;
    This->rs.num_scissors = 1;

    com_ref(&This->vs, NULL);
    com_ref(&This->hs, NULL);
    com_ref(&This->ds, NULL);
    com_ref(&This->gs, NULL);
    com_ref(&This->cs, NULL);

    for (i = 0; i < This->om.num_rtvs; ++i)
        com_ref(&This->om.rtv[i], NULL);
    This->om.num_rtvs = 0;
    com_ref(&This->om.dsv, NULL);

    This->ia.draw.mode = PIPE_PRIM_POINTS;
    This->ia.topology = 0;

    for (i = 0; i < Elements(This->ia.buffer); ++i)
        com_ref(&This->ia.buffer[i], NULL);

    for (s = 0; s < 6; ++s) {
        for (i = 0; i < 16; ++i) {
            com_ref(&This->cb[s][i].buffer, NULL);
            This->cb[s][i].cb.buffer = NULL;
            This->cb[s][i].cb.buffer_offset = 0;
            This->cb[s][i].cb.buffer_size = 0;
        }
    }

    for (i = 0; i < PIPE_MAX_SO_BUFFERS; ++i) {
        com_ref(&This->so.buffer[i], NULL);
        pipe_stream_output_target_reference(&This->so.target[i], NULL);
    }
}

void WINAPI
D3D11DeviceContext_Flush( struct D3D11DeviceContext *This )
{
    This->pipe->flush(This->pipe, NULL, 0);
}

D3D11_DEVICE_CONTEXT_TYPE WINAPI
D3D11DeviceContext_GetType( struct D3D11DeviceContext *This )
{
    return This->type;
}

UINT WINAPI
D3D11DeviceContext_GetContextFlags( struct D3D11DeviceContext *This )
{
    return This->flags;
}

HRESULT WINAPI
D3D11DeviceContext_FinishCommandList( struct D3D11DeviceContext *This,
                                      BOOL RestoreDeferredContextState,
                                      ID3D11CommandList **ppCommandList )
{
    STUB_return(E_NOTIMPL);
}

ID3D11DeviceContextVtbl D3D11DeviceContext_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11DeviceContext_VSSetConstantBuffers,
    (void *)D3D11DeviceContext_PSSetShaderResources,
    (void *)D3D11DeviceContext_PSSetShader,
    (void *)D3D11DeviceContext_PSSetSamplers,
    (void *)D3D11DeviceContext_VSSetShader,
    (void *)D3D11DeviceContext_DrawIndexed,
    (void *)D3D11DeviceContext_Draw,
    (void *)D3D11DeviceContext_Map,
    (void *)D3D11DeviceContext_Unmap,
    (void *)D3D11DeviceContext_PSSetConstantBuffers,
    (void *)D3D11DeviceContext_IASetInputLayout,
    (void *)D3D11DeviceContext_IASetVertexBuffers,
    (void *)D3D11DeviceContext_IASetIndexBuffer,
    (void *)D3D11DeviceContext_DrawIndexedInstanced,
    (void *)D3D11DeviceContext_DrawInstanced,
    (void *)D3D11DeviceContext_GSSetConstantBuffers,
    (void *)D3D11DeviceContext_GSSetShader,
    (void *)D3D11DeviceContext_IASetPrimitiveTopology,
    (void *)D3D11DeviceContext_VSSetShaderResources,
    (void *)D3D11DeviceContext_VSSetSamplers,
    (void *)D3D11DeviceContext_Begin,
    (void *)D3D11DeviceContext_End,
    (void *)D3D11DeviceContext_GetData,
    (void *)D3D11DeviceContext_SetPredication,
    (void *)D3D11DeviceContext_GSSetShaderResources,
    (void *)D3D11DeviceContext_GSSetSamplers,
    (void *)D3D11DeviceContext_OMSetRenderTargets,
    (void *)D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews,
    (void *)D3D11DeviceContext_OMSetBlendState,
    (void *)D3D11DeviceContext_OMSetDepthStencilState,
    (void *)D3D11DeviceContext_SOSetTargets,
    (void *)D3D11DeviceContext_DrawAuto,
    (void *)D3D11DeviceContext_DrawIndexedInstancedIndirect,
    (void *)D3D11DeviceContext_DrawInstancedIndirect,
    (void *)D3D11DeviceContext_Dispatch,
    (void *)D3D11DeviceContext_DispatchIndirect,
    (void *)D3D11DeviceContext_RSSetState,
    (void *)D3D11DeviceContext_RSSetViewports,
    (void *)D3D11DeviceContext_RSSetScissorRects,
    (void *)D3D11DeviceContext_CopySubresourceRegion,
    (void *)D3D11DeviceContext_CopyResource,
    (void *)D3D11DeviceContext_UpdateSubresource,
    (void *)D3D11DeviceContext_CopyStructureCount,
    (void *)D3D11DeviceContext_ClearRenderTargetView,
    (void *)D3D11DeviceContext_ClearUnorderedAccessViewUint,
    (void *)D3D11DeviceContext_ClearUnorderedAccessViewFloat,
    (void *)D3D11DeviceContext_ClearDepthStencilView,
    (void *)D3D11DeviceContext_GenerateMips,
    (void *)D3D11DeviceContext_SetResourceMinLOD,
    (void *)D3D11DeviceContext_GetResourceMinLOD,
    (void *)D3D11DeviceContext_ResolveSubresource,
    (void *)D3D11DeviceContext_ExecuteCommandList,
    (void *)D3D11DeviceContext_HSSetShaderResources,
    (void *)D3D11DeviceContext_HSSetShader,
    (void *)D3D11DeviceContext_HSSetSamplers,
    (void *)D3D11DeviceContext_HSSetConstantBuffers,
    (void *)D3D11DeviceContext_DSSetShaderResources,
    (void *)D3D11DeviceContext_DSSetShader,
    (void *)D3D11DeviceContext_DSSetSamplers,
    (void *)D3D11DeviceContext_DSSetConstantBuffers,
    (void *)D3D11DeviceContext_CSSetShaderResources,
    (void *)D3D11DeviceContext_CSSetUnorderedAccessViews,
    (void *)D3D11DeviceContext_CSSetShader,
    (void *)D3D11DeviceContext_CSSetSamplers,
    (void *)D3D11DeviceContext_CSSetConstantBuffers,
    (void *)D3D11DeviceContext_VSGetConstantBuffers,
    (void *)D3D11DeviceContext_PSGetShaderResources,
    (void *)D3D11DeviceContext_PSGetShader,
    (void *)D3D11DeviceContext_PSGetSamplers,
    (void *)D3D11DeviceContext_VSGetShader,
    (void *)D3D11DeviceContext_PSGetConstantBuffers,
    (void *)D3D11DeviceContext_IAGetInputLayout,
    (void *)D3D11DeviceContext_IAGetVertexBuffers,
    (void *)D3D11DeviceContext_IAGetIndexBuffer,
    (void *)D3D11DeviceContext_GSGetConstantBuffers,
    (void *)D3D11DeviceContext_GSGetShader,
    (void *)D3D11DeviceContext_IAGetPrimitiveTopology,
    (void *)D3D11DeviceContext_VSGetShaderResources,
    (void *)D3D11DeviceContext_VSGetSamplers,
    (void *)D3D11DeviceContext_GetPredication,
    (void *)D3D11DeviceContext_GSGetShaderResources,
    (void *)D3D11DeviceContext_GSGetSamplers,
    (void *)D3D11DeviceContext_OMGetRenderTargets,
    (void *)D3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews,
    (void *)D3D11DeviceContext_OMGetBlendState,
    (void *)D3D11DeviceContext_OMGetDepthStencilState,
    (void *)D3D11DeviceContext_SOGetTargets,
    (void *)D3D11DeviceContext_RSGetState,
    (void *)D3D11DeviceContext_RSGetViewports,
    (void *)D3D11DeviceContext_RSGetScissorRects,
    (void *)D3D11DeviceContext_HSGetShaderResources,
    (void *)D3D11DeviceContext_HSGetShader,
    (void *)D3D11DeviceContext_HSGetSamplers,
    (void *)D3D11DeviceContext_HSGetConstantBuffers,
    (void *)D3D11DeviceContext_DSGetShaderResources,
    (void *)D3D11DeviceContext_DSGetShader,
    (void *)D3D11DeviceContext_DSGetSamplers,
    (void *)D3D11DeviceContext_DSGetConstantBuffers,
    (void *)D3D11DeviceContext_CSGetShaderResources,
    (void *)D3D11DeviceContext_CSGetUnorderedAccessViews,
    (void *)D3D11DeviceContext_CSGetShader,
    (void *)D3D11DeviceContext_CSGetSamplers,
    (void *)D3D11DeviceContext_CSGetConstantBuffers,
    (void *)D3D11DeviceContext_ClearState,
    (void *)D3D11DeviceContext_Flush,
    (void *)D3D11DeviceContext_GetType,
    (void *)D3D11DeviceContext_GetContextFlags,
    (void *)D3D11DeviceContext_FinishCommandList
};

static const GUID *D3D11DeviceContext_IIDs[] = {
    &IID_ID3D11DeviceContext,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11DeviceContext_new( struct D3D11Device *pDevice,
struct D3D11DeviceContext **ppOut )
{
    D3D11_NEW(D3D11DeviceContext, ppOut, pDevice);
}

