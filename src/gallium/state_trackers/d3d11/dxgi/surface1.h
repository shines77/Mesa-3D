
#ifndef _DXGI_SURFACE1_H_
#define _DXGI_SURFACE1_H_

#include "surface.h"

struct DXGISurface1
{
    struct DXGISurface base;
};
static INLINE struct DXGISurface1 *DXGISurface1(void *ptr)
{
    return (struct DXGISurface1 *)ptr;
}

HRESULT
DXGISurface1_new( struct D3D11Device *, struct DXGISurface1 **ppOut );

HRESULT WINAPI
DXGISurface1_GetDC( struct DXGISurface1 *This,
                    BOOL Discard,
                    HDC *phdc );

HRESULT WINAPI
DXGISurface1_ReleaseDC( struct DXGISurface1 *This,
                        RECT *pDirtyRect );

#endif /* _DXGI_SURFACE1_H_ */

