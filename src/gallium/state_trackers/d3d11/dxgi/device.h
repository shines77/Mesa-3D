
#ifndef _DXGI_DEVICE_H_
#define _DXGI_DEVICE_H_

#include "object.h"

struct DXGIDevice
{
    struct DXGIObject base;
    struct DXGIAdapter *adapter;
    struct D3D11Device *device;
    INT priority;
};
static INLINE struct DXGIDevice *DXGIDevice(void *ptr)
{
    return (struct DXGIDevice *)ptr;
}

HRESULT
DXGIDevice_new( struct D3D11Device *, struct DXGIDevice **ppOut );

HRESULT WINAPI
DXGIDevice_GetAdapter( struct DXGIDevice *This,
                       IDXGIAdapter **pAdapter );

HRESULT WINAPI
DXGIDevice_CreateSurface( struct DXGIDevice *This,
                          DXGI_SURFACE_DESC *pDesc,
                          UINT NumSurfaces,
                          DXGI_USAGE Usage,
                          DXGI_SHARED_RESOURCE *pSharedResource,
                          IDXGISurface **ppSurface );

HRESULT WINAPI
DXGIDevice_QueryResourceResidency( struct DXGIDevice *This,
                                   IUnknown *const *ppResources,
                                   DXGI_RESIDENCY *pResidencyStatus,
                                   UINT NumResources );

HRESULT WINAPI
DXGIDevice_SetGPUThreadPriority( struct DXGIDevice *This,
                                 INT Priority );

HRESULT WINAPI
DXGIDevice_GetGPUThreadPriority( struct DXGIDevice *This,
                                 INT *pPriority );

#endif /* _DXGI_DEVICE_H_ */

