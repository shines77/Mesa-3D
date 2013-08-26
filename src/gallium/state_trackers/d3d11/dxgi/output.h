
#ifndef _DXGI_OUTPUT_H_
#define _DXGI_OUTPUT_H_

#include "object.h"

struct DXGIOutput
{
    struct DXGIObject base;
};
static INLINE struct DXGIOutput *DXGIOutput(void *ptr)
{
    return (struct DXGIOutput *)ptr;
}

HRESULT
DXGIOutput_new( struct D3D11Device *, struct DXGIOutput **ppOut );

HRESULT WINAPI
DXGIOutput_GetDesc( struct DXGIOutput *This,
                    DXGI_OUTPUT_DESC *pDesc );

HRESULT WINAPI
DXGIOutput_GetDisplayModeList( struct DXGIOutput *This,
                               Int EnumFormat,
                               UINT Flags,
                               UINT *pNumModes,
                               FunctionProto __out_ecount_part_opt );

HRESULT WINAPI
DXGIOutput_FindClosestMatchingMode( struct DXGIOutput *This,
                                    Int *pModeToMatch,
                                    Int *pClosestMatch,
                                    IUnknown *pConcernedDevice );

HRESULT WINAPI
DXGIOutput_WaitForVBlank( struct DXGIOutput *This );

HRESULT WINAPI
DXGIOutput_TakeOwnership( struct DXGIOutput *This,
                          IUnknown *pDevice,
                          BOOL Exclusive );

void WINAPI
DXGIOutput_ReleaseOwnership( struct DXGIOutput *This );

HRESULT WINAPI
DXGIOutput_GetGammaControlCapabilities( struct DXGIOutput *This,
                                        Int *pGammaCaps );

HRESULT WINAPI
DXGIOutput_SetGammaControl( struct DXGIOutput *This,
                            Int *pArray );

HRESULT WINAPI
DXGIOutput_GetGammaControl( struct DXGIOutput *This,
                            Int *pArray );

HRESULT WINAPI
DXGIOutput_SetDisplaySurface( struct DXGIOutput *This,
                              IDXGISurface *pScanoutSurface );

HRESULT WINAPI
DXGIOutput_GetDisplaySurfaceData( struct DXGIOutput *This,
                                  IDXGISurface *pDestination );

HRESULT WINAPI
DXGIOutput_GetFrameStatistics( struct DXGIOutput *This,
                               DXGI_FRAME_STATISTICS *pStats );

#endif /* _DXGI_OUTPUT_H_ */

