
#ifndef _D3D11_ASYNCHRONOUS_H_
#define _D3D11_ASYNCHRONOUS_H_

#include "devicechild.h"

struct D3D11Asynchronous
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11Asynchronous *D3D11Asynchronous(void *ptr)
{
    return (struct D3D11Asynchronous *)ptr;
}

HRESULT
D3D11Asynchronous_new( struct D3D11Device *, struct D3D11Asynchronous **ppOut );

UINT WINAPI
D3D11Asynchronous_GetDataSize( struct D3D11Asynchronous *This );

#endif /* _D3D11_ASYNCHRONOUS_H_ */

