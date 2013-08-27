
#ifndef _D3D11_BUFFER_H_
#define _D3D11_BUFFER_H_

#include "resource.h"

struct D3D11Buffer
{
    struct D3D11Resource base;
    D3D11_BUFFER_DESC desc;
};
static INLINE struct D3D11Buffer *D3D11Buffer(void *ptr)
{
    return (struct D3D11Buffer *)ptr;
}

HRESULT
D3D11Buffer_new( struct D3D11Device *, struct D3D11Buffer **ppOut );

void WINAPI
D3D11Buffer_GetDesc( struct D3D11Buffer *This,
                     D3D11_BUFFER_DESC *pDesc );

#endif /* _D3D11_BUFFER_H_ */

