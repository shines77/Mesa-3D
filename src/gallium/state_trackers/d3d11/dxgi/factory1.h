
#ifndef _DXGI_FACTORY1_H_
#define _DXGI_FACTORY1_H_

#include "factory.h"

struct DXGIFactory1
{
    struct DXGIFactory base;
};
static INLINE struct DXGIFactory1 *DXGIFactory1(void *ptr)
{
    return (struct DXGIFactory1 *)ptr;
}

HRESULT
DXGIFactory1_new( struct D3D11Device *, struct DXGIFactory1 **ppOut );

HRESULT WINAPI
DXGIFactory1_EnumAdapters1( struct DXGIFactory1 *This,
                            UINT Adapter,
                            IDXGIAdapter1 **ppAdapter );

BOOL WINAPI
DXGIFactory1_IsCurrent( struct DXGIFactory1 *This );

#endif /* _DXGI_FACTORY1_H_ */

