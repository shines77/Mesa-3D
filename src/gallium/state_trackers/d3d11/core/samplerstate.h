
#ifndef _D3D11_SAMPLERSTATE_H_
#define _D3D11_SAMPLERSTATE_H_

#include "devicechild.h"

struct D3D11SamplerState
{
    struct D3D11DeviceChild base;
    void *cso;
    D3D11_SAMPLER_DESC desc;
};
static INLINE struct D3D11SamplerState *D3D11SamplerState(void *ptr)
{
    return (struct D3D11SamplerState *)ptr;
}

HRESULT
D3D11SamplerState_new( struct D3D11Device *,
                       const D3D11_SAMPLER_DESC *,
                       struct D3D11SamplerState **ppOut );

void WINAPI
D3D11SamplerState_GetDesc( struct D3D11SamplerState *This,
                           D3D11_SAMPLER_DESC *pDesc );

#endif /* _D3D11_SAMPLERSTATE_H_ */

