
#ifndef _DXGI_DEVICE1_H_
#define _DXGI_DEVICE1_H_

#include "device.h"

struct DXGIDevice1
{
    struct DXGIDevice base;
};
static INLINE struct DXGIDevice1 *DXGIDevice1(void *ptr)
{
    return (struct DXGIDevice1 *)ptr;
}

HRESULT
DXGIDevice1_new( struct D3D11Device *, struct DXGIDevice1 **ppOut );

HRESULT WINAPI
DXGIDevice1_SetMaximumFrameLatency( struct DXGIDevice1 *This,
                                    UINT MaxLatency );

HRESULT WINAPI
DXGIDevice1_GetMaximumFrameLatency( struct DXGIDevice1 *This,
                                    UINT *pMaxLatency );

#endif /* _DXGI_DEVICE1_H_ */

