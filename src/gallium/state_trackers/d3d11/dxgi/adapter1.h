
#ifndef _DXGI_ADAPTER1_H_
#define _DXGI_ADAPTER1_H_

#include "adapter.h"

struct DXGIAdapter1
{
    struct DXGIAdapter base;
};
static INLINE struct DXGIAdapter1 *DXGIAdapter1(void *ptr)
{
    return (struct DXGIAdapter1 *)ptr;
}

HRESULT
DXGIAdapter1_new( struct D3D11Device *, struct DXGIAdapter1 **ppOut );

HRESULT WINAPI
DXGIAdapter1_GetDesc1( struct DXGIAdapter1 *This,
                       DXGI_ADAPTER_DESC1 *pDesc );

#endif /* _DXGI_ADAPTER1_H_ */

