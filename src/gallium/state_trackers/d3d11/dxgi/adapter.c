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

#include "adapter.h"

HRESULT
DXGIAdapter_ctor( struct DXGIAdapter *This,
                  struct D3D11UnknownParams *pParams,
                  struct DXGIFactory *factory,
                  const struct native_platform *platform,
                  void *dpy )
{
    struct native_display *display;
    HRESULT hr;
    int i, n;
    char name[128];

    hr = DXGIObject_ctor(&This->base, pParams, &factory->base.base);
    if (FAILED(hr))
        return hr;

    This->display = platform->create_display(dpy, FALSE);
    if (!This->display)
        This->display = platform->create_display(dpy, TRUE);
    if (!This->display)
        return Error(E_FAIL, "Failed to create_display.\n");
    This->display->user_data = This;

    display = This->display;

    if (!This->display->init_screen(This->display))
        return Error(E_FAIL, "Failed to init_screen.\n");

    n = snprintf(name, sizeof(name), "D3D11/st on %s %s",
                 This->display->screen->get_vendor(This->display->screen),
                 This->display->screen->get_name(This->display->screen));

    if (n > Elements(This->desc.Description))
        n = Elements(This->desc.Description);
    for (i = 0; i < n; ++i)
        This->desc.Description[i] = (WCHAR)name[i];

    desc.DedicatedVideoMemory = This->display->screen->get_param(
        This->display->screen, PIPE_CAP_DEVICE_MEMORY_SIZE) << 20;
    desc.DedicatedSystemMemory = 0;
    desc.SharedSystemMemory = 1ULL << 32;

    *(void **)&desc.AdapterLuid = dpy; /* XXX */

    This->configs = display->get_configs(display, &This->num_configs);

    for (i = 0; i < This->num_configs; ++i) {
    }

    if (display->modeset) {
    }

    return S_OK;
}

void
DXGIAdapter_dtor( struct DXGIAdapter *This )
{
    if (This->display) {
        This->display->destroy(This->display);
        FREE(This->configs);
        FREE(This->connectors);
    }
    DXGIObject_dtor(&This->base);
}

HRESULT WINAPI
DXGIAdapter_EnumOutputs( struct DXGIAdapter *This,
                         UINT Output,
                         IDXGIOutput **ppOutput )
{
    if (Output >= This->num_outputs)
        return Error(DXGI_ERROR_NOT_FOUND, "Output ID out of range.\n");
    STUB_return(E_NOTIMPL);
}

HRESULT WINAPI
DXGIAdapter_GetDesc( struct DXGIAdapter *This,
                     DXGI_ADAPTER_DESC *pDesc )
{
    assert(pDesc);
    *pDesc = This->desc;
    return S_OK;
}

HRESULT WINAPI
DXGIAdapter_CheckInterfaceSupport( struct DXGIAdapter *This,
                                   REFGUID InterfaceName,
                                   LARGE_INTEGER *pUMDVersion )
{
    if (InterfaceName == IID_ID3D11Device) {
        /* This value stems from Win7 with Catalyst 10.8, meaning unclear. */
        pUMDVersion->QuadPart = 0x00080011000a0411ULL;
    } else
    if (InterfaceName == IID_ID3D10Device1 ||
        InterfaceName == IID_ID3D11Device) {
        return Error(DXGI_ERROR_UNSUPPORTED, "No one uses D3D10 !\n");
    } else {
        return Error(DXGI_ERROR_UNSUPPORTED, "Unknown interface.\n");
    }
    return S_OK;
}

IDXGIAdapterVtbl DXGIAdapter_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)DXGIObject_SetPrivateData,
    (void *)DXGIObject_SetPrivateDataInterface,
    (void *)DXGIObject_GetPrivateData,
    (void *)DXGIObject_GetParent,
    (void *)DXGIAdapter_EnumOutputs,
    (void *)DXGIAdapter_GetDesc,
    (void *)DXGIAdapter_CheckInterfaceSupport
};

static const GUID *DXGIAdapter_IIDs[] = {
    &IID_IDXGIAdapter,
    &IID_IDXGIObject,
    &IID_IUnknown,
    NULL
};

HRESULT
DXGIAdapter_new( struct D3D11Device *pDevice,
struct DXGIAdapter **ppOut )
{
    D3D11_NEW(DXGIAdapter, ppOut, pDevice);
}

