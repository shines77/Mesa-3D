
#ifndef _D3D11_RESOURCE_H_
#define _D3D11_RESOURCE_H_

#include "devicechild.h"

struct D3D11Resource
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11Resource *D3D11Resource(void *ptr)
{
    return (struct D3D11Resource *)ptr;
}

HRESULT
D3D11Resource_new( struct D3D11Device *, struct D3D11Resource **ppOut );

void WINAPI
D3D11Resource_GetType( struct D3D11Resource *This,
                       D3D11_RESOURCE_DIMENSION *pResourceDimension );

void WINAPI
D3D11Resource_SetEvictionPriority( struct D3D11Resource *This,
                                   UINT EvictionPriority );

UINT WINAPI
D3D11Resource_GetEvictionPriority( struct D3D11Resource *This );

#endif /* _D3D11_RESOURCE_H_ */

