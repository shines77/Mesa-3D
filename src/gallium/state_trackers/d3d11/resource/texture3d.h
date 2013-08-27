
#ifndef _D3D11_TEXTURE3D_H_
#define _D3D11_TEXTURE3D_H_

#include "resource.h"

struct D3D11Texture3D
{
    struct D3D11Resource base;
    D3D11_TEXTURE3D_DESC desc;
};
static INLINE struct D3D11Texture3D *D3D11Texture3D(void *ptr)
{
    return (struct D3D11Texture3D *)ptr;
}

HRESULT
D3D11Texture3D_new( struct D3D11Device *,
                    const D3D11_TEXTURE3D_DESC *pDesc,
                    struct D3D11Texture3D **ppOut );

void WINAPI
D3D11Texture3D_GetDesc( struct D3D11Texture3D *This,
                        D3D11_TEXTURE3D_DESC *pDesc );

#endif /* _D3D11_TEXTURE3D_H_ */

