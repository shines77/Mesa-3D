
#ifndef _IUNKNOWN_H_
#define _IUNKNOWN_H_

#include "d3d11.h"

struct D3D11Unknown
{
    void *vtbl;

    int32_t refs; /* reference count */
    int32_t keep; /* internal reference count */

    struct D3D11Unknown *container;

    const GUID **guids; /* for QueryInterface */

    void (*dtor)(void *); /* top-level destructor */
};
static INLINE struct D3D11Unknown *
D3D11Unknown(void *ptr)
{
    return (struct D3D11Unknown *)ptr;
}

struct D3D11UnknownParams
{
    void *vtbl;
    const GUID **guids;
    void (*dtor)(void *);
    struct D3D11Unknown *container;
};

HRESULT
D3D11Unknown_ctor(struct D3D11Unknown *, struct D3D11UnknownParams *);
void
D3D11Unknown_dtor(struct D3D11Unknwon *);


HRESULT WINAPI
D3D11Unknown_QueryInterface(struct D3D11Unknown *,
                            REFIID riid,
                            void **ppvObject);

ULONG WINAPI
D3D11Unknown_AddRef(struct D3D11Unknown *);

ULONG WINAPI
D3D11Unknown_Release(struct D3D11Unknown *);


static INLINE void
com_reference(struct D3D11Unknown **ref, struct D3D11Unknown *obj)
{
    if (*ref != obj) {
        if (*ref)
            D3D11Unknown_Release(*ref);
        if (obj)
            D3D11Unknown_AddRef(obj);
        *ref = obj;
    }
}
#define com_ref(a, b) com_reference((void **)a, (void *)b)

static INLINE void
com_reference_set(struct D3D11Unknown **ref, struct D3D11Unknown *obj)
{
    D3D11Unknown_AddRef(obj);
    *ref = obj;
}
#define com_set(a, b) com_reference_set((void **)a, (void *)b)

#endif /* _IUNKNOWN_H_ */
