
#ifndef _DXGI_RESOURCE_H_
#define _DXGI_RESOURCE_H_

#include "devicesubobject.h"

struct DXGIResource
{
    struct DXGIDeviceSubObject base;
};
static INLINE struct DXGIResource *DXGIResource(void *ptr)
{
    return (struct DXGIResource *)ptr;
}

HRESULT
DXGIResource_new( struct D3D11Device *, struct DXGIResource **ppOut );

HRESULT WINAPI
DXGIResource_GetSharedHandle( struct DXGIResource *This,
                              HANDLE *pSharedHandle );

HRESULT WINAPI
DXGIResource_GetUsage( struct DXGIResource *This,
                       DXGI_USAGE *pUsage );

HRESULT WINAPI
DXGIResource_SetEvictionPriority( struct DXGIResource *This,
                                  UINT EvictionPriority );

HRESULT WINAPI
DXGIResource_GetEvictionPriority( struct DXGIResource *This,
                                  UINT *pEvictionPriority );

#endif /* _DXGI_RESOURCE_H_ */

