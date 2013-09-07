
#ifndef _DXGI_SURFACE_H_
#define _DXGI_SURFACE_H_

#include "devicesubobject.h"

struct DXGISurface
{
    struct DXGIDeviceSubObject base;
    DXGI_SURFACE_DESC desc;
    struct D3D11Texture2D *tex;
};
static INLINE struct DXGISurface *DXGISurface(void *ptr)
{
    return (struct DXGISurface *)ptr;
}

HRESULT
DXGISurface_new( struct D3D11Device *, struct DXGISurface **ppOut );

HRESULT WINAPI
DXGISurface_GetDesc( struct DXGISurface *This,
                     DXGI_SURFACE_DESC *pDesc );

HRESULT WINAPI
DXGISurface_Map( struct DXGISurface *This,
                 DXGI_MAPPED_RECT *pLockedRect,
                 UINT MapFlags );

HRESULT WINAPI
DXGISurface_Unmap( struct DXGISurface *This );

#endif /* _DXGI_SURFACE_H_ */

