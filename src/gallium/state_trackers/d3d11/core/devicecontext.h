
#ifndef _D3D11_DEVICECONTEXT_H_
#define _D3D11_DEVICECONTEXT_H_

#include "devicechild.h"

struct D3D11DeviceContext
{
    struct D3D11DeviceChild base;

    D3D11_DEVICE_CONTEXT_TYPE type;
    UINT flags;

    struct {
        struct D3D11InputLayout *so;
        D3D11_PRIMITIVE_TOPOLOGY topology;
        struct D3D11Buffer *buffer[PIPE_MAX_ATTRIBS + 1];
        struct pipe_draw_info draw;
        struct pipe_vertex_buffer vtxbuf[PIPE_MAX_ATTRIBS];
        struct pipe_index_buffer idxbuf;
    } ia;
    struct {
        struct D3D11RasterizerState *so;
        struct pipe_scissor_state scissor[PIPE_MAX_VIEWPORTS];
        struct pipe_viewport_state vport[PIPE_MAX_VIEWPORTS];
        unsigned num_scissors;
        unsigned num_vports;
    } rs;
    struct {
        struct D3D11RenderTargetView *rtv[PIPE_MAX_COLOR_BUFS];
        struct D3D11DepthStencilView *dsv;
        struct D3D11DepthStencilState *ds;
        struct D3D11BlendState *bs;
        uint8_t num_rtvs;
        uint8_t stencil_ref;
        struct pipe_blend_color blend_color;
        uint32_t sample_mask;
        struct D3D11UnorderedAccessView *uav[16];
    } om;

    struct D3D11VertexShader   *vs;
    struct D3D11HullShader     *hs;
    struct D3D11DomainShader   *ds;
    struct D3D11GeometryShader *gs;
    struct D3D11PixelShader    *ps;
    struct D3D11ComputeShader  *cs;

    struct {
        D3D11Buffer *buffer;
        struct pipe_constant_buffer cb;
    } cb[6][16];

    struct D3D11SamplerState *ss[6][PIPE_MAX_SAMPLERS];
    struct D3D11ShaderResourceView *srv[6][PIPE_MAX_SHADER_SAMPLER_VIEWS];
    void *ss_cso[6][PIPE_MAX_SAMPLERS];
    struct pipe_sampler_view *srv_cso[6][PIPE_MAX_SHADER_SAMPLER_VIEWS];

    struct D3D11UnorderedAccessView *uav[16];

    struct {
        struct D3D11Buffer *buffer[PIPE_MAX_SO_BUFFERS];
        struct pipe_stream_output_target *target[PIPE_MAX_SO_BUFFERS];
    } so;

    struct D3D11Predicate *predicate;
    BOOL predicate_value;
};
static INLINE struct D3D11DeviceContext *D3D11DeviceContext(void *ptr)
{
    return (struct D3D11DeviceContext *)ptr;
}

HRESULT
D3D11DeviceContext_new( struct D3D11Device *, struct D3D11DeviceContext **ppOut );

void WINAPI
D3D11DeviceContext_VSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_PSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_PSSetShader( struct D3D11DeviceContext *This,
                                ID3D11PixelShader *pPixelShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_PSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_VSSetShader( struct D3D11DeviceContext *This,
                                ID3D11VertexShader *pVertexShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_DrawIndexed( struct D3D11DeviceContext *This,
                                UINT IndexCount,
                                UINT StartIndexLocation,
                                INT BaseVertexLocation );

void WINAPI
D3D11DeviceContext_Draw( struct D3D11DeviceContext *This,
                         UINT VertexCount,
                         UINT StartVertexLocation );

HRESULT WINAPI
D3D11DeviceContext_Map( struct D3D11DeviceContext *This,
                        ID3D11Resource *pResource,
                        UINT Subresource,
                        D3D11_MAP MapType,
                        UINT MapFlags,
                        D3D11_MAPPED_SUBRESOURCE *pMappedResource );

void WINAPI
D3D11DeviceContext_Unmap( struct D3D11DeviceContext *This,
                          ID3D11Resource *pResource,
                          UINT Subresource );

void WINAPI
D3D11DeviceContext_PSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_IASetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout *pInputLayout );

void WINAPI
D3D11DeviceContext_IASetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets );

void WINAPI
D3D11DeviceContext_IASetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer *pIndexBuffer,
                                     Int Format,
                                     UINT Offset );

void WINAPI
D3D11DeviceContext_DrawIndexedInstanced( struct D3D11DeviceContext *This,
                                         UINT IndexCountPerInstance,
                                         UINT InstanceCount,
                                         UINT StartIndexLocation,
                                         INT BaseVertexLocation,
                                         UINT StartInstanceLocation );

void WINAPI
D3D11DeviceContext_DrawInstanced( struct D3D11DeviceContext *This,
                                  UINT VertexCountPerInstance,
                                  UINT InstanceCount,
                                  UINT StartVertexLocation,
                                  UINT StartInstanceLocation );

void WINAPI
D3D11DeviceContext_GSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_GSSetShader( struct D3D11DeviceContext *This,
                                ID3D11GeometryShader *pShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_IASetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY Topology );

void WINAPI
D3D11DeviceContext_VSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_VSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_Begin( struct D3D11DeviceContext *This,
                          ID3D11Asynchronous *pAsync );

void WINAPI
D3D11DeviceContext_End( struct D3D11DeviceContext *This,
                        ID3D11Asynchronous *pAsync );

HRESULT WINAPI
D3D11DeviceContext_GetData( struct D3D11DeviceContext *This,
                            ID3D11Asynchronous *pAsync,
                            void *pData,
                            UINT DataSize,
                            UINT GetDataFlags );

void WINAPI
D3D11DeviceContext_SetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate *pPredicate,
                                   BOOL PredicateValue );

void WINAPI
D3D11DeviceContext_GSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_GSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_OMSetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView *pDepthStencilView );

void WINAPI
D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews( struct D3D11DeviceContext *This,
                                                              UINT NumRTVs,
                                                              ID3D11RenderTargetView **ppRenderTargetViews,
                                                              ID3D11DepthStencilView *pDepthStencilView,
                                                              UINT UAVStartSlot,
                                                              UINT NumUAVs,
                                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews,
                                                              UINT *pUAVInitialCounts );

void WINAPI
D3D11DeviceContext_OMSetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState *pBlendState,
                                    ConstantArray BlendFactor,
                                    UINT SampleMask );

void WINAPI
D3D11DeviceContext_OMSetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState *pDepthStencilState,
                                           UINT StencilRef );

void WINAPI
D3D11DeviceContext_SOSetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets,
                                 UINT *pOffsets );

void WINAPI
D3D11DeviceContext_DrawAuto( struct D3D11DeviceContext *This );

void WINAPI
D3D11DeviceContext_DrawIndexedInstancedIndirect( struct D3D11DeviceContext *This,
                                                 ID3D11Buffer *pBufferForArgs,
                                                 UINT AlignedByteOffsetForArgs );

void WINAPI
D3D11DeviceContext_DrawInstancedIndirect( struct D3D11DeviceContext *This,
                                          ID3D11Buffer *pBufferForArgs,
                                          UINT AlignedByteOffsetForArgs );

void WINAPI
D3D11DeviceContext_Dispatch( struct D3D11DeviceContext *This,
                             UINT ThreadGroupCountX,
                             UINT ThreadGroupCountY,
                             UINT ThreadGroupCountZ );

void WINAPI
D3D11DeviceContext_DispatchIndirect( struct D3D11DeviceContext *This,
                                     ID3D11Buffer *pBufferForArgs,
                                     UINT AlignedByteOffsetForArgs );

void WINAPI
D3D11DeviceContext_RSSetState( struct D3D11DeviceContext *This,
                               ID3D11RasterizerState *pRasterizerState );

void WINAPI
D3D11DeviceContext_RSSetViewports( struct D3D11DeviceContext *This,
                                   UINT NumViewports,
                                   D3D11_VIEWPORT *pViewports );

void WINAPI
D3D11DeviceContext_RSSetScissorRects( struct D3D11DeviceContext *This,
                                      UINT NumRects,
                                      D3D11_RECT *pRects );

void WINAPI
D3D11DeviceContext_CopySubresourceRegion( struct D3D11DeviceContext *This,
                                          ID3D11Resource *pDstResource,
                                          UINT DstSubresource,
                                          UINT DstX,
                                          UINT DstY,
                                          UINT DstZ,
                                          ID3D11Resource *pSrcResource,
                                          UINT SrcSubresource,
                                          D3D11_BOX *pSrcBox );

void WINAPI
D3D11DeviceContext_CopyResource( struct D3D11DeviceContext *This,
                                 ID3D11Resource *pDstResource,
                                 ID3D11Resource *pSrcResource );

void WINAPI
D3D11DeviceContext_UpdateSubresource( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pDstResource,
                                      UINT DstSubresource,
                                      D3D11_BOX *pDstBox,
                                      void *pSrcData,
                                      UINT SrcRowPitch,
                                      UINT SrcDepthPitch );

void WINAPI
D3D11DeviceContext_CopyStructureCount( struct D3D11DeviceContext *This,
                                       ID3D11Buffer *pDstBuffer,
                                       UINT DstAlignedByteOffset,
                                       ID3D11UnorderedAccessView *pSrcView );

void WINAPI
D3D11DeviceContext_ClearRenderTargetView( struct D3D11DeviceContext *This,
                                          ID3D11RenderTargetView *pRenderTargetView,
                                          ConstantArray ColorRGBA );

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewUint( struct D3D11DeviceContext *This,
                                                 ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                 ConstantArray Values );

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewFloat( struct D3D11DeviceContext *This,
                                                  ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                  ConstantArray Values );

void WINAPI
D3D11DeviceContext_ClearDepthStencilView( struct D3D11DeviceContext *This,
                                          ID3D11DepthStencilView *pDepthStencilView,
                                          UINT ClearFlags,
                                          FLOAT Depth,
                                          Int Stencil );

void WINAPI
D3D11DeviceContext_GenerateMips( struct D3D11DeviceContext *This,
                                 ID3D11ShaderResourceView *pShaderResourceView );

void WINAPI
D3D11DeviceContext_SetResourceMinLOD( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pResource,
                                      FLOAT MinLOD );

FLOAT WINAPI
D3D11DeviceContext_GetResourceMinLOD( struct D3D11DeviceContext *This,
                                      ID3D11Resource *pResource );

void WINAPI
D3D11DeviceContext_ResolveSubresource( struct D3D11DeviceContext *This,
                                       ID3D11Resource *pDstResource,
                                       UINT DstSubresource,
                                       ID3D11Resource *pSrcResource,
                                       UINT SrcSubresource,
                                       Int Format );

void WINAPI
D3D11DeviceContext_ExecuteCommandList( struct D3D11DeviceContext *This,
                                       ID3D11CommandList *pCommandList,
                                       BOOL RestoreContextState );

void WINAPI
D3D11DeviceContext_HSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_HSSetShader( struct D3D11DeviceContext *This,
                                ID3D11HullShader *pHullShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_HSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_HSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_DSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_DSSetShader( struct D3D11DeviceContext *This,
                                ID3D11DomainShader *pDomainShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_DSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_DSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_CSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_CSSetUnorderedAccessViews( struct D3D11DeviceContext *This,
                                              UINT StartSlot,
                                              UINT NumUAVs,
                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews,
                                              UINT *pUAVInitialCounts );

void WINAPI
D3D11DeviceContext_CSSetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader *pComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances );

void WINAPI
D3D11DeviceContext_CSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_CSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_VSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_PSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_PSGetShader( struct D3D11DeviceContext *This,
                                ID3D11PixelShader **ppPixelShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_PSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_VSGetShader( struct D3D11DeviceContext *This,
                                ID3D11VertexShader **ppVertexShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_PSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_IAGetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout **ppInputLayout );

void WINAPI
D3D11DeviceContext_IAGetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets );

void WINAPI
D3D11DeviceContext_IAGetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer **pIndexBuffer,
                                     Int *Format,
                                     UINT *Offset );

void WINAPI
D3D11DeviceContext_GSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_GSGetShader( struct D3D11DeviceContext *This,
                                ID3D11GeometryShader **ppGeometryShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_IAGetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY *pTopology );

void WINAPI
D3D11DeviceContext_VSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_VSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_GetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate **ppPredicate,
                                   BOOL *pPredicateValue );

void WINAPI
D3D11DeviceContext_GSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_GSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_OMGetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView **ppDepthStencilView );

void WINAPI
D3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews( struct D3D11DeviceContext *This,
                                                              UINT NumRTVs,
                                                              ID3D11RenderTargetView **ppRenderTargetViews,
                                                              ID3D11DepthStencilView **ppDepthStencilView,
                                                              UINT UAVStartSlot,
                                                              UINT NumUAVs,
                                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews );

void WINAPI
D3D11DeviceContext_OMGetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState **ppBlendState,
                                    ConstantArray BlendFactor,
                                    UINT *pSampleMask );

void WINAPI
D3D11DeviceContext_OMGetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState **ppDepthStencilState,
                                           UINT *pStencilRef );

void WINAPI
D3D11DeviceContext_SOGetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets );

void WINAPI
D3D11DeviceContext_RSGetState( struct D3D11DeviceContext *This,
                               ID3D11RasterizerState **ppRasterizerState );

void WINAPI
D3D11DeviceContext_RSGetViewports( struct D3D11DeviceContext *This,
                                   UINT *pNumViewports,
                                   D3D11_VIEWPORT *pViewports );

void WINAPI
D3D11DeviceContext_RSGetScissorRects( struct D3D11DeviceContext *This,
                                      UINT *pNumRects,
                                      D3D11_RECT *pRects );

void WINAPI
D3D11DeviceContext_HSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_HSGetShader( struct D3D11DeviceContext *This,
                                ID3D11HullShader **ppHullShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_HSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_HSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_DSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_DSGetShader( struct D3D11DeviceContext *This,
                                ID3D11DomainShader **ppDomainShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_DSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_DSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_CSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews );

void WINAPI
D3D11DeviceContext_CSGetUnorderedAccessViews( struct D3D11DeviceContext *This,
                                              UINT StartSlot,
                                              UINT NumUAVs,
                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews );

void WINAPI
D3D11DeviceContext_CSGetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader **ppComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances );

void WINAPI
D3D11DeviceContext_CSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers );

void WINAPI
D3D11DeviceContext_CSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers );

void WINAPI
D3D11DeviceContext_ClearState( struct D3D11DeviceContext *This );

void WINAPI
D3D11DeviceContext_Flush( struct D3D11DeviceContext *This );

D3D11_DEVICE_CONTEXT_TYPE WINAPI
D3D11DeviceContext_GetType( struct D3D11DeviceContext *This );

UINT WINAPI
D3D11DeviceContext_GetContextFlags( struct D3D11DeviceContext *This );

HRESULT WINAPI
D3D11DeviceContext_FinishCommandList( struct D3D11DeviceContext *This,
                                      BOOL RestoreDeferredContextState,
                                      ID3D11CommandList **ppCommandList );

#endif /* _D3D11_DEVICECONTEXT_H_ */

