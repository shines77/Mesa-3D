
#include "dxbc.h"

#define FOURCC_DXBC MAKEFOURCC('D', 'X', 'B', 'C')
#define FOURCC_RDEF MAKEFOURCC('R', 'D', 'E', 'F')
#define FOURCC_ISGN MAKEFOURCC('I', 'S', 'G', 'N')
#define FOURCC_OSGN MAKEFOURCC('O', 'S', 'G', 'N')
#define FOURCC_SHDR MAKEFOURCC('S', 'H', 'D', 'R')
#define FOURCC_SHEX MAKEFOURCC('S', 'H', 'E', 'X')
#define FOURCC_STAT MAKEFOURCC('S', 'T', 'A', 'T')
#define FOURCC_PCSG MAKEFOURCC('P', 'C', 'S', 'G')

#define FOURCC_CHARS(x) ((x) >>  0) & 0xff, ((x) >>  8) & 0xff, ((x) >> 16) & 0xff, ((x) >> 24) & 0xff)


struct dxbc_chunk_header
{
    uint32_t FourCC;
    uint32_t Size;
};

struct dxbc_chunk_signature
{
    struct dxbc_chunk_header Header;
    uint32_t Count;
    uint32_t Unk;
    struct {
        uint32_t NameOffset;
        uint32_t SemanticIndex;
        uint32_t SystemValueType;
        uint32_t ComponentType;
        uint32_t RegisterNum;
        uint8_t Mask;
        uint8_t ReadWriteMask;
        uint8_t Stream;
        uint8_t Unused;
    } Element[];
};

struct dxbc_container_header
{
    uint32_t FourCC;
    uint32_t Unk[4];
    uint32_t One;
    uint32_t TotalSize;
    uint32_t ChunkCount;
    uint32_t ChunkOffset[0];
};

struct d3d11_shader_info
{
    D3D11_SHADER_VERSION_TYPE type;
    struct {
        uint8_t major;
        uint8_t minor;
    } version;

    uint8_t num_inputs;
    uint8_t num_outputs;
    uint8_t num_pcs;
    struct {
        D3D11_SIGNATURE_PARAMETER_DESC *in;
        D3D11_SIGNATURE_PARAMETER_DESC *out;
        D3D11_SIGNATURE_PARAMETER_DESC *pc;
    } sig;

    struct {
        D3D11_TESSELLATOR_DOMAIN domain;
        D3D11_TESSELLATOR_PARTITIONING partitioning;
        D3D11_TESSELLATOR_OUTPUT_PRIMITIVE output;
    } tess;

    struct {
        uint8_t *toks;
        uint32_t size;
    } bytecode;
};

static unsigned
dxbc_parse_signature(const struct dxbc_chunk_header *chunk, D3D11_SIGNATURE_PARAMETER_DESC **sig)
{
    const struct dxbc_chunk_signature *data = (void *)chunk;

    *sig = MALLOC(data->Count * sizeof(D3D11_SIGNATURE_PARAMETER_DESC));
    if (!*sig)
        return 0;
    for (i = 0; i < data->Count; ++i) {
        (*sig)[i].SemanticName = (char *)&data->Count + LE32_TO_CPU(data->Element[i].NameOffset);
        (*sig)[i].SemanticIndex = data->Element[i].SemanticIndex;
        (*sig)[i].Register = data->Element[i].RegisterNum;
        (*sig)[i].SystemValueType = data->Element[i].SystemValueType;
        (*sig)[i].ComponentType = data->Element[i].ComponentType;
        (*sig)[i].Mask = data->Element[i].Mask;
        (*sig)[i].ReadWriteMask = data->Element[i].ReadWriteMask;
    }
    return data->Count;
}

HRESULT
dxbc_parse(struct d3d11_shader_blob *info, const uint8_t *bytecode, SIZE_T size)
{
    struct dxbc_container_header *hdr = bytecode;
    struct dxbc_chunk_header *chunk;
    unsigned num_chunks;
    unsigned i;

    if (hdr->FourCC != FOURCC_DXBC)
        return E_INVALIDARG;

    num_chunks = LE32_TO_CPU(hdr->ChunkCount);
    for (i = 0; i < num_chunks; ++i) {
        chunk = (void *)(bytecode + LE32_TO_CPU(hdr->ChunkOffset[i]));

        switch (LE32_TO_CPU(chunk->FourCC)) {
        case FOURCC_DXBC:
        case FOURCC_RDEF:
        case FOURCC_STAT:
            MESSAGE("ignored chunk: %c%c%c%c\n", FOURCC_CHARS(chunk->FourCC));
            break;
        case FOURCC_ISGN:
            info->num_inputs = dxbc_parse_signature(chunk, &info->in);
            break;
        case FOURCC_OSGN:
            info->num_outputs = dxbc_parse_signature(chunk, &info->out);
            break;
        case FOURCC_PCSG:
            info->num_pcs = dxbc_parse_signature(chunk, &info->pc);
            break;
        case FOURCC_SHDR:
        case FOURCC_SHEX:
            info->bytecode.toks = (uint8_t *)&chunk[1];
            info->bytecode.size = LE32_TO_CPU(chunk->Size);
            break;
        default:
            MESSAGE("unrecognized DXBC chunk: %c%c%c%c\n", FOURCC_CHARS(chunk->FourCC);
            break;
        }
    }

    return S_OK;
}
