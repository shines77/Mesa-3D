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
    HRESULT hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11DeviceContext_dtor( struct D3D11DeviceContext *This )
{
    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11DeviceContext_VSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSSetShader( struct D3D11DeviceContext *This,
                                ID3D11PixelShader *pPixelShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSSetShader( struct D3D11DeviceContext *This,
                                ID3D11VertexShader *pVertexShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawIndexed( struct D3D11DeviceContext *This,
                                UINT IndexCount,
                                UINT StartIndexLocation,
                                INT BaseVertexLocation )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_Draw( struct D3D11DeviceContext *This,
                         UINT VertexCount,
                         UINT StartVertexLocation )
{
    STUB();
}

HRESULT WINAPI
D3D11DeviceContext_Map( struct D3D11DeviceContext *This,
                        ID3D11Resource *pResource,
                        UINT Subresource,
                        D3D11_MAP MapType,
                        UINT MapFlags,
                        D3D11_MAPPED_SUBRESOURCE *pMappedResource )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11DeviceContext_Unmap( struct D3D11DeviceContext *This,
                          ID3D11Resource *pResource,
                          UINT Subresource )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IASetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout *pInputLayout )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IASetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IASetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer *pIndexBuffer,
                                     Int Format,
                                     UINT Offset )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawIndexedInstanced( struct D3D11DeviceContext *This,
                                         UINT IndexCountPerInstance,
                                         UINT InstanceCount,
                                         UINT StartIndexLocation,
                                         INT BaseVertexLocation,
                                         UINT StartInstanceLocation )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawInstanced( struct D3D11DeviceContext *This,
                                  UINT VertexCountPerInstance,
                                  UINT InstanceCount,
                                  UINT StartVertexLocation,
                                  UINT StartInstanceLocation )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSSetShader( struct D3D11DeviceContext *This,
                                ID3D11GeometryShader *pShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IASetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY Topology )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_Begin( struct D3D11DeviceContext *This,
                          ID3D11Asynchronous *pAsync )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_End( struct D3D11DeviceContext *This,
                        ID3D11Asynchronous *pAsync )
{
    STUB();
}

HRESULT WINAPI
D3D11DeviceContext_GetData( struct D3D11DeviceContext *This,
                            ID3D11Asynchronous *pAsync,
                            void *pData,
                            UINT DataSize,
                            UINT GetDataFlags )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11DeviceContext_SetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate *pPredicate,
                                   BOOL PredicateValue )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_OMSetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView *pDepthStencilView )
{
    STUB();
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
    STUB();
}

void WINAPI
D3D11DeviceContext_OMSetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState *pBlendState,
                                    ConstantArray BlendFactor,
                                    UINT SampleMask )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_OMSetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState *pDepthStencilState,
                                           UINT StencilRef )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_SOSetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets,
                                 UINT *pOffsets )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawAuto( struct D3D11DeviceContext *This )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawIndexedInstancedIndirect( struct D3D11DeviceContext *This,
                                                 ID3D11Buffer *pBufferForArgs,
                                                 UINT AlignedByteOffsetForArgs )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DrawInstancedIndirect( struct D3D11DeviceContext *This,
                                          ID3D11Buffer *pBufferForArgs,
                                          UINT AlignedByteOffsetForArgs )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_Dispatch( struct D3D11DeviceContext *This,
                             UINT ThreadGroupCountX,
                             UINT ThreadGroupCountY,
                             UINT ThreadGroupCountZ )
{
    STUB();
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
    STUB();
}

void WINAPI
D3D11DeviceContext_RSSetViewports( struct D3D11DeviceContext *This,
                                   UINT NumViewports,
                                   D3D11_VIEWPORT *pViewports )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_RSSetScissorRects( struct D3D11DeviceContext *This,
                                      UINT NumRects,
                                      D3D11_RECT *pRects )
{
    STUB();
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
    STUB();
}

void WINAPI
D3D11DeviceContext_CopyResource( struct D3D11DeviceContext *This,
                                 ID3D11Resource *pDstResource,
                                 ID3D11Resource *pSrcResource )
{
    STUB();
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
    STUB();
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
    STUB();
}

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewUint( struct D3D11DeviceContext *This,
                                                 ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                 ConstantArray Values )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_ClearUnorderedAccessViewFloat( struct D3D11DeviceContext *This,
                                                  ID3D11UnorderedAccessView *pUnorderedAccessView,
                                                  ConstantArray Values )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_ClearDepthStencilView( struct D3D11DeviceContext *This,
                                          ID3D11DepthStencilView *pDepthStencilView,
                                          UINT ClearFlags,
                                          FLOAT Depth,
                                          Int Stencil )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GenerateMips( struct D3D11DeviceContext *This,
                                 ID3D11ShaderResourceView *pShaderResourceView )
{
    STUB();
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
                                       Int Format )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_ExecuteCommandList( struct D3D11DeviceContext *This,
                                       ID3D11CommandList *pCommandList,
                                       BOOL RestoreContextState )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSSetShader( struct D3D11DeviceContext *This,
                                ID3D11HullShader *pHullShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSSetShader( struct D3D11DeviceContext *This,
                                ID3D11DomainShader *pDomainShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSSetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
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
    STUB();
}

void WINAPI
D3D11DeviceContext_CSSetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader *pComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT NumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSSetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSSetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSGetShader( struct D3D11DeviceContext *This,
                                ID3D11PixelShader **ppPixelShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSGetShader( struct D3D11DeviceContext *This,
                                ID3D11VertexShader **ppVertexShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_PSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IAGetInputLayout( struct D3D11DeviceContext *This,
                                     ID3D11InputLayout **ppInputLayout )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IAGetVertexBuffers( struct D3D11DeviceContext *This,
                                       UINT StartSlot,
                                       UINT NumBuffers,
                                       ID3D11Buffer **ppVertexBuffers,
                                       UINT *pStrides,
                                       UINT *pOffsets )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IAGetIndexBuffer( struct D3D11DeviceContext *This,
                                     ID3D11Buffer **pIndexBuffer,
                                     Int *Format,
                                     UINT *Offset )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSGetShader( struct D3D11DeviceContext *This,
                                ID3D11GeometryShader **ppGeometryShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_IAGetPrimitiveTopology( struct D3D11DeviceContext *This,
                                           D3D11_PRIMITIVE_TOPOLOGY *pTopology )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_VSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate **ppPredicate,
                                   BOOL *pPredicateValue )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_GSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_OMGetRenderTargets( struct D3D11DeviceContext *This,
                                       UINT NumViews,
                                       ID3D11RenderTargetView **ppRenderTargetViews,
                                       ID3D11DepthStencilView **ppDepthStencilView )
{
    STUB();
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
    STUB();
}

void WINAPI
D3D11DeviceContext_OMGetBlendState( struct D3D11DeviceContext *This,
                                    ID3D11BlendState **ppBlendState,
                                    ConstantArray BlendFactor,
                                    UINT *pSampleMask )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_OMGetDepthStencilState( struct D3D11DeviceContext *This,
                                           ID3D11DepthStencilState **ppDepthStencilState,
                                           UINT *pStencilRef )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_SOGetTargets( struct D3D11DeviceContext *This,
                                 UINT NumBuffers,
                                 ID3D11Buffer **ppSOTargets )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_RSGetState( struct D3D11DeviceContext *This,
                               ID3D11RasterizerState **ppRasterizerState )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_RSGetViewports( struct D3D11DeviceContext *This,
                                   UINT *pNumViewports,
                                   D3D11_VIEWPORT *pViewports )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_RSGetScissorRects( struct D3D11DeviceContext *This,
                                      UINT *pNumRects,
                                      D3D11_RECT *pRects )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSGetShader( struct D3D11DeviceContext *This,
                                ID3D11HullShader **ppHullShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_HSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSGetShader( struct D3D11DeviceContext *This,
                                ID3D11DomainShader **ppDomainShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_DSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSGetShaderResources( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumViews,
                                         ID3D11ShaderResourceView **ppShaderResourceViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSGetUnorderedAccessViews( struct D3D11DeviceContext *This,
                                              UINT StartSlot,
                                              UINT NumUAVs,
                                              ID3D11UnorderedAccessView **ppUnorderedAccessViews )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSGetShader( struct D3D11DeviceContext *This,
                                ID3D11ComputeShader **ppComputeShader,
                                ID3D11ClassInstance **ppClassInstances,
                                UINT *pNumClassInstances )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSGetSamplers( struct D3D11DeviceContext *This,
                                  UINT StartSlot,
                                  UINT NumSamplers,
                                  ID3D11SamplerState **ppSamplers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_CSGetConstantBuffers( struct D3D11DeviceContext *This,
                                         UINT StartSlot,
                                         UINT NumBuffers,
                                         ID3D11Buffer **ppConstantBuffers )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_ClearState( struct D3D11DeviceContext *This )
{
    STUB();
}

void WINAPI
D3D11DeviceContext_Flush( struct D3D11DeviceContext *This )
{
    STUB();
}

D3D11_DEVICE_CONTEXT_TYPE WINAPI
D3D11DeviceContext_GetType( struct D3D11DeviceContext *This )
{
    STUB_return(0);
}

UINT WINAPI
D3D11DeviceContext_GetContextFlags( struct D3D11DeviceContext *This )
{
    STUB_return(0);
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

