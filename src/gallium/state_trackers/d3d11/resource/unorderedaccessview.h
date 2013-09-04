
#ifndef _D3D11_UNORDEREDACCESSVIEW_H_
#define _D3D11_UNORDEREDACCESSVIEW_H_

#include "view.h"

struct D3D11UnorderedAccessView
{
    struct D3D11View base;
    struct pipe_surface *surface;
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
};
static INLINE struct D3D11UnorderedAccessView *D3D11UnorderedAccessView(void *ptr)
{
    return (struct D3D11UnorderedAccessView *)ptr;
}

HRESULT
D3D11UnorderedAccessView_new( struct D3D11Device *,
                              struct D3D11Resource *,
                              const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
                              struct D3D11UnorderedAccessView **ppOut );

void WINAPI
D3D11UnorderedAccessView_GetDesc( struct D3D11UnorderedAccessView *This,
                                  D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc );

#endif /* _D3D11_UNORDEREDACCESSVIEW_H_ */

