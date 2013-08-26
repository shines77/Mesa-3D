
#ifndef _D3D11_INFOQUEUE_H_
#define _D3D11_INFOQUEUE_H_

#include "unknown.h"

struct D3D11InfoQueue
{
    struct D3D11Unknown base;
};
static INLINE struct D3D11InfoQueue *D3D11InfoQueue(void *ptr)
{
    return (struct D3D11InfoQueue *)ptr;
}

HRESULT
D3D11InfoQueue_new( struct D3D11Device *, struct D3D11InfoQueue **ppOut );

HRESULT WINAPI
D3D11InfoQueue_SetMessageCountLimit( struct D3D11InfoQueue *This,
                                     UINT64 MessageCountLimit );

void WINAPI
D3D11InfoQueue_ClearStoredMessages( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_GetMessage( struct D3D11InfoQueue *This,
                           UINT64 MessageIndex,
                           D3D11_MESSAGE *pMessage,
                           Int *pMessageByteLength );

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesAllowedByStorageFilter( struct D3D11InfoQueue *This );

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesDeniedByStorageFilter( struct D3D11InfoQueue *This );

UINT64 WINAPI
D3D11InfoQueue_GetNumStoredMessages( struct D3D11InfoQueue *This );

UINT64 WINAPI
D3D11InfoQueue_GetNumStoredMessagesAllowedByRetrievalFilter( struct D3D11InfoQueue *This );

UINT64 WINAPI
D3D11InfoQueue_GetNumMessagesDiscardedByMessageCountLimit( struct D3D11InfoQueue *This );

UINT64 WINAPI
D3D11InfoQueue_GetMessageCountLimit( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_AddStorageFilterEntries( struct D3D11InfoQueue *This,
                                        D3D11_INFO_QUEUE_FILTER *pFilter );

HRESULT WINAPI
D3D11InfoQueue_GetStorageFilter( struct D3D11InfoQueue *This,
                                 D3D11_INFO_QUEUE_FILTER *pFilter,
                                 Int *pFilterByteLength );

void WINAPI
D3D11InfoQueue_ClearStorageFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushEmptyStorageFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushCopyOfStorageFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushStorageFilter( struct D3D11InfoQueue *This,
                                  D3D11_INFO_QUEUE_FILTER *pFilter );

void WINAPI
D3D11InfoQueue_PopStorageFilter( struct D3D11InfoQueue *This );

UINT WINAPI
D3D11InfoQueue_GetStorageFilterStackSize( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_AddRetrievalFilterEntries( struct D3D11InfoQueue *This,
                                          D3D11_INFO_QUEUE_FILTER *pFilter );

HRESULT WINAPI
D3D11InfoQueue_GetRetrievalFilter( struct D3D11InfoQueue *This,
                                   D3D11_INFO_QUEUE_FILTER *pFilter,
                                   Int *pFilterByteLength );

void WINAPI
D3D11InfoQueue_ClearRetrievalFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushEmptyRetrievalFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushCopyOfRetrievalFilter( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_PushRetrievalFilter( struct D3D11InfoQueue *This,
                                    D3D11_INFO_QUEUE_FILTER *pFilter );

void WINAPI
D3D11InfoQueue_PopRetrievalFilter( struct D3D11InfoQueue *This );

UINT WINAPI
D3D11InfoQueue_GetRetrievalFilterStackSize( struct D3D11InfoQueue *This );

HRESULT WINAPI
D3D11InfoQueue_AddMessage( struct D3D11InfoQueue *This,
                           D3D11_MESSAGE_CATEGORY Category,
                           D3D11_MESSAGE_SEVERITY Severity,
                           D3D11_MESSAGE_ID ID,
                           Int pDescription );

HRESULT WINAPI
D3D11InfoQueue_AddApplicationMessage( struct D3D11InfoQueue *This,
                                      D3D11_MESSAGE_SEVERITY Severity,
                                      Int pDescription );

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnCategory( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_CATEGORY Category,
                                   BOOL bEnable );

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnSeverity( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_SEVERITY Severity,
                                   BOOL bEnable );

HRESULT WINAPI
D3D11InfoQueue_SetBreakOnID( struct D3D11InfoQueue *This,
                             D3D11_MESSAGE_ID ID,
                             BOOL bEnable );

BOOL WINAPI
D3D11InfoQueue_GetBreakOnCategory( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_CATEGORY Category );

BOOL WINAPI
D3D11InfoQueue_GetBreakOnSeverity( struct D3D11InfoQueue *This,
                                   D3D11_MESSAGE_SEVERITY Severity );

BOOL WINAPI
D3D11InfoQueue_GetBreakOnID( struct D3D11InfoQueue *This,
                             D3D11_MESSAGE_ID ID );

void WINAPI
D3D11InfoQueue_SetMuteDebugOutput( struct D3D11InfoQueue *This,
                                   BOOL bMute );

BOOL WINAPI
D3D11InfoQueue_GetMuteDebugOutput( struct D3D11InfoQueue *This );

#endif /* _D3D11_INFOQUEUE_H_ */

