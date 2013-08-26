
#ifndef _D3D11_SWITCHTOREF_H_
#define _D3D11_SWITCHTOREF_H_

#include "unknown.h"

struct D3D11SwitchToRef
{
    struct D3D11Unknown base;
};
static INLINE struct D3D11SwitchToRef *D3D11SwitchToRef(void *ptr)
{
    return (struct D3D11SwitchToRef *)ptr;
}

HRESULT
D3D11SwitchToRef_new( struct D3D11Device *, struct D3D11SwitchToRef **ppOut );

BOOL WINAPI
D3D11SwitchToRef_SetUseRef( struct D3D11SwitchToRef *This,
                            BOOL UseRef );

BOOL WINAPI
D3D11SwitchToRef_GetUseRef( struct D3D11SwitchToRef *This );

#endif /* _D3D11_SWITCHTOREF_H_ */

