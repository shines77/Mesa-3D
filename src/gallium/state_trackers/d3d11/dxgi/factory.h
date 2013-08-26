
#ifndef _DXGI_FACTORY_H_
#define _DXGI_FACTORY_H_

#include "object.h"

struct DXGIFactory
{
    struct DXGIObject base;
};
static INLINE struct DXGIFactory *DXGIFactory(void *ptr)
{
    return (struct DXGIFactory *)ptr;
}

HRESULT
DXGIFactory_new( struct D3D11Device *, struct DXGIFactory **ppOut );

HRESULT WINAPI
DXGIFactory_EnumAdapters( struct DXGIFactory *This,
                          UINT Adapter,
                          IDXGIAdapter **ppAdapter );

HRESULT WINAPI
DXGIFactory_MakeWindowAssociation( struct DXGIFactory *This,
                                   HWND WindowHandle,
                                   UINT Flags );

HRESULT WINAPI
DXGIFactory_GetWindowAssociation( struct DXGIFactory *This,
                                  HWND *pWindowHandle );

HRESULT WINAPI
DXGIFactory_CreateSwapChain( struct DXGIFactory *This,
                             IUnknown *pDevice,
                             DXGI_SWAP_CHAIN_DESC *pDesc,
                             IDXGISwapChain **ppSwapChain );

HRESULT WINAPI
DXGIFactory_CreateSoftwareAdapter( struct DXGIFactory *This,
                                   Int Module,
                                   IDXGIAdapter **ppAdapter );

#endif /* _DXGI_FACTORY_H_ */

