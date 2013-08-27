/*
 * Copyright 2013 Christoph Bumiller
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the Software),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "privatedata.h"

void WINAPI
D3D11PrivateDatae_GetDevice( struct D3D11PrivateData *This,
                             ID3D11Device **ppDevice )
{
   assert_pointer(ppDevice);
   D3D11Unknown_AddRef(This->device);
   *ppDevice = (ID3D11Device *)This->device;
}


struct privdata_header
{
    boolean is_iunk;
    UINT size;
    char data[0];
};

static int
ht_guid_compare(void *key1, void *key2)
{
    return !GUID_equal(key1, key2);
}

static unsigned
ht_guid_hash(void *key)
{
    const uint8_t *u8 = key;
    unsigned i;
    unsigned hash = u8[0];

    for (i = 1; i < sizeof(GUID); ++i)
        hash = (hash << 6) + (hash << 16) - hash + (unsigned)u8[i];
}

static enum pipe_error
ht_guid_delete(void *key, void *value, void *data)
{
    struct privdata_header *pdh = value;

    if (pdh->is_iunk)
        IUnknown_Release(*(IUnknown **)pdh->data);
    FREE(pdh);

    return PIPE_OK;
}

static HRESULT
D3D11PrivateData_Free( struct D3D11PrivateData *This,
                                  REFGUID guid,
                                  struct privdata_header *pdh )
{
    if (!pdh && likely(This->pdata))
        pdh = util_hash_table_get(This->pdata, guid);
    if (!pdh)
        return_error(D3DERR_NOTFOUND);

    if (pdh->is_iunk)
        IUnknown_Release(*(IUnknown **)pdh->data);
    FREE(pdh);
    util_hash_table_remove(This->pdata, guid);

    return S_OK;
}

HRESULT WINAPI
D3D11PrivateData_Get( struct D3D11PrivateData *This,
                                 REFGUID guid,
                                 UINT *pDataSize,
                                 void *pData )
{
    const struct privdata_header *pdh;

    user_assert(pDataSize, E_INVALIDARG);

    if (!This->pdata)
        return_error(D3DERR_NOTFOUND);

    pdh = util_hash_table_get(This->pdata, guid);
    if (!pdh)
        return_error(D3DERR_NOTFOUND);

    if (!pData) {
        *pDataSize = pdh->size;
        return S_OK;
    }
    user_assert(!pdh->is_iunk || 
                *pDataSize >= sizeof(IUnknown *), E_INVALIDARG);

    *pDataSize = MIN2(*pDataSize, pdh->size);

    if (pdh->is_iunk)
        IUnknown_AddRef(*(IUnknown **)pdh->data);

    memcpy(pData, pdh->data, *pDataSize);

    return S_OK;
}

HRESULT WINAPI
D3D11PrivateData_Set( struct D3D11PrivateData *This,
                                 REFGUID guid,
                                 UINT DataSize,
                                 const void *pData )
{
    struct privdata_header *pdh;

    if (!pData)
        return D3D11PrivateData_Free(This, guid);

    if (unlikely(!This->pdata))
        This->pdata = util_hash_table_create(ht_guid_hash, ht_guid_compare);

    pdh = util_hash_table_get(This->pdata, guid);

    if (!pdh || pdh->size != DataSize) {
        enum pipe_error err;
        if (pdh)
            D3D11PrivateData_Free(This, NULL, pdh);

        pdh = CALLOC_VARIANT_LENGTH_STRUCT(privdata_header, DataSize);
        if (!pdh)
            return_error(E_OUTOFMEMORY);

        err = util_hash_table_set(This->pdata, guid, pdh);
        if (err != PIPE_OK) {
            FREE(pdh);
            if (err == PIPE_ERROR_OUT_OF_MEMORY)
                return_error(E_OUTOFMEMORY);
            return_error(D3DERR_DRIVERINTERNALERROR);
        }
    } else {
        if (pdh->is_iunk)
            IUnknown_Release(*(IUnknown **)pdh->data);
    }
    pdh->size = DataSize;

    memcpy(pdh->data, pData, DataSize);

    return S_OK;
}

HRESULT WINAPI
D3D11PrivateData_SetPrivateDataInterface( struct D3D11PrivateData *This,
                                          REFGUID guid,
                                          const IUnknown *pData )
{
    struct privdata_header *pdh;

    if (!pData)
        return D3D11PrivateData_Free(This, guid);

    if (unlikely(!This->pdata))
        This->pdata = util_hash_table_create(ht_guid_hash, ht_guid_compare);

    pdh = util_hash_table_get(This->pdata, guid);

    if (!pdh || pdh->size != sizeof(pData)) {
        enum pipe_error err;
        if (pdh)
            D3D11PrivateData_Free(This, NULL, pdh);

        pdh = CALLOC_VARIANT_LENGTH_STRUCT(privdata_header, sizeof(pData));
        if (!pdh)
            return_error(E_OUTOFMEMORY);

        err = util_hash_table_set(This->pdata, guid, pdh);
        if (err != PIPE_OK) {
            FREE(pdh);
            if (err == PIPE_ERROR_OUT_OF_MEMORY)
                return_error(E_OUTOFMEMORY);
            return_error(D3DERR_DRIVERINTERNALERROR);
        }
    } else {
        if (pdh->is_iunk)
            IUnknown_Release(*(IUnknown **)pdh->data);
    }
    pdh->size = sizeof(pData);

    IUnknown_AddRef(pData);
    *(IUnknown **)pdh->data = pData;

    return S_OK;
}
