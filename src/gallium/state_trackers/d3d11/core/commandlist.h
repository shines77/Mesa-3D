
#ifndef _D3D11_COMMANDLIST_H_
#define _D3D11_COMMANDLIST_H_

#include "devicechild.h"

struct D3D11CommandList
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11CommandList *D3D11CommandList(void *ptr)
{
    return (struct D3D11CommandList *)ptr;
}

HRESULT
D3D11CommandList_new( struct D3D11Device *, struct D3D11CommandList **ppOut );

UINT WINAPI
D3D11CommandList_GetContextFlags( struct D3D11CommandList *This );

#endif /* _D3D11_COMMANDLIST_H_ */

