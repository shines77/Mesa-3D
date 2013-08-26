
#ifndef _D3D11_TEXTURE1D_H_
#define _D3D11_TEXTURE1D_H_

#include "resource.h"

struct D3D11Texture1D
{
    struct D3D11Resource base;
};
static INLINE struct D3D11Texture1D *D3D11Texture1D(void *ptr)
{
    return (struct D3D11Texture1D *)ptr;
}

HRESULT
D3D11Texture1D_new( struct D3D11Device *, struct D3D11Texture1D **ppOut );

void WINAPI
D3D11Texture1D_GetDesc( struct D3D11Texture1D *This,
                        D3D11_TEXTURE1D_DESC *pDesc );

#endif /* _D3D11_TEXTURE1D_H_ */

