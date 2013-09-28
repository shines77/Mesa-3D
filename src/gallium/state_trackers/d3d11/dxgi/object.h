
#ifndef _DXGI_OBJECT_H_
#define _DXGI_OBJECT_H_

#include "unknown.h"
#include "privatedata.h"

struct DXGIObject
{
    struct D3D11Unknown base;
    struct D3D11Unknown *parent;
    struct D3D11PrivateData pdata;
};
static INLINE struct DXGIObject *DXGIObject(void *ptr)
{
    return (struct DXGIObject *)ptr;
}

HRESULT
DXGIObject_ctor( struct DXGIObject *This,
                 struct D3D11UnknownParams *pParams,
                 struct D3D11Unknown *pParent )

void
DXGIObject_dtor( struct DXGIObject *This );

HRESULT WINAPI
DXGIObject_SetPrivateData( struct DXGIObject *This,
                           REFGUID Name,
                           UINT DataSize,
                           void *pData );

HRESULT WINAPI
DXGIObject_SetPrivateDataInterface( struct DXGIObject *This,
                                    REFGUID Name,
                                    IUnknown *pUnknown );

HRESULT WINAPI
DXGIObject_GetPrivateData( struct DXGIObject *This,
                           REFGUID Name,
                           UINT *pDataSize,
                           void *pData );

HRESULT WINAPI
DXGIObject_GetParent( struct DXGIObject *This,
                      REFIID riid,
                      void **ppParent );

#endif /* _DXGI_OBJECT_H_ */

