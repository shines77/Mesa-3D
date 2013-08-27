
#ifndef _D3D11_PRIVATEDATA_H_
#define _D3D11_PRIVATEDATA_H_

struct util_hash_table;

struct D3D11PrivateData
{
    struct util_hash_table *ht;
};

HRESULT
D3D11PrivateData_Free( struct D3D11PrivateData *,
                       REFGUID guid );

HRESULT
D3D11PrivateData_Get( struct D3D11PrivateData *,
                      REFGUID guid,
                      UINT *pDataSize,
                      void *pData );

HRESULT
D3D11PrivateData_Set( struct D3D11PrivateData *,
                      REFGUID guid,
                      UINT pDataSize,
                      const void *pData );

HRESULT
D3D11PrivateData_SetInterface( struct D3D11PrivateData *,
                               REFGUID guid,
                               const IUnknown *pData );

#endif /* _D3D11_PRIVATEDATA_H_ */
