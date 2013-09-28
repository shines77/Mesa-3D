
#ifndef _D3D11_VERTEXSHADER_H_
#define _D3D11_VERTEXSHADER_H_

#include "devicechild.h"

struct D3D11Shader
{
    struct D3D11DeviceChild base;
    void *cso;
    struct pipe_constant_buffer icb;
    uint8_t type; /* PIPE_SHADER_x */
};
static INLINE struct D3D11Shader *D3D11Shader(void *ptr)
{
    return (struct D3D11Shader *)ptr;
}

HRESULT
D3D11Shader_ctor( struct D3D11Shader *This,
                  struct D3D11UnknownParams *pParams );
void
D3D11Shader_dtor( struct D3D11Shader *This );

#endif /* _D3D11_BASESHADER_H_ */
