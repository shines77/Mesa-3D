
#ifndef _D3D11_DEVICECHILD_H_
#define _D3D11_DEVICECHILD_H_

#include "iunknown.h"
#include "privatedata.h"

struct D3D11DeviceChild
{
    struct D3D11Unknown base;
    struct D3D11Device *device;
    struct D3D11PrivateData pdata;
};
static INLINE struct D3D11DeviceChild *D3D11DeviceChild(void *ptr)
{
    return (struct D3D11DeviceChild *)ptr;
}

HRESULT
D3D11DeviceChild_new( struct D3D11Device *, struct D3D11DeviceChild **ppOut );

void WINAPI
D3D11DeviceChild_GetDevice( struct D3D11DeviceChild *This,
                            ID3D11Device **ppDevice );

HRESULT WINAPI
D3D11DeviceChild_GetPrivateData( struct D3D11DeviceChild *This,
                                 REFGUID guid,
                                 UINT *pDataSize,
                                 void *pData );

HRESULT WINAPI
D3D11DeviceChild_SetPrivateData( struct D3D11DeviceChild *This,
                                 REFGUID guid,
                                 UINT DataSize,
                                 void *pData );

HRESULT WINAPI
D3D11DeviceChild_SetPrivateDataInterface( struct D3D11DeviceChild *This,
                                          REFGUID guid,
                                          IUnknown *pData );

#endif /* _D3D11_DEVICECHILD_H_ */

