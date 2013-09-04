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

#include "inputlayout.h"

HRESULT
D3D11InputLayout_ctor( struct D3D11InputLayout *This,
                       struct D3D11UnknownParams *pParams,
                       D3D11_INPUT_ELEMENT_DESC *pElement,
                       UINT NumElements,
                       void *pBytecode,
                       SIZE_T BytecodeLength )
{
    struct D3D11Device *dev = pParams->device;
    HRESULT hr;
    struct d3d11_shader_blob blob;
    D3D11_SIGNATURE_PARAMETER_DESC *parm;
    unsigned num_parms;
    struct pipe_vertex_element in[PIPE_MAX_ATTRIBS];
    struct pipe_vertex_element ve[PIPE_MAX_ATTRIBS];
    unsigned i, k;

    hr = D3D11DeviceChild_ctor(&This->base, pParams);
    if (FAILED(hr))
        return hr;

    user_assert(pBytecode, E_INVALIDARG);

    hr = dxbc_parse(&blob, pBytecode,  BytecodeLength);
    if (FAILED(hr))
        return_error(hr);
    parm = blob.in;
    num_parms = blob.num_inputs;

    for (i = 0; i < NumElements; ++i) {
        ve[i].src_format = dxgi_to_pipe_format(pElement[i].Format);

        ve[i].instance_divisor = pElement[i].InstanceDataStepRate;
        if (pElement[i].InputSlotClass != D3D11_INPUT_PER_INSTANCE_DATA) {
            if (ve[i].instance_divisor != 0)
                return_error(E_INVALIDARG);
        } else
        if (ve[i].instance_divisor == 0) {
            ve[i].instance_divisor = ~0; /* XXX: can't specify "never" */
        }

        ve[i].vertex_buffer_index = pElement[i].InputSlot;
        if (ve[i].vertex_buffer_index >= PIPE_MAX_ATTRIBS) {
            return_error(E_INVALIDARG);
        }

        if (pElement[i].AlignedByteOffset == D3D11_APPEND_ALIGNED_ELEMENT) {
            if (i) {
                uint8_t size =
                    util_format_description(ve[i].src_format)->channel[0].size;
                if (size & 7) /* packed format like R10G10B10A2 */
                    size = 32;
                ve[i].src_offset = align(ve[i - 1].src_offset +
                    util_format_get_blocksize(ve[i - 1].src_format), size);
            } else {
                ve[i].src_offset = 0;
            }
        } else {
            ve[i].src_offset = pElement[i].AlignedByteOffset;
        }
    }

    for (i = 0; i < NumElements; ++i) {
        for (k = 0; k < num_parms; ++k) {
            if (parm[k].SemanticIndex == pElement[i].SemanticIndex &&
                !strcmp(parm[k].SemanticName, pElement[i].SemanticName))
                break;
        }
        if (k < num_parms) {
            in[i] = ve[k];
            /* check */
            {
                struct util_format_channel_description *ch;
                ch = &util_format_description(in[i].src_format)->channel[0];
                switch (parm[k].ComponentType) {
                case D3D_REGISTER_COMPONENT_UINT32:
                    if (ch->type != UTIL_FORMAT_TYPE_UNSIGNED || ch->normalized)
                        WARN("input element format mismatch\n");
                    break;
                case D3D_REGISTER_COMPONENT_SINT32:
                    if (ch->type != UTIL_FORMAT_TYPE_SIGNED || ch->normalized)
                        WARN("input element format mismatch\n");
                    break;
                case D3D_REGISTER_COMPONENT_FLOAT32:
                    if (ch->type != UTIL_FORMAT_TYPE_FLOAT && !ch->normalized)
                        WARN("input element format mismatch\n");
                    break;
                default:
                    break;
            }                
            }
            fd = 

            switch (
            switch (util_format_description(in[i].src_format)->channel[0].type) {
            case UTIL_FORMAT_TYPE_UNSIGNED:
            case UTIL_FORMAT_TYPE_SIGNED:
            case UTIL_FORMAT_TYPE_FLOAT:
            }
        } else {
            memset(&in[i], 0, sizeof(in[i]));
        }
    }
    This->cso = dev->pipe->create_vertex_elements_state(dev->pipe, NumElements,
                                                        in);
    if (!This->cso) {
        return_error(DXGI_ERROR_DRIVER_INTERNAL_ERROR);
    }

    return S_OK;
}

void
D3D11InputLayout_dtor( struct D3D11InputLayout *This )
{
    if (This->cso) {
        struct D3D11Device *dev = This->base.device;
        dev->pipe->delete_vertex_elements_state(dev->pipe, This->cso);
    }
    D3D11DeviceChild_dtor(&This->base);
}

ID3D11InputLayoutVtbl D3D11InputLayout_vtable = {
    (void *)D3D11Unknown_QueryInterface,
    (void *)D3D11Unknown_AddRef,
    (void *)D3D11Unknown_Release,
    (void *)D3D11DeviceChild_GetDevice,
    (void *)D3D11DeviceChild_GetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateData,
    (void *)D3D11DeviceChild_SetPrivateDataInterface
};

static const GUID *D3D11InputLayout_IIDs[] = {
    &IID_ID3D11InputLayout,
    &IID_ID3D11DeviceChild,
    &IID_IUnknown,
    NULL
};

HRESULT
D3D11InputLayout_new( struct D3D11Device *pDevice,
                      D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
                      UINT NumElements,
                      void *pShaderBytecodeWithInputSignature,
                      SIZE_T BytecodeLength,
                      struct D3D11InputLayout **ppOut )
{
    D3D11_NEW(D3D11InputLayout, ppOut, pDevice,
              pInputElementDescs, NumElements,
              pShaderByteCodeWithInputSignature, BytecodeLength);
}

