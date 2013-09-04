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

#include "device.h"

static unsigned caps_d3d9_3[] =
{
    UTIL_CHECK_INT(MAX_RENDER_TARGETS, 4),
    UTIL_CHECK_CAP(MIXED_COLORBUFFER_FORMATS),
    UTIL_CHECK_CAP(BLEND_EQUATION_SEPARATE),
    UTIL_CHECK_CAP(INDEP_BLEND_ENABLE),
    UTIL_CHECK_CAP(TWO_SIDED_STENCIL),
    UTIL_CHECK_INT(MAX_TEXTURE_2D_LEVELS, 12),
    UTIL_CHECK_INT(MAX_TEXTURE_CUBE_LEVELS, 12),
    UTIL_CHECK_INT(MAX_TEXTURE_3D_LEVELS, 8),
    UTIL_CHECK_CAP(ANISOTROPIC_FILTER),
    UTIL_CHECK_FLOAT(MAX_TEXTURE_ANISOTROPY, 16.0f),
    UTIL_CHECK_CAP(TEXTURE_MIRROR_CLAMP),
    UTIL_CHECK_CAP(VERTEX_ELEMENT_INSTANCE_DIVISOR),
    UTIL_CHECK_CAP(OCCLUSION_QUERY),
    UTIL_CHECK_CAP(SM3),
    UTIL_CHECK_SHADER(VERTEX, MAX_INPUTS, 16),
    UTIL_CHECK_TERMINATE
};
static unsigned caps_d3d10_0[] =
{
    /* OM */
    UTIL_CHECK_INT(MAX_RENDER_TARGETS, 8),
    UTIL_CHECK_INT(MAX_DUAL_SOURCE_RENDER_TARGETS, 1),
    UTIL_CHECK_CAP(INDEP_BLEND_FUNC),
    /* RS */
    UTIL_CHECK_INT(MAX_VIEWPORTS, 16),
    UTIL_CHECK_CAP(DEPTH_CLIP_DISABLE),
    /* TEX */
    UTIL_CHECK_INT(MAX_TEXTURE_2D_LEVELS, 13),
    UTIL_CHECK_INT(MAX_TEXTURE_CUBE_LEVELS, 13),
    UTIL_CHECK_INT(MAX_TEXTURE_3D_LEVELS, 11),
    UTIL_CHECK_INT(MAX_TEXTURE_ARRAY_LAYERS, 512),
    UTIL_CHECK_INT(MAX_TEXTURE_BUFFER_SIZE, 2048),
    UTIL_CHECK_CAP(CUBE_MAP_ARRAY),
    UTIL_CHECK_CAP(TEXTURE_BUFFER_OBJECTS),
    UTIL_CHECK_CAP(TEXTURE_MULTISAMPLE),
    UTIL_CHECK_CAP(TEXTURE_SHADOW_MAP),
    UTIL_CHECK_CAP(SEAMLESS_CUBE_MAP),
    /* SO */
    UTIL_CHECK_INT(MAX_STREAM_OUTPUT_BUFFERS, 4),
    UTIL_CHECK_CAP(STREAM_OUTPUT_PAUSE_RESUME),
    /* SHADER */
    UTIL_CHECK_INT(GLSL_FEATURE_LEVEL, 150),
    UTIL_CHECK_INT(MAX_TEXEL_OFFSET, 7),
    UTIL_CHECK_CAP(TGSI_INSTANCEID),
    UTIL_CHECK_SHADER(VERTEX, MAX_CONTROL_FLOW_DEPTH, 64),
    UTIL_CHECK_SHADER(VERTEX, MAX_CONST_BUFFERS, 14),
    UTIL_CHECK_SHADER(VERTEX, MAX_TEXTURE_SAMPLERS, 16),
    UTIL_CHECK_SHADER(GEOMETRY, MAX_INPUTS, 16),
    UTIL_CHECK_SHADER(FRAGMENT, MAX_INPUTS, 32),
    UTIL_CHECK_SHADER(FRAGMENT, INTEGERS, 1),
    /* DRAW */
    UTIL_CHECK_CAP(CONDITIONAL_RENDER),
    UTIL_CHECK_CAP(PRIMITIVE_RESTART),
    UTIL_CHECK_CAP(START_INSTANCE),
    /* QUERY */
    UTIL_CHECK_CAP(QUERY_TIMESTAMP),
    UTIL_CHECK_CAP(QUERY_PIPELINE_STATISTICS),
    UTIL_CHECK_TERMINATE
};
static unsigned caps_d3d11_0[] =
{
    UTIL_CHECK_INT(MAX_TEXTURE_2D_LEVELS, 14),
    UTIL_CHECK_INT(MAX_TEXTURE_CUBE_LEVELS, 14),
    UTIL_CHECK_INT(MAX_TEXTURE_ARRAY_LAYERS, 2048),
    UTIL_CHECK_INT(GLSL_FEATURE_LEVEL, 400),
    UTIL_CHECK_SHADER(VERTEX, MAX_CONST_BUFFERS, 15),
    UTIL_CHECK_SHADER(VERTEX, MAX_INPUTS, 32),
    UTIL_CHECK_TERMINATE
};

static boolean
D3D11Device_DetermineFeatureLevel( struct D3D11Device *This )
{
    if (!util_check_caps(This->screen, caps_d3d9_3))
        return FALSE;
    if (!util_check_caps(This->screen, caps_d3d10_0))
        This->feature_level = D3D_FEATURE_LEVEL_9_3;
    else
    if (!util_check_caps(This->screen, caps_d3d11_0))
        This->feature_level = D3D_FEATURE_LEVEL_10_0;
    else
        This->feature_level = D3D_FEATURE_LEVEL_11_0;
    return TRUE;
}

HRESULT
D3D11Device_ctor( struct D3D11Device *This,
                  struct D3D11UnknownParams *pParams )
{
    HRESULT hr = Unknown_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    if (!D3D11Device_DetermineFeatureLevel(This))
        return_error(DXGI_ERROR_UNSUPPORTED);

    hr = D3D11DeviceInit_BlendState(This);
    if (FAILED(hr))
        return hr;
    hr = D3D11DeviceInit_DepthStencilState(This);
    if (FAILED(hr))
        return hr;
    hr = D3D11DeviceInit_RasterizerState(This);
    if (FAILED(hr))
        return hr;
    hr = D3D11DeviceInit_SamplerState(This);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11Device_dtor( struct D3D11Device *This )
{
    Unknown_dtor(&This->base);
}

HRESULT WINAPI
D3D11Device_CreateBuffer( struct D3D11Device *This,
                          D3D11_BUFFER_DESC *pDesc,
                          D3D11_SUBRESOURCE_DATA *pInitialData,
                          ID3D11Buffer **ppBuffer )
{
    struct D3D11Buffer *pBuf;
    HRESULT hr;

    if (!pInitialData && pDesc->Usage == D3D11_USAGE_IMMUTABLE)
        return_error(E_INVALIDARG);

    hr = D3D11Buffer_new(This, pDesc, &pBuf);
    if (FAILED(hr))
        return_error(hr);
    *ppBuffer = (ID3D11Buffer *)pBuf;

    if (pInitialData) {
        struct pipe_context *pipe = This->pipe2;
        struct pipe_box box;
        u_box_1d(0, pDesc->ByteWidth, &box);
        pipe->transfer_inline_write(pipe, pBuf->base.resource, 0,
                                    PIPE_TRANSFER_UNSYNCHRONIZED,
                                    &box,
                                    pInitialData->pSysMem, 0, 0);
    }
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateTexture1D( struct D3D11Device *This,
                             D3D11_TEXTURE1D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture1D **ppTexture1D )
{
    struct D3D11Texture1D *pTex;
    HRESULT hr;

    if (!pInitialData && pDesc->Usage == D3D11_USAGE_IMMUTABLE)
        return_error(E_INVALIDARG);

    hr = D3D11Texture1D_new(This, pDesc, &pTex);
    if (FAILED(hr))
        return_error(hr);
    *ppTexture1D = (ID3D11Texture1D *)pTex;

    if (pInitialData) {
        struct pipe_context *pipe = This->pipe2;
        unsigned l, z;
        struct pipe_box box;
        u_box_1d(0, pDesc->Width, &box);
        for (box.z = 0; box.z < pTex->base.resource->array_size; ++box.z) {
            for (l = 0; l <= pTex->base.resource->last_level; ++l) {
                pipe->transfer_inline_write(pipe, pBuf->base.resource, l,
                                            PIPE_TRANSFER_UNSYNCHRONIZED,
                                            &box,
                                            pInitialData->pSysMem, 0, 0);
                ++pInitialData;
            }
        }
    }
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateTexture2D( struct D3D11Device *This,
                             D3D11_TEXTURE2D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture2D **ppTexture2D )
{
    struct D3D11Texture2D *pTex;
    HRESULT hr;

    if (pInitialData && pDesc->SampleDesc.Count > 1)
        return_error(E_INVALIDARG);
    if (!pInitialData && pDesc->Usage == D3D11_USAGE_IMMUTABLE)
        return_error(E_INVALIDARG);

    hr = D3D11Texture2D_new(This, pDesc, &pTex);
    if (FAILED(hr))
        return_error(hr);
    *ppTexture2D = (ID3D11Texture2D *)pTex;

    if (pInitialData) {
        struct pipe_context *pipe = This->pipe2;
        unsigned l, z;
        struct pipe_box box;
        u_box_2d(0, 0, pDesc->Width, pDesc->Height, &box);
        for (box.z = 0; box.z < pTex->base.resource->array_size; ++box.z) {
            for (l = 0; l <= pTex->base.resource->last_level; ++l) {
                pipe->transfer_inline_write(pipe, pBuf->base.resource, l,
                                            PIPE_TRANSFER_UNSYNCHRONIZED,
                                            &box,
                                            pInitialData->pSysMem,
                                            pInitialData->SysMemPitch, 0);
                ++pInitialData;
            }
        }
    }
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateTexture3D( struct D3D11Device *This,
                             D3D11_TEXTURE3D_DESC *pDesc,
                             D3D11_SUBRESOURCE_DATA *pInitialData,
                             ID3D11Texture3D **ppTexture3D )
{
    struct D3D11Texture3D *pTex;
    HRESULT hr;

    if (!pInitialData && pDesc->Usage == D3D11_USAGE_IMMUTABLE)
        return_error(E_INVALIDARG);

    hr = D3D11Texture3D_new(This, pDesc, &pTex);
    if (FAILED(hr))
        return_error(hr);
    *ppTexture3D = (ID3D11Texture3D *)pTex;

    if (pInitialData) {
        struct pipe_context *pipe = This->pipe2;
        unsigned l;
        struct pipe_box box;
        u_box_3d(0, 0, 0, pDesc->Width, pDesc->Height, pDesc->Depth, &box);
        for (l = 0; l <= pTex->base.resource->last_level; ++l) {
            pipe->transfer_inline_write(pipe, pBuf->base.resource, l,
                                        PIPE_TRANSFER_UNSYNCHRONIZED,
                                        &box,
                                        pInitialData->pSysMem,
                                        pInitialData->SysMemPitch,
                                        pInitialData->SysMemSlicePitch);
            ++pInitialData;
        }
    }
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateShaderResourceView( struct D3D11Device *This,
                                      ID3D11Resource *pResource,
                                      D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
                                      ID3D11ShaderResourceView **ppSRView )
{
    struct D3D11ShaderResourceView *pSRV;
    HRESULT hr;

    hr = D3D11ShaderResourceView_new(This, D3D11Resource(pResource), pDesc, pSRV);
    if (FAILED(hr))
        return_error(hr);
    *ppSRView = (ID3D11ShaderResourceView *)pSRV;
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateUnorderedAccessView( struct D3D11Device *This,
                                       ID3D11Resource *pResource,
                                       D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
                                       ID3D11UnorderedAccessView **ppUAView )
{
    struct D3D11UnorderedAccessView *pUAV;
    HRESULT hr;

    hr = D3D11UnorderedAccessView_new(This, D3D11Resource(pResource), pDesc, pUAV);
    if (FAILED(hr))
        return_error(hr);
    *ppUAView = (ID3D11UnorderedAccessView *)pUAV;
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateRenderTargetView( struct D3D11Device *This,
                                    ID3D11Resource *pResource,
                                    D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
                                    ID3D11RenderTargetView **ppRTView )
{
    struct D3D11RenderTargetView *pRTV;
    HRESULT hr;

    hr = D3D11RenderTargetView_new(This, D3D11Resource(pResource), pDesc, pRTV);
    if (FAILED(hr))
        return_error(hr);
    *ppRTView = (ID3D11RenderTargetView *)pRTV;
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateDepthStencilView( struct D3D11Device *This,
                                    ID3D11Resource *pResource,
                                    D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
                                    ID3D11DepthStencilView **ppDepthStencilView )
{
    struct D3D11DepthstencilView *pDSV;
    HRESULT hr;

    hr = D3D11DepthStencilView_new(This, D3D11Resource(pResource), pDesc, pDSV);
    if (FAILED(hr))
        return_error(hr);
    *ppDepthStencilView = (ID3D11DepthStencilView *)pDSV;
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateInputLayout( struct D3D11Device *This,
                               D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
                               UINT NumElements,
                               void *pShaderBytecodeWithInputSignature,
                               Int BytecodeLength,
                               ID3D11InputLayout **ppInputLayout )
{
    struct D3D11InputLayout *pLayout;
    HRESULT hr;

    hr = D3D11InputLayout_new(This, pInputElementDescs, NumElements,
                              pShaderBytecodeWithInputSignature, BytecodeLength,
                              &pLayout);
    if (FAILED(hr))
        return_error(hr);
    *ppInputLayout = (ID3D11InputLayout *)pLayout;
    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateVertexShader( struct D3D11Device *This,
                                void *pShaderBytecode,
                                Int BytecodeLength,
                                ID3D11ClassLinkage *pClassLinkage,
                                ID3D11VertexShader **ppVertexShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateGeometryShader( struct D3D11Device *This,
                                  void *pShaderBytecode,
                                  Int BytecodeLength,
                                  ID3D11ClassLinkage *pClassLinkage,
                                  ID3D11GeometryShader **ppGeometryShader )
{
    STUB_return(E_NOTIMPL);
}

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
                                                  ID3D11GeometryShader **ppGeometryShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreatePixelShader( struct D3D11Device *This,
                               void *pShaderBytecode,
                               Int BytecodeLength,
                               ID3D11ClassLinkage *pClassLinkage,
                               ID3D11PixelShader **ppPixelShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateHullShader( struct D3D11Device *This,
                              void *pShaderBytecode,
                              Int BytecodeLength,
                              ID3D11ClassLinkage *pClassLinkage,
                              ID3D11HullShader **ppHullShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateDomainShader( struct D3D11Device *This,
                                void *pShaderBytecode,
                                Int BytecodeLength,
                                ID3D11ClassLinkage *pClassLinkage,
                                ID3D11DomainShader **ppDomainShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateComputeShader( struct D3D11Device *This,
                                 void *pShaderBytecode,
                                 Int BytecodeLength,
                                 ID3D11ClassLinkage *pClassLinkage,
                                 ID3D11ComputeShader **ppComputeShader )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateClassLinkage( struct D3D11Device *This,
                                ID3D11ClassLinkage **ppLinkage )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CreateBlendState( struct D3D11Device *This,
                              D3D11_BLEND_DESC *pBlendStateDesc,
                              ID3D11BlendState **ppBlendState )
{
    struct D3D11BlendState *bs;
    HRESULT hr;

    if (unlikely(!pBlendStateDesc)) {
        com_set(ppBlendState, This->bs_default);
        return S_OK;
    }

    bs = util_hash_table_get(This->ht_bs, pBlendStateDesc);
    if (!bs) {
        if (This->bs_count >= D3D11_MAX_UNIQUE_STATE_OBJECT_COUNT)
            return_error(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);

        hr = D3D11BlendState_new(This, pBlendStateDesc, &bs);
        if (FAILED(hr))
            return_error(hr);
    }
    com_set(ppBlendState, bs);

    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateDepthStencilState( struct D3D11Device *This,
                                     D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
                                     ID3D11DepthStencilState **ppDepthStencilState )
{
    struct D3D11DepthStancilState *ds;
    HRESULT hr;

    if (unlikely(!pDepthStencilDesc)) {
        com_set(ppDepthStencilState, This->ds_default);
        return S_OK;
    }

    ds = util_hash_table_get(This->ht_ds, pDepthStencilDesc);
    if (!ds) {
        if (This->ds_count >= D3D11_MAX_UNIQUE_STATE_OBJECT_COUNT)
            return_error(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);

        hr = D3D11DepthStencilState_new(This, pDepthStencilDesc, &ds);
        if (FAILED(hr))
            return_error(hr);
    }
    com_set(ppDepthStencilState, ds);

    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateRasterizerState( struct D3D11Device *This,
                                   D3D11_RASTERIZER_DESC *pRasterizerDesc,
                                   ID3D11RasterizerState **ppRasterizerState )
{
    struct D3D11RasterizerState *rs;
    HRESULT hr;

    if (unlikely(!pRasterizerDesc)) {
        com_set(ppRasterizerState, This->rs_default);
        return S_OK;
    }

    rs = util_hash_table_get(This->ht_rs, pRasterizerDesc);
    if (!rs) {
        if (This->rs_count >= D3D11_MAX_UNIQUE_STATE_OBJECT_COUNT)
            return_error(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);

        hr = D3D11RasterizerState_new(This, pRasterizerDesc, &rs);
        if (FAILED(hr))
            return_error(hr);
    }
    com_set(ppRasterizerState, rs);

    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateSamplerState( struct D3D11Device *This,
                                D3D11_SAMPLER_DESC *pSamplerDesc,
                                ID3D11SamplerState **ppSamplerState )
{
    struct D3D11SamplerState *ss;
    HRESULT hr;

    if (unlikely(!pSamplerDesc)) {
        com_set(ppSamplerState, This->ss_default);
        return S_OK;
    }

    ss = util_hash_table_get(This->ht_ss, pSamplerDesc);
    if (!ss) {
        if (This->ss_count >= D3D11_MAX_UNIQUE_STATE_OBJECT_COUNT)
            return_error(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);

        hr = D3D11SamplerState_new(This, pSamplerDesc, &ss);
        if (FAILED(hr))
            return_error(hr);
    }
    com_set(ppSamplerState, ss);

    return S_OK;
}

HRESULT WINAPI
D3D11Device_CreateQuery( struct D3D11Device *This,
                         D3D11_QUERY_DESC *pQueryDesc,
                         ID3D11Query **ppQuery )
{
    return D3D11Query_new(This, pQueryDesc, (struct D3D11Query **)ppQuery);
}

HRESULT WINAPI
D3D11Device_CreatePredicate( struct D3D11Device *This,
                             D3D11_QUERY_DESC *pPredicateDesc,
                             ID3D11Predicate **ppPredicate )
{
    return D3D11Predicate_new(This, pPredicateDesc,
                              (struct D3D11Predicate **)ppPredicate);
}

HRESULT WINAPI
D3D11Device_CreateCounter( struct D3D11Device *This,
                           D3D11_COUNTER_DESC *pCounterDesc,
                           ID3D11Counter **ppCounter )
{
    return D3D11Counter_new(This, pCounterDesc, (struct D3D11Counter **)ppCounter);
}

HRESULT WINAPI
D3D11Device_CreateDeferredContext( struct D3D11Device *This,
                                   UINT ContextFlags,
                                   ID3D11DeviceContext **ppDeferredContext )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_OpenSharedResource( struct D3D11Device *This,
                                HANDLE hResource,
                                REFIID ReturnedInterface,
                                void **ppResource )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11Device_CheckFormatSupport( struct D3D11Device *This,
                                Int Format,
                                UINT *pFormatSupport )
{
    struct pipe_screen *screen = This->screen;
    const enum pipe_format pf = dxgi_to_pipe_format(Format);
    UINT mask = 0;

    if (!pFormatSupport)
        return_error(E_POINTER);

    FSUP(BUFFER, 0, SAMPLER_VIEW, BUFFER);
    FSUP(BUFFER, 0, VERTEX_BUFFER, IA_VERTEX_BUFFER);
    FSUP(BUFFER, 0, INDEX_BUFFER, IA_INDEX_BUFFER);
    FSUP(BUFFER, 0, STREAM_OUTPUT, SO_BUFFER);
    FSUP(TEXTURE_1D, 0, SAMPLER_VIEW, TEXTURE1D);
    FSUP(TEXTURE_2D, 0, SAMPLER_VIEW, TEXTURE2D);
    FSUP(TEXTURE_3D, 0, SAMPLER_VIEW, TEXTURE3D);
    FSUP(TEXTURE_CUBE, 0, SAMPLER_VIEW, TEXTURECUBE);

    if (mask & D3D11_FORMAT_SUPPORT_TEXTURE2D) {
       mask |=
          D3D11_FORMAT_SUPPORT_SHADER_LOAD |
          D3D11_FORMAT_SUPPORT_SHADER_SAMPLE |
          D3D11_FORMAT_SUPPORT_MIP |
          D3D11_FORMAT_SUPPORT_MIP_AUTOGEN |
          D3D11_FORMAT_SUPPORT_SHADER_GATHER |
       if (util_format_is_depth_or_stencil(pf)) {
          mask |=
             D3D11_FORMAT_SUPPORT_SHADER_SAMPLE_COMPARISON |
             D3D11_FORMAT_SUPPORT_SHADER_GATHER_COMPARISON;
       }
    }

    if (1)
       mask |=
          D3D11_FORMAT_SUPPORT_CAST_WITHIN_BIT_LAYOUT |
          D3D11_FORMAT_SUPPORT_BACK_BUFFER_CAST;

    FSUP(TEXTURE_2D, 0, RENDER_TARGET, RENDER_TARGET);
    FSUP(TEXTURE_2D, 0, RENDER_TARGET | PIPE_BIND_BLENDABLE, BLENDABLE);
    FSUP(TEXTURE_2D, 0, DEPTH_STENCIL, DEPTH_STENCIL);
    FSUP(TEXTURE_2D, 0, RENDER_TARGET | PIPE_BIND_DISPLAY_TARGET, DISPLAY);
    FSUP(TEXTURE_2D, 0, SHADER_RESOURCE, TYPED_UNORDERED_ACCESS_VIEW);

    FSUP(TEXTURE_2D, 2, RENDER_TARGET, MULTISAMPLE_RENDERTARGET);
    if (mask & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RENDERTARGET)
       if (GET_PCAP(TEXTURE_MULTISAMPLE))
          mask |= D3D11_FORMAT_SUPPORT_MULTISAMPLE_LOAD;

    mask |= D3D11_FORMAT_SUPPORT_CPU_LOCKABLE;
    mask |= D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE;

    *pFormatSupport = mask;

    return S_OK;
}

HRESULT WINAPI
D3D11Device_CheckMultisampleQualityLevels( struct D3D11Device *This,
                                           Int Format,
                                           UINT SampleCount,
                                           UINT *pNumQualityLevels )
{
    enum pipe_format pf = dxgi_to_pipe_format(Format);
    unsigned bind;

    if (SampleCount > 32)
        return_error(E_INVALIDARG);

    if (!pNumQualityLevels)
        return_error(E_POINTER);

    bind = util_format_is_depth_or_stencil(pf) ?
        PIPE_BIND_DEPTH_STENCIL :
        PIPE_BIND_RENDER_TARGET;

    if (This->screen->is_format_supported(This->screen, pf, PIPE_TEXTURE_2D,
                                          SampleCount, bind))
        *pNumQualityLevels = 1;
    else
        *pNumQualityLevels = 0;

    return S_OK;
}

#define D3D11_FEATURE_CASE(n, m) case D3D11_FEATURE_##n: \
    if (FeatureSupportDataSize < sizeof(D3D11_FEATURE_DATA_##n)) \
        return E_INVALIDARG;
HRESULT WINAPI
D3D11Device_CheckFeatureSupport( struct D3D11Device *This,
                                 D3D11_FEATURE Feature,
                                 void *pFeatureSupportData,
                                 UINT FeatureSupportDataSize )
{
   struct pipe_screen *screen = This->screen;
   union {
      D3D11_FEATURE_DATA_THREADING t;
      D3D11_FEATURE_DATA_DOUBLES d;
      D3D11_FEATURE_DATA_FORMAT_SUPPORT f;
      D3D11_FEATURE_DATA_FORMAT_SUPPORT2 f2;
      D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS o10_x;
      D3D11_FEATURE_DATA_D3D11_OPTIONS o11_0;
      D3D11_FEATURE_DATA_ARCHITECTURE_INFO a;
      D3D11_FEATURE_DATA_D3D9_OPTIONS o9;
      D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT p;
      D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT s9;
      D3D11_FEATURE_DATA_D3D11_OPTIONS1 o11_1;
      D3D11_FEATURE_DATA_D3D9_SIMPLE_INSTANCING_SUPPORT i9;
      D3D11_FEATURE_DATA_MARKER_SUPPORT m;
   } *u = pFeatureSupportData;

   switch (Feature) {
   D3D11_FEATURE_CASE(THREADING)
       u->t.DriverConcurrentCreates = FALSE;
       u->t.DriverCommandLists = FALSE;
       break;
   D3D11_FEATURE_CASE(DOUBLES)
       u->d.DoublePrecisionFloatShaderOps = FALSE; /* TODO */
       break;
   D3D11_FEATURE_CASE(FORMAT_SUPPORT)
       return D3D11Device_CheckFormatSupport(This, u->f.InFormat, &u->f.OutFormatSupport);
   D3D11_FEATURE_CASE(FORMAT_SUPPORT2)
       return D3D11Device_CheckFormatSupport2(This, u->f.InFormat, &u->f.OutFormatSupport2);
   D3D11_FEATURE_CASE(D3D10_X_HARDWARE_OPTIONS)
       u->o10_x.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x = GET_PCAP(COMPUTE);
       break;
   /* NOTE: not honouring contraints listed on MSDN */
   D3D11_FEATURE_CASE(D3D11_OPTIONS)
       u->o11_0.OutputMergerLogicOp = TRUE;
       u->o11_0.UAVOnlyRenderingForcedSampleCount = TRUE; /* always true in 11.1 */
       u->o11_0.DiscardAPIsSeenByDriver = TRUE;
       u->o11_0.FlagsForUpdateAndCopySeenByDriver = TRUE;
       u->o11_0.ClearView = TRUE;
       u->o11_0.CopyWithOverlap = TRUE;
       u->o11_0.ConstantBufferPartialUpdate = TRUE; /* TODO: should this be false on r600 ? */
       u->o11_0.ConstantBufferOffsetting = !!GET_PCAP(CONSTANT_BUFFER_OFFSET_ALIGNMENT);
       u->o11_0.MapNoOverwriteOnDynamicConstantBuffer = TRUE;
       u->o11_0.MapNoOverwriteOnDynamicBufferSRV = TRUE;
       u->o11_0.MultisampleRTVWithForcedSampleCountOne = TRUE;
       u->o11_0.SAD4ShaderInstructions = FALSE; /* TODO */
       u->o11_0.ExtendedDoublesShaderInstructions = FALSE; /* TODO: f64 */
       u->o11_0.ExtendedResourceSharing = FALSE;
       break;
   D3D11_FEATURE_CASE(ARCHITECTURE_INFO)
       u->a.TileBasedDeferredRenderer = TRUE; /* TODO: query driver */
       break;
   D3D11_FEATURE_CASE(D3D9_OPTIONS)
       u->o9.FullNonPow2TextureSupport = GET_PCAP(NPOT_TEXTURES);
       break;
   D3D11_FEATURE_CASE(SHADER_MIN_PRECISION_SUPPORT)
       /* 32-bit precision only */
       u->p.PixelShaderMinPrecision = 0;
       u->p.AllOtherShaderStagesMinPrecision = 0;
       break;
   D3D11_FEATURE_CASE(D3D9_SHADOW_SUPPORT)
       u->s9.SupportsDepthAsTextureWithLessEqualComparisonFilter = GET_PCAP(TEXTURE_SHADOW_MAP);
       break;
   D3D11_FEATURE_CASE(D3D11_OPTIONS1)
       u->o11_1.TiledResourcesTier = D3D11_TILED_RESOURCES_NOT_SUPPORTED; /* TODO */
       u->o11_1.MinMaxFiltering = FALSE; /* TODO */
       u->o11_1.ClearViewAlsoSupportsDepthOnlyFormats = TRUE;
       u->o11_1.MapOnDefaultBuffers = TRUE;
       break;
   D3D11_FEATURE_CASE(D3D9_SIMPLE_INSTANCING_SUPPORT)
       u->i9.SimpleInstancingSupported = GET_PCAP(VERTEX_ELEMENT_INSTANCE_DIVISOR);
       break;
   D3D11_FEATURE_CASE(MARKER_SUPPORT)
       u->m.Profile = FALSE;
       break;
   default:
      return_error(E_INVALIDARG);
   }
   return S_OK;
}

HRESULT WINAPI
D3D11Device_GetPrivateData( struct D3D11Device *This,
                            REFGUID guid,
                            UINT *pDataSize,
                            void *pData )
{
    return D3D11PrivateData_Get(&This->pdata, guid, pDataSize, pData);
}

HRESULT WINAPI
D3D11Device_SetPrivateData( struct D3D11Device *This,
                            REFGUID guid,
                            UINT DataSize,
                            void *pData )
{
    return D3D11PrivateData_Set(&This->pdata, guid, pDataSize, pData);
}

HRESULT WINAPI
D3D11Device_SetPrivateDataInterface( struct D3D11Device *This,
                                     REFGUID guid,
                                     IUnknown *pData )
{
    return D3D11PrivateData_SetInterface(This, guid, pData);
}

D3D_FEATURE_LEVEL
D3D11Device_GetFeatureLevel( struct D3D11Device *This )
{
    return This->feature_level;
}

UINT WINAPI
D3D11Device_GetCreationFlags( struct D3D11Device *This )
{
    return This->creation_flags;
}

HRESULT WINAPI
D3D11Device_GetDeviceRemovedReason( struct D3D11Device *This )
{
    return This->fatal_reason;
}

void WINAPI
D3D11Device_GetImmediateContext( struct D3D11Device *This,
                                 ID3D11DeviceContext **ppImmediateContext )
{
    assert(ppImmediateContext);
    com_set(ppImmediateContext, This->immediate_context);
}

HRESULT WINAPI
D3D11Device_SetExceptionMode( struct D3D11Device *This,
                              UINT RaiseFlags )
{
    if (RaiseFlags & ~(D3D11_RAISE_FLAG_DRIVER_INTERNAL_ERROR |
                       0))
        return_error(E_INVALIDARG);

    This->exceptions = RaiseFlags;

    return S_OK;
}

UINT WINAPI
D3D11Device_GetExceptionMode( struct D3D11Device *This )
{
    return This->exceptions;
}

ID3D11DeviceVtbl D3D11Device_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11Device_CreateBuffer,
    (void *)D3D11Device_CreateTexture1D,
    (void *)D3D11Device_CreateTexture2D,
    (void *)D3D11Device_CreateTexture3D,
    (void *)D3D11Device_CreateShaderResourceView,
    (void *)D3D11Device_CreateUnorderedAccessView,
    (void *)D3D11Device_CreateRenderTargetView,
    (void *)D3D11Device_CreateDepthStencilView,
    (void *)D3D11Device_CreateInputLayout,
    (void *)D3D11Device_CreateVertexShader,
    (void *)D3D11Device_CreateGeometryShader,
    (void *)D3D11Device_CreateGeometryShaderWithStreamOutput,
    (void *)D3D11Device_CreatePixelShader,
    (void *)D3D11Device_CreateHullShader,
    (void *)D3D11Device_CreateDomainShader,
    (void *)D3D11Device_CreateComputeShader,
    (void *)D3D11Device_CreateClassLinkage,
    (void *)D3D11Device_CreateBlendState,
    (void *)D3D11Device_CreateDepthStencilState,
    (void *)D3D11Device_CreateRasterizerState,
    (void *)D3D11Device_CreateSamplerState,
    (void *)D3D11Device_CreateQuery,
    (void *)D3D11Device_CreatePredicate,
    (void *)D3D11Device_CreateCounter,
    (void *)D3D11Device_CreateDeferredContext,
    (void *)D3D11Device_OpenSharedResource,
    (void *)D3D11Device_CheckFormatSupport,
    (void *)D3D11Device_CheckMultisampleQualityLevels,
    (void *)D3D11Device_CheckCounterInfo,
    (void *)D3D11Device_CheckCounter,
    (void *)D3D11Device_CheckFeatureSupport,
    (void *)D3D11Device_GetPrivateData,
    (void *)D3D11Device_SetPrivateData,
    (void *)D3D11Device_SetPrivateDataInterface,
    (void *)D3D11Device_,
    (void *)D3D11Device_GetCreationFlags,
    (void *)D3D11Device_GetDeviceRemovedReason,
    (void *)D3D11Device_GetImmediateContext,
    (void *)D3D11Device_SetExceptionMode,
    (void *)D3D11Device_GetExceptionMode
};

static const GUID *D3D11Device_IIDs[] = {
    &IID_ID3D11Device,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11Device_new( struct D3D11Device *pDevice,
struct D3D11Device **ppOut )
{
    D3D11_NEW(D3D11Device, ppOut, pDevice);
}

