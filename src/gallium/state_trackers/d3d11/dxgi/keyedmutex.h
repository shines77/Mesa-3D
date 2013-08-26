
#ifndef _DXGI_KEYEDMUTEX_H_
#define _DXGI_KEYEDMUTEX_H_

#include "devicesubobject.h"

struct DXGIKeyedMutex
{
    struct DXGIDeviceSubObject base;
};
static INLINE struct DXGIKeyedMutex *DXGIKeyedMutex(void *ptr)
{
    return (struct DXGIKeyedMutex *)ptr;
}

HRESULT
DXGIKeyedMutex_new( struct D3D11Device *, struct DXGIKeyedMutex **ppOut );

HRESULT WINAPI
DXGIKeyedMutex_AcquireSync( struct DXGIKeyedMutex *This,
                            UINT64 Key,
                            DWORD dwMilliseconds );

HRESULT WINAPI
DXGIKeyedMutex_ReleaseSync( struct DXGIKeyedMutex *This,
                            UINT64 Key );

#endif /* _DXGI_KEYEDMUTEX_H_ */

