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

#include "infoqueue.h"

HRESULT
D3D11InfoQueue_ctor( struct D3D11InfoQueue *This,
struct D3D11UnknownParams *pParams)
{
    HRESULT hr = Unknown_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void
D3D11InfoQueue_dtor( struct D3D11InfoQueue *This )
{
    Unknown_dtor(&This->base);
}

HRESULT WINAPI
D3D11InfoQueue_SetMessageCountLimit( struct D3D11InfoQueue *This,
                                     UINT64 MessageCountLimit )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11InfoQueue_ClearStoredMessages( struct D3D11InfoQueue *This )
{
    STUB();
}

HRESULT WINAPI
D3D11InfoQueue_GetMessage( struct D3D11InfoQueue *This,
                           UINT64 MessageIndex,
                           D3D11_MESSAGE *pMessage,
                           Int *pMessageByteLength )
{
    STUB_return(E_NOTIMPL);
}

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesAllowedByStorageFilter( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesDeniedByStorageFilter( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

UINT64 WINAPI
D3D11InfoQueue_GetNumStoredMessages( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

UINT64 WINAPI
D3D11InfoQueue_GetNumStoredMessagesAllowedByRetrievalFilter( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesDiscardedByMessageCountLimit( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

UINT64 WINAPI
D3D11InfoQueue_GetMessageCountLimit( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

HRESULT WINAPI
D3D11InfoQueue_AddStorageFilterEntries( struct D3D11InfoQueue *This,
                                        D3D11_INFO_QUEUE_FILTER *pFilter )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_GetStorageFilter( struct D3D11InfoQueue *This,
                                 D3D11_INFO_QUEUE_FILTER *pFilter,
                                 Int *pFilterByteLength )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11InfoQueue_ClearStorageFilter( struct D3D11InfoQueue *This )
{
    STUB();
}

HRESULT WINAPI
D3D11InfoQueue_PushEmptyStorageFilter( struct D3D11InfoQueue *This )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_PushCopyOfStorageFilter( struct D3D11InfoQueue *This )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_PushStorageFilter( struct D3D11InfoQueue *This,
                                  D3D11_INFO_QUEUE_FILTER *pFilter )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11InfoQueue_PopStorageFilter( struct D3D11InfoQueue *This )
{
    STUB();
}

UINT WINAPI
D3D11InfoQueue_GetStorageFilterStackSize( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

HRESULT WINAPI
D3D11InfoQueue_AddRetrievalFilterEntries( struct D3D11InfoQueue *This,
                                          D3D11_INFO_QUEUE_FILTER *pFilter )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_GetRetrievalFilter( struct D3D11InfoQueue *This,
                                   D3D11_INFO_QUEUE_FILTER *pFilter,
                                   Int *pFilterByteLength )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11InfoQueue_ClearRetrievalFilter( struct D3D11InfoQueue *This )
{
    STUB();
}

HRESULT WINAPI
D3D11InfoQueue_PushEmptyRetrievalFilter( struct D3D11InfoQueue *This )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_PushCopyOfRetrievalFilter( struct D3D11InfoQueue *This )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_PushRetrievalFilter( struct D3D11InfoQueue *This,
                                    D3D11_INFO_QUEUE_FILTER *pFilter )
{
    STUB_return(E_NOTIMPL);
}

void WINAPI
D3D11InfoQueue_PopRetrievalFilter( struct D3D11InfoQueue *This )
{
    STUB();
}

UINT WINAPI
D3D11InfoQueue_GetRetrievalFilterStackSize( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

HRESULT WINAPI
D3D11InfoQueue_AddMessage( struct D3D11InfoQueue *This,
                           D3D11_MESSAGE_CATEGORY Category,
                           D3D11_MESSAGE_SEVERITY Severity,
                           D3D11_MESSAGE_ID ID,
                           Int pDescription )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_AddApplicationMessage( struct D3D11InfoQueue *This,
                                      D3D11_MESSAGE_SEVERITY Severity,
                                      Int pDescription )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnCategory( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_CATEGORY Category,
                                   BOOL bEnable )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnSeverity( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_SEVERITY Severity,
                                   BOOL bEnable )
{
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnID( struct D3D11InfoQueue *This,
                             D3D11_MESSAGE_ID ID,
                             BOOL bEnable )
{
    STUB_return(E_NOTIMPL);
}

BOOL WINAPI
D3D11InfoQueue_GetBreakOnCategory( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_CATEGORY Category )
{
    STUB_return(0);
}

BOOL WINAPI
D3D11InfoQueue_GetBreakOnSeverity( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_SEVERITY Severity )
{
    STUB_return(0);
}

BOOL WINAPI
D3D11InfoQueue_GetBreakOnID( struct D3D11InfoQueue *This,
                             D3D11_MESSAGE_ID ID )
{
    STUB_return(0);
}

void WINAPI
D3D11InfoQueue_SetMuteDebugOutput( struct D3D11InfoQueue *This,
                                   BOOL bMute )
{
    STUB();
}

BOOL WINAPI
D3D11InfoQueue_GetMuteDebugOutput( struct D3D11InfoQueue *This )
{
    STUB_return(0);
}

ID3D11InfoQueueVtbl D3D11InfoQueue_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11InfoQueue_SetMessageCountLimit,
    (void *)D3D11InfoQueue_ClearStoredMessages,
    (void *)D3D11InfoQueue_GetMessage,
    (void *)D3D11InfoQueue_GetNumMessagesAllowedByStorageFilter,
    (void *)D3D11InfoQueue_GetNumMessagesDeniedByStorageFilter,
    (void *)D3D11InfoQueue_GetNumStoredMessages,
    (void *)D3D11InfoQueue_GetNumStoredMessagesAllowedByRetrievalFilter,
    (void *)D3D11InfoQueue_GetNumMessagesDiscardedByMessageCountLimit,
    (void *)D3D11InfoQueue_GetMessageCountLimit,
    (void *)D3D11InfoQueue_AddStorageFilterEntries,
    (void *)D3D11InfoQueue_GetStorageFilter,
    (void *)D3D11InfoQueue_ClearStorageFilter,
    (void *)D3D11InfoQueue_PushEmptyStorageFilter,
    (void *)D3D11InfoQueue_PushCopyOfStorageFilter,
    (void *)D3D11InfoQueue_PushStorageFilter,
    (void *)D3D11InfoQueue_PopStorageFilter,
    (void *)D3D11InfoQueue_GetStorageFilterStackSize,
    (void *)D3D11InfoQueue_AddRetrievalFilterEntries,
    (void *)D3D11InfoQueue_GetRetrievalFilter,
    (void *)D3D11InfoQueue_ClearRetrievalFilter,
    (void *)D3D11InfoQueue_PushEmptyRetrievalFilter,
    (void *)D3D11InfoQueue_PushCopyOfRetrievalFilter,
    (void *)D3D11InfoQueue_PushRetrievalFilter,
    (void *)D3D11InfoQueue_PopRetrievalFilter,
    (void *)D3D11InfoQueue_GetRetrievalFilterStackSize,
    (void *)D3D11InfoQueue_AddMessage,
    (void *)D3D11InfoQueue_AddApplicationMessage,
    (void *)D3D11InfoQueue_SetBreakOnCategory,
    (void *)D3D11InfoQueue_SetBreakOnSeverity,
    (void *)D3D11InfoQueue_SetBreakOnID,
    (void *)D3D11InfoQueue_GetBreakOnCategory,
    (void *)D3D11InfoQueue_GetBreakOnSeverity,
    (void *)D3D11InfoQueue_GetBreakOnID,
    (void *)D3D11InfoQueue_SetMuteDebugOutput,
    (void *)D3D11InfoQueue_GetMuteDebugOutput
};

static const GUID *D3D11InfoQueue_IIDs[] = {
    &IID_ID3D11InfoQueue,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11InfoQueue_new( struct D3D11Device *pDevice,
struct D3D11InfoQueue **ppOut )
{
    D3D11_NEW(D3D11InfoQueue, ppOut, pDevice);
}

