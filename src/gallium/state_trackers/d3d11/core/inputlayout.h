
#ifndef _D3D11_INPUTLAYOUT_H_
#define _D3D11_INPUTLAYOUT_H_

#include "devicechild.h"

struct D3D11InputLayout
{
    struct D3D11DeviceChild base;
    void *cso;
};
static INLINE struct D3D11InputLayout *D3D11InputLayout(void *ptr)
{
    return (struct D3D11InputLayout *)ptr;
}

HRESULT
D3D11InputLayout_new( struct D3D11Device *, struct D3D11InputLayout **ppOut );

#endif /* _D3D11_INPUTLAYOUT_H_ */

