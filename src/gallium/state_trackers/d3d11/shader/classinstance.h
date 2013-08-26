
#ifndef _D3D11_CLASSINSTANCE_H_
#define _D3D11_CLASSINSTANCE_H_

#include "devicechild.h"

struct D3D11ClassInstance
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11ClassInstance *D3D11ClassInstance(void *ptr)
{
    return (struct D3D11ClassInstance *)ptr;
}

HRESULT
D3D11ClassInstance_new( struct D3D11Device *, struct D3D11ClassInstance **ppOut );

void WINAPI
D3D11ClassInstance_GetClassLinkage( struct D3D11ClassInstance *This,
                                    ID3D11ClassLinkage **ppLinkage );

void WINAPI
D3D11ClassInstance_GetDesc( struct D3D11ClassInstance *This,
                            D3D11_CLASS_INSTANCE_DESC *pDesc );

void WINAPI
D3D11ClassInstance_GetInstanceName( struct D3D11ClassInstance *This,
                                    Int pInstanceName,
                                    Int *pBufferLength );

void WINAPI
D3D11ClassInstance_GetTypeName( struct D3D11ClassInstance *This,
                                Int pTypeName,
                                Int *pBufferLength );

#endif /* _D3D11_CLASSINSTANCE_H_ */

