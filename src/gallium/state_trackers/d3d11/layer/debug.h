
#ifndef _D3D11_DEBUG_H_
#define _D3D11_DEBUG_H_

#include "unknown.h"

struct D3D11Debug
{
    struct D3D11Unknown base;
};
static INLINE struct D3D11Debug *D3D11Debug(void *ptr)
{
    return (struct D3D11Debug *)ptr;
}

HRESULT
D3D11Debug_new( struct D3D11Device *, struct D3D11Debug **ppOut );

HRESULT WINAPI
D3D11Debug_SetFeatureMask( struct D3D11Debug *This,
                           UINT Mask );

UINT WINAPI
D3D11Debug_GetFeatureMask( struct D3D11Debug *This );

HRESULT WINAPI
D3D11Debug_SetPresentPerRenderOpDelay( struct D3D11Debug *This,
                                       UINT Milliseconds );

UINT WINAPI
D3D11Debug_GetPresentPerRenderOpDelay( struct D3D11Debug *This );

HRESULT WINAPI
D3D11Debug_SetSwapChain( struct D3D11Debug *This,
                         Int *pSwapChain );

HRESULT WINAPI
D3D11Debug_GetSwapChain( struct D3D11Debug *This,
                         Int **ppSwapChain );

HRESULT WINAPI
D3D11Debug_ValidateContext( struct D3D11Debug *This,
                            Int *pContext );

HRESULT WINAPI
D3D11Debug_ReportLiveDeviceObjects( struct D3D11Debug *This,
                                    D3D11_RLDO_FLAGS Flags );

HRESULT WINAPI
D3D11Debug_ValidateContextForDispatch( struct D3D11Debug *This,
                                       Int *pContext );

#endif /* _D3D11_DEBUG_H_ */

