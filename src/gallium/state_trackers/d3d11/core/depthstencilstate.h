
#ifndef _D3D11_DEPTHSTENCILSTATE_H_
#define _D3D11_DEPTHSTENCILSTATE_H_

#include "devicechild.h"

struct D3D11DepthStencilState
{
    struct D3D11DeviceChild base;
    void *cso;
    D3D11_DEPTH_STENCIL_DESC desc;
};
static INLINE struct D3D11DepthStencilState *D3D11DepthStencilState(void *ptr)
{
    return (struct D3D11DepthStencilState *)ptr;
}

HRESULT
D3D11DepthStencilState_new( struct D3D11Device *,
                            const D3D11_DEPTH_STENCIL_DESC *,
                            struct D3D11DepthStencilState **ppOut );

void WINAPI
D3D11DepthStencilState_GetDesc( struct D3D11DepthStencilState *This,
                                D3D11_DEPTH_STENCIL_DESC *pDesc );

#endif /* _D3D11_DEPTHSTENCILSTATE_H_ */

