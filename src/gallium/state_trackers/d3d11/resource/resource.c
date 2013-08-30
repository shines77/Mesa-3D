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

#include "resource.h"

HRESULT
D3D11Resource_ctor( struct D3D11Resource *This,
                    struct D3D11UnknownParams *pParams )
{
    struct pipe_resource *res = This->resource;
    HRESULT hr;
    unsigned l, z, s;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    assert(res);
    This->num_sub = res->array_size * (res->last_level + 1);

    This->sub = MALLOC(This->num_sub * sizeof(*This->sub));
    if (!This->sub)
        return_error(E_OUTOFMEMORY);

    for (s = 0, l = 0; l <= res->last_level; ++l) {
        for (z = 0; z < res->array_size; ++z, ++s) {
            This->sub[s].level = l;
            This->sub[s].layer = z;
            This->sub[s].transfer = NULL;
        }
    }

    return S_OK;
}

void
D3D11Resource_dtor( struct D3D11Resource *This )
{
    pipe_resource_reference(&This->resource, NULL);

    if (This->sub)
        FREE(This->sub);

    D3D11DeviceChild_dtor(&This->base);
}

void WINAPI
D3D11Resource_SetEvictionPriority( struct D3D11Resource *This,
                                   UINT EvictionPriority )
{
    This->eviction_priority = EvictionPriority;
}

UINT WINAPI
D3D11Resource_GetEvictionPriority( struct D3D11Resource *This )
{
    return This->eviction_priority;
}
