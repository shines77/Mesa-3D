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

#include "asynchronous.h"

HRESULT
D3D11Asynchronous_ctor( struct D3D11Asynchronous *This,
                        struct D3D11UnknownParams *pParams )
{
    HRESULT hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;
    return S_OK;
}

void
D3D11Asynchronous_dtor( struct D3D11Asynchronous *This )
{
    D3D11DeviceChild_dtor(&This->base);
}

UINT WINAPI
D3D11Asynchronous_GetDataSize( struct D3D11Asynchronous *This )
{
    return This->data_size;
}

void WINAPI
D3D11DeviceContext_Begin( struct D3D11DeviceContext *This,
                          ID3D11Asynchronous *pAsync )
{
    struct D3D11Asynchronous *async = D3D11Asynchronous(pAsync);
    if (!async->is_instant)
        This->pipe->begin_query(This->pipe, async->pq);
}

void WINAPI
D3D11DeviceContext_End( struct D3D11DeviceContext *This,
                        ID3D11Asynchronous *pAsync )
{
    struct D3D11Asynchronous *async = D3D11Asynchronous(pAsync);
    This->pipe->end_query(This->pipe, async->pq);
}

HRESULT WINAPI
D3D11DeviceContext_GetData( struct D3D11DeviceContext *This,
                            ID3D11Asynchronous *pAsync,
                            void *pData,
                            UINT DataSize,
                            UINT GetDataFlags )
{
    struct D3D11Asynchronous *async = D3D11QueryAsynchronous(pAsync);
    if (async->is_query)
        return D3D11Query_GetData(D3D11Query(async), This, pData, DataSize, GetDataFlags);
    if (async->is_counter)
        return D3D11Counter_GetData(D3D11Counter(async), This, pData, DataSize, GetDataFlags);
    return_error(E_INVALIDARG);
}
