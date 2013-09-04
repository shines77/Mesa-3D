
#ifndef _D3D11_DEVICE_H_
#define _D3D11_DEVICE_H_

#include "iunknown.h"
#include "privatedata.h"

#include "util/u_hash_table.h"

#define D3D11_MAX_UNIQUE_STATE_OBJECT_COUNT 4096

struct D3D11Device
{
    struct D3D11Unknown base;
    struct D3D11DeviceContext *immediate_context;
    struct pipe_screen *screen;
    struct pipe_context *pipe;
    struct pipe_context *pipe2;

    struct util_hash_tabke *ht_bs;
    struct util_hash_table *ht_ds;
    struct util_hash_table *ht_rs;
    struct util_hash_table *ht_ss;
    uint16_t bs_count;
    uint16_t ds_count;
    uint16_t rs_count;
    uint16_t ss_count;
    struct D3D11BlendState        *bs_default;
    struct D3D11DepthStencilState *ds_default;
    struct D3D11RasterizerState   *rs_default;
    struct D3D11SamplerState      *ss_default;

    UINT creation_flags;
    UINT exceptions;
    HRESULT fatal_reason:

    struct D3D11PrivateData pdata;

    D3D_FEATURE_LEVEL feature_level;
};
static INLINE struct D3D11Device *D3D11Device(void *ptr)
{
    return (struct D3D11Device *)ptr;
}

HRESULT
D3D11Device_new( struct D3D11Device *, struct D3D11Device **ppOut );

HRESULT WINAPI
D3D11Device_CreateBuffer( struct D3D11Device *This,
                          D3D11_BUFFER_DESC *pDesc,
                          D3D11_SUBRESOURCE_DATA *pInitialData,
                          ID3D11Buffer **ppBuffer );

HRESULT WINAPI
D3D11Device_CreateTexture1D( struct D3D11Device *This,
                             D3D11_TEXTURE1D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture1D **ppTexture1D );

HRESULT WINAPI
D3D11Device_CreateTexture2D( struct D3D11Device *This,
                             D3D11_TEXTURE2D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture2D **ppTexture2D );

HRESULT WINAPI
D3D11Device_CreateTexture3D( struct D3D11Device *This,
                             D3D11_TEXTURE3D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture3D **ppTexture3D );

HRESULT WINAPI
D3D11Device_CreateShaderResourceView( struct D3D11Device *This,
                                      ID3D11Resource *pResource,
                                      D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
                                      ID3D11ShaderResourceView **ppSRView );

HRESULT WINAPI
D3D11Device_CreateUnorderedAccessView( struct D3D11Device *This,
                                       ID3D11Resource *pResource,
                                       D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
                                       ID3D11UnorderedAccessView **ppUAView );

HRESULT WINAPI
D3D11Device_CreateRenderTargetView( struct D3D11Device *This,
                                    ID3D11Resource *pResource,
                                    D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
                                    ID3D11RenderTargetView **ppRTView );

HRESULT WINAPI
D3D11Device_CreateDepthStencilView( struct D3D11Device *This,
                                    ID3D11Resource *pResource,
                                    D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
                                    ID3D11DepthStencilView **ppDepthStencilView );

HRESULT WINAPI
D3D11Device_CreateInputLayout( struct D3D11Device *This,
                               D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
                               UINT NumElements,
                               void *pShaderBytecodeWithInputSignature,
                               Int BytecodeLength,
                               ID3D11InputLayout **ppInputLayout );

HRESULT WINAPI
D3D11Device_CreateVertexShader( struct D3D11Device *This,
                                void *pShaderBytecode,
                                Int BytecodeLength,
                                ID3D11ClassLinkage *pClassLinkage,
                                ID3D11VertexShader **ppVertexShader );

HRESULT WINAPI
D3D11Device_CreateGeometryShader( struct D3D11Device *This,
                                  void *pShaderBytecode,
                                  Int BytecodeLength,
                                  ID3D11ClassLinkage *pClassLinkage,
                                  ID3D11GeometryShader **ppGeometryShader );

HRESULT WINAPI
D3D11Device_CreateGeometryShaderWithStreamOutput( struct D3D11Device *This,
                                                  void *pShaderBytecode,
                                                  Int BytecodeLength,
                                                  D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
                                                  UINT NumEntries,
                                                  UINT *pBufferStrides,
                                                  UINT NumStrides,
                                                  UINT RasterizedStream,
                                                  ID3D11ClassLinkage *pClassLinkage,
                                                  ID3D11GeometryShader **ppGeometryShader );

HRESULT WINAPI
D3D11Device_CreatePixelShader( struct D3D11Device *This,
                               void *pShaderBytecode,
                               Int BytecodeLength,
                               ID3D11ClassLinkage *pClassLinkage,
                               ID3D11PixelShader **ppPixelShader );

HRESULT WINAPI
D3D11Device_CreateHullShader( struct D3D11Device *This,
                              void *pShaderBytecode,
                              Int BytecodeLength,
                              ID3D11ClassLinkage *pClassLinkage,
                              ID3D11HullShader **ppHullShader );

HRESULT WINAPI
D3D11Device_CreateDomainShader( struct D3D11Device *This,
                                void *pShaderBytecode,
                                Int BytecodeLength,
                                ID3D11ClassLinkage *pClassLinkage,
                                ID3D11DomainShader **ppDomainShader );

HRESULT WINAPI
D3D11Device_CreateComputeShader( struct D3D11Device *This,
                                 void *pShaderBytecode,
                                 Int BytecodeLength,
                                 ID3D11ClassLinkage *pClassLinkage,
                                 ID3D11ComputeShader **ppComputeShader );

HRESULT WINAPI
D3D11Device_CreateClassLinkage( struct D3D11Device *This,
                                ID3D11ClassLinkage **ppLinkage );

HRESULT WINAPI
D3D11Device_CreateBlendState( struct D3D11Device *This,
                              D3D11_BLEND_DESC *pBlendStateDesc,
                              ID3D11BlendState **ppBlendState );

HRESULT WINAPI
D3D11Device_CreateDepthStencilState( struct D3D11Device *This,
                                     D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
                                     ID3D11DepthStencilState **ppDepthStencilState );

HRESULT WINAPI
D3D11Device_CreateRasterizerState( struct D3D11Device *This,
                                   D3D11_RASTERIZER_DESC *pRasterizerDesc,
                                   ID3D11RasterizerState **ppRasterizerState );

HRESULT WINAPI
D3D11Device_CreateSamplerState( struct D3D11Device *This,
                                D3D11_SAMPLER_DESC *pSamplerDesc,
                                ID3D11SamplerState **ppSamplerState );

HRESULT WINAPI
D3D11Device_CreateQuery( struct D3D11Device *This,
                         D3D11_QUERY_DESC *pQueryDesc,
                         ID3D11Query **ppQuery );

HRESULT WINAPI
D3D11Device_CreatePredicate( struct D3D11Device *This,
                             D3D11_QUERY_DESC *pPredicateDesc,
                             ID3D11Predicate **ppPredicate );

HRESULT WINAPI
D3D11Device_CreateCounter( struct D3D11Device *This,
                           D3D11_COUNTER_DESC *pCounterDesc,
                           ID3D11Counter **ppCounter );

HRESULT WINAPI
D3D11Device_CreateDeferredContext( struct D3D11Device *This,
                                   UINT ContextFlags,
                                   ID3D11DeviceContext **ppDeferredContext );

HRESULT WINAPI
D3D11Device_OpenSharedResource( struct D3D11Device *This,
                                HANDLE hResource,
                                REFIID ReturnedInterface,
                                void **ppResource );

HRESULT WINAPI
D3D11Device_CheckFormatSupport( struct D3D11Device *This,
                                Int Format,
                                UINT *pFormatSupport );

HRESULT WINAPI
D3D11Device_CheckMultisampleQualityLevels( struct D3D11Device *This,
                                           Int Format,
                                           UINT SampleCount,
                                           UINT *pNumQualityLevels );

void WINAPI
D3D11Device_CheckCounterInfo( struct D3D11Device *This,
                              Int *pCounterInfo );

HRESULT WINAPI
D3D11Device_CheckCounter( struct D3D11Device *This,
                          D3D11_COUNTER_DESC *pDesc,
                          D3D11_COUNTER_TYPE *pType,
                          UINT *pActiveCounters,
                          Int szName,
                          UINT *pNameLength,
                          Int szUnits,
                          UINT *pUnitsLength,
                          Int szDescription,
                          UINT *pDescriptionLength );

HRESULT WINAPI
D3D11Device_CheckFeatureSupport( struct D3D11Device *This,
                                 D3D11_FEATURE Feature,
                                 void *pFeatureSupportData,
                                 UINT FeatureSupportDataSize );

HRESULT WINAPI
D3D11Device_GetPrivateData( struct D3D11Device *This,
                            REFGUID guid,
                            UINT *pDataSize,
                            void *pData );

HRESULT WINAPI
D3D11Device_SetPrivateData( struct D3D11Device *This,
                            REFGUID guid,
                            UINT DataSize,
                            void *pData );

HRESULT WINAPI
D3D11Device_SetPrivateDataInterface( struct D3D11Device *This,
                                     REFGUID guid,
                                     IUnknown *pData );

D3D11Device_(  );

UINT WINAPI
D3D11Device_GetCreationFlags( struct D3D11Device *This );

HRESULT WINAPI
D3D11Device_GetDeviceRemovedReason( struct D3D11Device *This );

void WINAPI
D3D11Device_GetImmediateContext( struct D3D11Device *This,
                                 ID3D11DeviceContext **ppImmediateContext );

HRESULT WINAPI
D3D11Device_SetExceptionMode( struct D3D11Device *This,
                              UINT RaiseFlags );

UINT WINAPI
D3D11Device_GetExceptionMode( struct D3D11Device *This );

#endif /* _D3D11_DEVICE_H_ */

