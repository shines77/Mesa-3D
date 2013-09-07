
#ifndef _D3D11_TEXTURE2D_H_
#define _D3D11_TEXTURE2D_H_

#include "resource.h"

struct D3D11Texture2D
{
    struct D3D11Resource base;
    D3D11_TEXTURE2D_DESC desc;
    struct IDXGISurface1 *surf;
};
static INLINE struct D3D11Texture2D *D3D11Texture2D(void *ptr)
{
    return (struct D3D11Texture2D *)ptr;
}

HRESULT
D3D11Texture2D_new( struct D3D11Device *,
                    const D3D11_TEXTURE2D_DESC *pDesc,
                    struct D3D11Texture2D **ppOut );

void WINAPI
D3D11Texture2D_GetDesc( struct D3D11Texture2D *This,
                        D3D11_TEXTURE2D_DESC *pDesc );

#endif /* _D3D11_TEXTURE2D_H_ */

