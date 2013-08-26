
#ifndef _D3D11_CLASSLINKAGE_H_
#define _D3D11_CLASSLINKAGE_H_

#include "devicechild.h"

struct D3D11ClassLinkage
{
    struct D3D11DeviceChild base;
};
static INLINE struct D3D11ClassLinkage *D3D11ClassLinkage(void *ptr)
{
    return (struct D3D11ClassLinkage *)ptr;
}

HRESULT
D3D11ClassLinkage_new( struct D3D11Device *, struct D3D11ClassLinkage **ppOut );

HRESULT WINAPI
D3D11ClassLinkage_GetClassInstance( struct D3D11ClassLinkage *This,
                                    Int pClassInstanceName,
                                    UINT InstanceIndex,
                                    ID3D11ClassInstance **ppInstance );

HRESULT WINAPI
D3D11ClassLinkage_CreateClassInstance( struct D3D11ClassLinkage *This,
                                       Int pClassTypeName,
                                       UINT ConstantBufferOffset,
                                       UINT ConstantVectorOffset,
                                       UINT TextureOffset,
                                       UINT SamplerOffset,
                                       ID3D11ClassInstance **ppInstance );

#endif /* _D3D11_CLASSLINKAGE_H_ */

