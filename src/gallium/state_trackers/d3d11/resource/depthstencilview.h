
#ifndef _D3D11_DEPTHSTENCILVIEW_H_
#define _D3D11_DEPTHSTENCILVIEW_H_

#include "view.h"

struct D3D11DepthStencilView
{
    struct D3D11View base;
};
static INLINE struct D3D11DepthStencilView *D3D11DepthStencilView(void *ptr)
{
    return (struct D3D11DepthStencilView *)ptr;
}

HRESULT
D3D11DepthStencilView_new( struct D3D11Device *, struct D3D11DepthStencilView **ppOut );

void WINAPI
D3D11DepthStencilView_GetDesc( struct D3D11DepthStencilView *This,
                               D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc );

#endif /* _D3D11_DEPTHSTENCILVIEW_H_ */

