
#ifndef _D3D11_VIEW_H_
#define _D3D11_VIEW_H_

#include "devicechild.h"

struct D3D11View
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11View *D3D11View(void *ptr)
{
    return (struct D3D11View *)ptr;
}

HRESULT
D3D11View_new( struct D3D11Device *, struct D3D11View **ppOut );

void WINAPI
D3D11View_GetResource( struct D3D11View *This,
                       ID3D11Resource **ppResource );

#endif /* _D3D11_VIEW_H_ */

