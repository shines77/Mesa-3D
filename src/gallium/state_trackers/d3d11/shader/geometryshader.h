
#ifndef _D3D11_GEOMETRYSHADER_H_
#define _D3D11_GEOMETRYSHADER_H_

#include "devicechild.h"

struct D3D11GeometryShader
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11GeometryShader *D3D11GeometryShader(void *ptr)
{
    return (struct D3D11GeometryShader *)ptr;
}

HRESULT
D3D11GeometryShader_new( struct D3D11Device *, struct D3D11GeometryShader **ppOut );

#endif /* _D3D11_GEOMETRYSHADER_H_ */

