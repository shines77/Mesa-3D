
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
