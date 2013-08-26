
#ifndef _D3D11_COMPUTESHADER_H_
#define _D3D11_COMPUTESHADER_H_

#include "devicechild.h"

struct D3D11ComputeShader
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11ComputeShader *D3D11ComputeShader(void *ptr)
{
    return (struct D3D11ComputeShader *)ptr;
}

HRESULT
D3D11ComputeShader_new( struct D3D11Device *, struct D3D11ComputeShader **ppOut );

#endif /* _D3D11_COMPUTESHADER_H_ */

