
#ifndef _D3D11_BLENDSTATE_H_
#define _D3D11_BLENDSTATE_H_

#include "devicechild.h"

struct D3D11BlendState
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11BlendState *D3D11BlendState(void *ptr)
{
    return (struct D3D11BlendState *)ptr;
}

HRESULT
D3D11BlendState_new( struct D3D11Device *, struct D3D11BlendState **ppOut );

void WINAPI
D3D11BlendState_GetDesc( struct D3D11BlendState *This,
                         D3D11_BLEND_DESC *pDesc );

#endif /* _D3D11_BLENDSTATE_H_ */

