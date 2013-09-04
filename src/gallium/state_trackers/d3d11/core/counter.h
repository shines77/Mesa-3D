
#ifndef _D3D11_COUNTER_H_
#define _D3D11_COUNTER_H_

#include "asynchronous.h"

struct D3D11Counter
{
    struct D3D11Asynchronous base;
    D3D11_COUNTER_DESC desc;
};
static INLINE struct D3D11Counter *D3D11Counter(void *ptr)
{
    return (struct D3D11Counter *)ptr;
}

HRESULT
D3D11Counter_new( struct D3D11Device *, struct D3D11Counter **ppOut );

void WINAPI
D3D11Counter_GetDesc( struct D3D11Counter *This,
                      D3D11_COUNTER_DESC *pDesc );


/* For ID3D11Device: */
HRESULT
D3D11Counter_GetData( struct D3D11Counter *This,
                      struct D3D11DeviceContext *pContext,
                      void *pData,
                      UINT DataSize,
                      UINT GetDataFlags );
void WINAPI
D3D11Device_CheckCounterInfo( struct D3D11Device *This,
                              D3D11_COUNTER_INFO *pCounterInfo );
HRESULT WINAPI
D3D11Device_CheckCounter( struct D3D11Device *This,
                          D3D11_COUNTER_DESC *pDesc,
                          D3D11_COUNTER_TYPE *pType,
                          UINT *pActiveCounters,
                          Int szName,
                          UINT *pNameLength,
                          Int szUnits,
                          UINT *pUnitsLength,
                          Int szDescription,
                          UINT *pDescriptionLength );

#endif /* _D3D11_COUNTER_H_ */

