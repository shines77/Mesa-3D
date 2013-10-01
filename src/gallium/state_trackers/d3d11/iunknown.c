
HRESULT
D3D11Unknown_ctor( struct D3D11Unknown *This,
                   struct D3D11UnknownParams *pParams )
{
   This->refs = 1;
   This->bind = 0;

   This->vtable = pParams->vtable;
   This->guids = pParams->guids;
   This->dtor = pParams->dtor;

   return S_OK;
}

void
D3D11Unknown_dtor( struct D3D11Unknown *This )
{
    FREE(This);
}

HRESULT WINAPI
D3D11Unknown_QueryInterface( struct D3D11Unknown *This,
                             REFIID riid,
                             void **ppvObject )
{
    unsigned i;

    if (!ppvObject)
        return E_POINTER;

    i = 0;
    do {
        if (GUID_equal(This->guids[i], riid)) {
            assert(This->refs);
            D3D11Unknown_AddRef(This);
            *ppvObject = This;
            return S_OK;
        }
    } while (This->guids[++i]);

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

ULONG WINAPI
D3D11Unknown_AddRef( struct D3D11Unknown *This )
{
    if (This->container)
        return D3D11Unknown_AddRef(This->container);

    return p_atomic_inc_return(&This->refs);
}

ULONG WINAPI
D3D11Unknown_Release( struct D3D11Unknown *This )
{
    ULONG r;

    if (This->container)
        return D3D11Unknown_Release(This->container);

    r = p_atomic_dec_return(&This->refs);
    assert(r != -1);
    if (r == 0) {
        if (!This->keep)
            This->dtor(This);
    }
    return r;
}

