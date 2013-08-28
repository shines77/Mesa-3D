
#ifndef _D3D11_QUERY_H_
#define _D3D11_QUERY_H_

#include "asynchronous.h"

struct D3D11Query
{
    struct D3D11Asynchronous base;
    struct D3D11_QUERY_DESC desc;
};
static INLINE struct D3D11Query *D3D11Query(void *ptr)
{
    return (struct D3D11Query *)ptr;
}

HRESULT
D3D11Query_new( struct D3D11Device *,
                const D3D11_QUERY_DESC *pDesc,
                struct D3D11Query **ppOut );

void WINAPI
D3D11Query_GetDesc( struct D3D11Query *This,
                    D3D11_QUERY_DESC *pDesc );

#endif /* _D3D11_QUERY_H_ */

