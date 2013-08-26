
#ifndef _D3D11_RASTERIZERSTATE_H_
#define _D3D11_RASTERIZERSTATE_H_

#include "devicechild.h"

struct D3D11RasterizerState
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11RasterizerState *D3D11RasterizerState(void *ptr)
{
    return (struct D3D11RasterizerState *)ptr;
}

HRESULT
D3D11RasterizerState_new( struct D3D11Device *, struct D3D11RasterizerState **ppOut );

void WINAPI
D3D11RasterizerState_GetDesc( struct D3D11RasterizerState *This,
                              D3D11_RASTERIZER_DESC *pDesc );

#endif /* _D3D11_RASTERIZERSTATE_H_ */

