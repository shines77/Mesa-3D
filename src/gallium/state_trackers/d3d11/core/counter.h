
#ifndef _D3D11_COUNTER_H_
#define _D3D11_COUNTER_H_

#include "asynchronous.h"

struct D3D11Counter
{
    struct D3D11Asynchronous base;
    D3D11_COUNTER_DESC desc;
};
static INLINE struct D3D11Counter *D3D11Counter(void *ptr)
{
    return (struct D3D11Counter *)ptr;
}

HRESULT
D3D11Counter_new( struct D3D11Device *, struct D3D11Counter **ppOut );

void WINAPI
D3D11Counter_GetDesc( struct D3D11Counter *This,
                      D3D11_COUNTER_DESC *pDesc );

#endif /* _D3D11_COUNTER_H_ */

