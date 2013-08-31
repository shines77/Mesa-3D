
#ifndef _D3D11_PIXELSHADER_H_
#define _D3D11_PIXELSHADER_H_

#include "devicechild.h"

struct D3D11PixelShader
{
    struct D3D11DeviceChild base;
    void *cso;
};
static INLINE struct D3D11PixelShader *D3D11PixelShader(void *ptr)
{
    return (struct D3D11PixelShader *)ptr;
}

HRESULT
D3D11PixelShader_new( struct D3D11Device *, struct D3D11PixelShader **ppOut );

#endif /* _D3D11_PIXELSHADER_H_ */

