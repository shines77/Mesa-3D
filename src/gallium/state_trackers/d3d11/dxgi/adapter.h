
#ifndef _DXGI_ADAPTER_H_
#define _DXGI_ADAPTER_H_

#include "object.h"

struct DXGIAdapter
{
    struct DXGIObject base;
};
static INLINE struct DXGIAdapter *DXGIAdapter(void *ptr)
{
    return (struct DXGIAdapter *)ptr;
}

HRESULT
DXGIAdapter_new( struct D3D11Device *, struct DXGIAdapter **ppOut );

HRESULT WINAPI
DXGIAdapter_EnumOutputs( struct DXGIAdapter *This,
                         UINT Output,
                         IDXGIOutput **ppOutput );

HRESULT WINAPI
DXGIAdapter_GetDesc( struct DXGIAdapter *This,
                     DXGI_ADAPTER_DESC *pDesc );

HRESULT WINAPI
DXGIAdapter_CheckInterfaceSupport( struct DXGIAdapter *This,
                                   REFGUID InterfaceName,
                                   LARGE_INTEGER *pUMDVersion );

#endif /* _DXGI_ADAPTER_H_ */

