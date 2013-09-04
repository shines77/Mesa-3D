
#ifndef _D3D11_SHADERRESOURCEVIEW_H_
#define _D3D11_SHADERRESOURCEVIEW_H_

#include "view.h"

struct D3D11ShaderResourceView
{
    struct D3D11View base;
    struct pipe_sampler_view *sv;
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
};
static INLINE struct D3D11ShaderResourceView *D3D11ShaderResourceView(void *ptr)
{
    return (struct D3D11ShaderResourceView *)ptr;
}

HRESULT
D3D11ShaderResourceView_new( struct D3D11Device *,
                             struct D3D11Resource *,
                             const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
                             struct D3D11ShaderResourceView **ppOut );

void WINAPI
D3D11ShaderResourceView_GetDesc( struct D3D11ShaderResourceView *This,
                                 D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc );

#endif /* _D3D11_SHADERRESOURCEVIEW_H_ */

