
#ifndef _DXGI_DEVICESUBOBJECT_H_
#define _DXGI_DEVICESUBOBJECT_H_

#include "object.h"

struct DXGIDeviceSubObject
{
    struct DXGIObject base;
    struct DXGIDevice *device;
};
static INLINE struct DXGIDeviceSubObject *DXGIDeviceSubObject(void *ptr)
{
    return (struct DXGIDeviceSubObject *)ptr;
}

HRESULT
DXGIDeviceSubObject_ctor( struct DXGIDeviceSubObject *,
                          struct D3D11UnknownParams *,
                          struct DXGIDevice * );
void
DXGIDeviceSubObject_dtor( struct DXGIDeviceSubObject * );

HRESULT WINAPI
DXGIDeviceSubObject_GetDevice( struct DXGIDeviceSubObject *This,
                               REFIID riid,
                               void **ppDevice );

#endif /* _DXGI_DEVICESUBOBJECT_H_ */

