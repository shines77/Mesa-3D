
#ifndef _D3D11_HULLSHADER_H_
#define _D3D11_HULLSHADER_H_

#include "devicechild.h"

struct D3D11HullShader
{
    struct D3D11DeviceChild base;
    void *cso;
};
static INLINE struct D3D11HullShader *D3D11HullShader(void *ptr)
{
    return (struct D3D11HullShader *)ptr;
}

HRESULT
D3D11HullShader_new( struct D3D11Device *, struct D3D11HullShader **ppOut );

#endif /* _D3D11_HULLSHADER_H_ */

