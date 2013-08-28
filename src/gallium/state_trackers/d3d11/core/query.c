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

#include "query.h"

static INLINE unsigned
d3d11_to_pipe_query(D3D11_QUERY query)
{
    switch (query) {
    case D3D11_QUERY_EVENT:
        return PIPE_QUERY_GPU_FINISHED;
    case D3D11_QUERY_OCCLUSION:
        return PIPE_QUERY_OCCLUSION_COUNTER;
    case D3D11_QUERY_TIMESTAMP:
        return PIPE_QUERY_TIMESTAMP:
    case D3D11_QUERY_TIMESTAMP_DISJOINT:
        return PIPE_QUERY_TIMESTAMP_DISJOINT;
    case D3D11_QUERY_PIPELINE_STATISTICS:
        return PIPE_QUERY_PIPELINE_STATISTICS;
    case D3D11_QUERY_OCCLUSION_PREDICATE:
        return PIPE_QUERY_OCCLUSION_PREDICATE;
    case D3D11_QUERY_SO_STATISTICS:
    case D3D11_QUERY_SO_STATISTICS_STREAM0:
    case D3D11_QUERY_SO_STATISTICS_STREAM1:
    case D3D11_QUERY_SO_STATISTICS_STREAM2:
    case D3D11_QUERY_SO_STATISTICS_STREAM3:
        return PIPE_QUERY_SO_STATISTICS;
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM3:
        return PIPE_QUERY_SO_OVERFLOW_PREDICATE;
    default:
        return PIPE_QUERY_TYPES;
   }
}

static INLINE UINT
d3d11_query_data_size(D3D11_QUERY query)
{
    switch (query) {
    case D3D11_QUERY_EVENT:
        return sizeof(BOOL);
    case D3D11_QUERY_OCCLUSION:
        return sizeof(UINT64);
    case D3D11_QUERY_TIMESTAMP:
        return sizeof(UINT64);
    case D3D11_QUERY_TIMESTAMP_DISJOINT:
        return sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT);
    case D3D11_QUERY_PIPELINE_STATISTICS:
        return sizeof(D3D11_QUERY_DATA_PIPELINE_STATISTICS);
    case D3D11_QUERY_OCCLUSION_PREDICATE:
        return sizeof(BOOL);
    case D3D11_QUERY_SO_STATISTICS:
    case D3D11_QUERY_SO_STATISTICS_STREAM0:
    case D3D11_QUERY_SO_STATISTICS_STREAM1:
    case D3D11_QUERY_SO_STATISTICS_STREAM2:
    case D3D11_QUERY_SO_STATISTICS_STREAM3:
        return sizeof(D3D11_QUERY_DATA_SO_STATISTICS);
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM3:
        return sizeof(BOOL);
    default:
        return 0;
    }
}

HRESULT
D3D11Query_ctor( struct D3D11Query *This,
                 struct D3D11UnknownParams *pParams,
                 const D3D11_QUERY_DESC *pDesc )
{
    struct pipe_context *pipe = pParams->device->pipe;
    HRESULT hr;

    hr = D3D11Asynchronous_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;
    This->base.data_size = d3d11_query_data_size(pDesc->Query);
    if (!This->base.data_size)
        return_error(E_INVALIDARG);

    This->base.pq_type = d3d11_to_pipe_query(pDesc->Query);
    if (This->base.pq_type == PIPE_QUERY_TYPES)
        return_error(E_NOTIMPL);

    This->base.pq = pipe->create_query(pipe, This->base.pq_type);
    if (!This->base.pq)
        return_error(D3DERR_DRIVERINTERNALERROR);

    This->desc = *pDesc;

    return S_OK;
}

void
D3D11Query_dtor( struct D3D11Query *This )
{
    if (This->base.pq) {
        struct pipe_context *pipe = This->base.base.device->pipe;
        pipe->destroy_query(pipe, This->base.pq);
    }
    D3D11Asynchronous_dtor(&This->base);
}

void WINAPI
D3D11Query_GetDesc( struct D3D11Query *This,
                    D3D11_QUERY_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11QueryVtbl D3D11Query_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11Asynchronous_GetDataSize,
    (void *)D3D11Query_GetDesc
};

static const GUID *D3D11Query_IIDs[] = {
    &IID_ID3D11Query,
    &IID_ID3D11Asynchronous,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11Query_new( struct D3D11Device *pDevice,
                const D3D11_QUERY_DESC *pDesc,
                struct D3D11Query **ppOut )
{
    D3D11_NEW(D3D11Query, ppOut, pDevice, pDesc);
}


union d3d11_query_result
{
    UINT64 u64;
    BOOL b;
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT tsd;
    D3D11_QUERY_DATA_SO_STATISTICS so;
    D3D11_QUERY_DATA_PIPELINE_STATISTICS ps;
};
HRESULT
D3D11Query_GetData( struct D3D11Query *This,
                    struct D3D11DeviceContext *pContext,
                    void *pData,
                    UINT DataSize,
                    UINT GetDataFlags )
{
    struct pipe_context *pipe = pContext->pipe;
    boolean ret;
    union pipe_query_result presult;
    union d3d11_query_result uresult;

    if (This->desc.MiscFlags & D3D11_QUERY_MISC_PREDICATEHINT)
        return_error(D3DERR_INVALIDCALL);

    if (!This->pipe->get_query_result(This->pipe, async->pq))
        return S_FAIL;

    switch (query->desc.Query) {
    case D3D11_QUERY_EVENT:
    case D3D11_QUERY_OCCLUSION_PREDICATE:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2:
    case D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM3:
        uresult.b = presult.b;
        break;
    case D3D11_QUERY_OCCLUSION:
    case D3D11_QUERY_TIMESTAMP:
        uresult.u64 = presult.u64;
        break;
    case D3D11_QUERY_TIMESTAMP_DISJOINT:
        uresult.tsd.Frequency = presult.timestamp_disjoint.frequency;
        uresult.tsd.Disjoint = presult.timestamp_disjoint.disjoint;
        break;
    case D3D11_QUERY_PIPELINE_STATISTICS:
        uresult.ps.IAVertices = presult.pipeline_statistics.ia_vertices;
        uresult.ps.IAPrimitives = presult.pipeline_statistics.ia_primitives;
        uresult.ps.VSInvocations = presult.pipeline_statistics.vs_invocations;
        uresult.ps.GSInvocations = presult.pipeline_statistics.gs_invocations;
        uresult.ps.GSPrimitives = presult.pipeline_statistics.gs_primitives;
        uresult.ps.CInvocations = presult.pipeline_statistics.c_invocations;
        uresult.ps.CPrimitives = presult.pipeline_statistics.c_primitives;
        uresult.ps.PSInvocations = presult.pipeline_statistics.ps_invocations;
        uresult.ps.HSInvocations = presult.pipeline_statistics.hs_invocations;
        uresult.ps.DSInvocations = presult.pipeline_statistics.ds_invocations;
        uresult.ps.CSInvocations = presult.pipeline_statistics.cs_invocations;
        break;
    case D3D11_QUERY_SO_STATISTICS:
    case D3D11_QUERY_SO_STATISTICS_STREAM0:
    case D3D11_QUERY_SO_STATISTICS_STREAM1:
    case D3D11_QUERY_SO_STATISTICS_STREAM2:
    case D3D11_QUERY_SO_STATISTICS_STREAM3:
        uresult.so.NumPrimitivesWritten = presult.so_statistics.num_primitives_written;
        uresult.so.PrimitivesStorageNeeded = presult.so_statistics.primitives_storage_needed;
        break;
    default:
        assert(0);
        break;
    }
    memcpy(pData, &uresult, DataSize);

    return S_OK;
}
