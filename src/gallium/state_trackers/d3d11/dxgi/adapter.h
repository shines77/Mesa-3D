
#ifndef _DXGI_ADAPTER_H_
#define _DXGI_ADAPTER_H_

#include "object.h"

struct DXGIAdapter
{
    struct DXGIObject base;
    DXGI_ADAPTER_DESC1 desc;
    struct native_display *diplay;
    const struct native_config **configs;
    const struct native_connector **connectors;
    struct DXGIOutput **output;
    unsigned num_configs;
    unsigned num_crtcs;
    unsigned num_outputs;
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

