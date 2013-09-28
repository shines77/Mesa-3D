
#ifndef _D3D11_VERTEXSHADER_H_
#define _D3D11_VERTEXSHADER_H_

#include "devicechild.h"

struct D3D11VertexShader
{
    struct D3D11Shader base;
    void *cso;
};
static INLINE struct D3D11VertexShader *D3D11VertexShader(void *ptr)
{
    return (struct D3D11VertexShader *)ptr;
}

HRESULT
D3D11VertexShader_new( struct D3D11Device *, struct D3D11VertexShader **ppOut );

#endif /* _D3D11_VERTEXSHADER_H_ */

