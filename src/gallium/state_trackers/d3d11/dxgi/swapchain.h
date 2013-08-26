
#ifndef _DXGI_SWAPCHAIN_H_
#define _DXGI_SWAPCHAIN_H_

#include "devicesubobject.h"

struct DXGISwapChain
{
    struct DXGIDeviceSubObject base;
};
static INLINE struct DXGISwapChain *DXGISwapChain(void *ptr)
{
    return (struct DXGISwapChain *)ptr;
}

HRESULT
DXGISwapChain_new( struct D3D11Device *, struct DXGISwapChain **ppOut );

HRESULT WINAPI
DXGISwapChain_Present( struct DXGISwapChain *This,
                       UINT SyncInterval,
                       UINT Flags );

HRESULT WINAPI
DXGISwapChain_GetBuffer( struct DXGISwapChain *This,
                         UINT Buffer,
                         REFIID riid,
                         void **ppSurface );

HRESULT WINAPI
DXGISwapChain_SetFullscreenState( struct DXGISwapChain *This,
                                  BOOL Fullscreen,
                                  IDXGIOutput *pTarget );

HRESULT WINAPI
DXGISwapChain_GetFullscreenState( struct DXGISwapChain *This,
                                  BOOL *pFullscreen,
                                  IDXGIOutput **ppTarget );

HRESULT WINAPI
DXGISwapChain_GetDesc( struct DXGISwapChain *This,
                       DXGI_SWAP_CHAIN_DESC *pDesc );

HRESULT WINAPI
DXGISwapChain_ResizeBuffers( struct DXGISwapChain *This,
                             UINT BufferCount,
                             UINT Width,
                             UINT Height,
                             Int NewFormat,
                             UINT SwapChainFlags );

HRESULT WINAPI
DXGISwapChain_ResizeTarget( struct DXGISwapChain *This,
                            Int *pNewTargetParameters );

HRESULT WINAPI
DXGISwapChain_GetContainingOutput( struct DXGISwapChain *This,
                                   IDXGIOutput **ppOutput );

HRESULT WINAPI
DXGISwapChain_GetFrameStatistics( struct DXGISwapChain *This,
                                  DXGI_FRAME_STATISTICS *pStats );

HRESULT WINAPI
DXGISwapChain_GetLastPresentCount( struct DXGISwapChain *This,
                                   UINT *pLastPresentCount );

#endif /* _DXGI_SWAPCHAIN_H_ */

