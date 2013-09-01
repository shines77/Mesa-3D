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

#include "predicate.h"

HRESULT
D3D11Predicate_ctor( struct D3D11Predicate *This,
                     struct D3D11UnknownParams *pParams,
                     const D3D11_QUERY_DESC *pDesc )
{
    HRESULT hr = D3D11Query_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;
    This->base.base.is_predicate = TRUE;
    return S_OK;
}

void
D3D11Predicate_dtor( struct D3D11Predicate *This )
{
    D3D11Query_dtor(&This->base);
}

ID3D11PredicateVtbl D3D11Predicate_vtable = {
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

static const GUID *D3D11Predicate_IIDs[] = {
    &IID_ID3D11Predicate,
    &IID_ID3D11Query,
    &IID_ID3D11Asynchronous,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11Predicate_new( struct D3D11Device *pDevice,
                    const D3D11_QUERY_DESC *pDesc,
                    struct D3D11Predicate **ppOut )
{
    D3D11_NEW(D3D11Predicate, ppOut, pDevice, pDesc);
}


void WINAPI
D3D11DeviceContext_SetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate *pPredicate,
                                   BOOL PredicateValue )
{
    struct D3D11Query *query = &D3D11Predicate(pPredicate)->base;
    unsigned mode;

    if (!query->base.is_predicate)
        return;
    com_ref(&This->predicate, pPredicate);
    This->predicate_value = PredicateValue;

    mode = PIPE_RENDER_COND_BY_REGION_WAIT;
    if (query->desc.MiscFlags & D3D11_QUERY_MISC_PREDICATEHINT)
        mode = PIPE_RENDER_COND_BY_REGION_NO_WAIT;

    This->pipe->render_condition(This->pipq, async->pq, PredicateValue, mode);
}

void WINAPI
D3D11DeviceContext_GetPredication( struct D3D11DeviceContext *This,
                                   ID3D11Predicate **ppPredicate,
                                   BOOL *pPredicateValue )
{
    if (ppPredicate)
        com_set(ppPredicate, This->predicate);
    if (pPredicateValue)
        *pPredicateValue = This->predicate_value;
}
