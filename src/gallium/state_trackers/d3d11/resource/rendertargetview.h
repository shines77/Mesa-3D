
#ifndef _D3D11_RENDERTARGETVIEW_H_
#define _D3D11_RENDERTARGETVIEW_H_

#include "view.h"

struct D3D11RenderTargetView
{
    struct D3D11View base;
};
static INLINE struct D3D11RenderTargetView *D3D11RenderTargetView(void *ptr)
{
    return (struct D3D11RenderTargetView *)ptr;
}

HRESULT
D3D11RenderTargetView_new( struct D3D11Device *, struct D3D11RenderTargetView **ppOut );

void WINAPI
D3D11RenderTargetView_GetDesc( struct D3D11RenderTargetView *This,
                               D3D11_RENDER_TARGET_VIEW_DESC *pDesc );

#endif /* _D3D11_RENDERTARGETVIEW_H_ */

