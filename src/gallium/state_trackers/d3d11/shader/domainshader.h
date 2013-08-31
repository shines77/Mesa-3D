
#ifndef _D3D11_DOMAINSHADER_H_
#define _D3D11_DOMAINSHADER_H_

#include "devicechild.h"

struct D3D11DomainShader
{
    struct D3D11DeviceChild base;
    void *cso;
};
static INLINE struct D3D11DomainShader *D3D11DomainShader(void *ptr)
{
    return (struct D3D11DomainShader *)ptr;
}

HRESULT
D3D11DomainShader_new( struct D3D11Device *, struct D3D11DomainShader **ppOut );

#endif /* _D3D11_DOMAINSHADER_H_ */

