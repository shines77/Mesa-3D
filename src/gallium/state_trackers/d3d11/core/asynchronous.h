
#ifndef _D3D11_ASYNCHRONOUS_H_
#define _D3D11_ASYNCHRONOUS_H_

#include "devicechild.h"

struct D3D11Asynchronous
{
    struct D3D11DeviceChild base;
    UINT data_size;
    struct pipe_query *pq;
    unsigned pq_type;
    boolean is_predicate;
    boolean is_query;
    boolean is_instant;
    boolean is_counter; /* redundant */
};
static INLINE struct D3D11Asynchronous *D3D11Asynchronous(void *ptr)
{
    return (struct D3D11Asynchronous *)ptr;
}

UINT WINAPI
D3D11Asynchronous_GetDataSize( struct D3D11Asynchronous *This );

#endif /* _D3D11_ASYNCHRONOUS_H_ */

