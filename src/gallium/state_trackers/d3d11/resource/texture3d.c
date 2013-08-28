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

#include "texture3d.h"

HRESULT
D3D11Texture3D_ctor( struct D3D11Texture3D *This,
                     struct D3D11UnknownParams *pParams,
                     const D3D11_TEXTURE3D_DESC *pDesc )
{
    struct pipe_screen *screen = pParams->device->screen;
    struct pipe_resource templ;
    HRESULT hr;
    const UINT max_dim = MAX2(MAX2(pDesc->Width, pDesc->Height), pDesc->Depth);

    user_assert(pDesc->Width && pDesc->Height && pDesc->Depth,
                D3DERR_INVALIDCALL);

    templ.target = PIPE_TEXTURE_3D;
    templ.format = dxgi_to_pipe_format(pDesc->Format);
    templ.width0 = pDesc->Width;
    templ.height0 = pDesc->Height;
    templ.depth0 = pDesc->Depth;
    templ.array_size = 1;
    templ.last_level =
        pDesc->MipLevels ? (pDesc->MipLevels - 1) : util_logbase2(max_dim);
    templ.nr_samples = 0;
    templ.usage = d3d11_to_pipe_usage(pDesc->Usage);
    templ.bind = d3d11_usage_to_pipe_bind(pDesc->Usage) |
        d3d11_to_pipe_bind(pDesc->BindFlags) |
        d3d11_cpu_access_flags_to_pipe_bind(pDesc->CPUAccessFlags) |
        d3d11_misc_flags_to_pipe_bind(pDesc->MiscFlags);
    templ.flags = 0;

    user_assert(screen_is_template_supported(screen, &templ),
                D3DERR_INVALIDCALL);

    This->base.resource = screen->resource_create(screen, &templ);
    if (!This->base.resource)
        return_error(D3DERR_OUTOFVIDEOMEMORY);

    hr = D3D11Resource_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;
    This->desc = *pDesc;

    return S_OK;
}

void
D3D11Texture3D_dtor( struct D3D11Texture3D *This )
{
    D3D11Resource_dtor(&This->base);
}

static void WINAPI
D3D11Resource_GetType( struct D3D11Resource *This,
                       D3D11_RESOURCE_DIMENSION *pResourceDimension )
{
    assert(pResourceDimension);
    *pResourceDimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
}

void WINAPI
D3D11Texture3D_GetDesc( struct D3D11Texture3D *This,
                        D3D11_TEXTURE3D_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
}

ID3D11Texture3DVtbl D3D11Texture3D_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface,
    (void *)D3D11Resource_GetType,
    (void *)D3D11Resource_SetEvictionPriority,
    (void *)D3D11Resource_GetEvictionPriority,
    (void *)D3D11Texture3D_GetDesc
};

static const GUID *D3D11Texture3D_IIDs[] = {
    &IID_ID3D11Texture3D,
    &IID_ID3D11Resource,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11Texture3D_new( struct D3D11Device *pDevice,
                    const D3D11_TEXTURE3D_DESC *pDesc,
                    struct D3D11Texture3D **ppOut )
{
    D3D11_NEW(D3D11Texture3D, ppOut, pDevice, pDesc);
}

