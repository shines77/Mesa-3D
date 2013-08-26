
#ifndef _D3D11_PREDICATE_H_
#define _D3D11_PREDICATE_H_

#include "query.h"

struct D3D11Predicate
{
    struct D3D11Query base;
};
static INLINE struct D3D11Predicate *D3D11Predicate(void *ptr)
{
    return (struct D3D11Predicate *)ptr;
}

HRESULT
D3D11Predicate_new( struct D3D11Device *, struct D3D11Predicate **ppOut );

#endif /* _D3D11_PREDICATE_H_ */

