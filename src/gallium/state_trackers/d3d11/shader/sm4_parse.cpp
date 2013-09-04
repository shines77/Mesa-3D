
union sm4_token_version
{
    uint32_t b32;
    struct {
        uint32_t Minor  : 4;
        uint32_t Major  : 4;
        uint32_t Format : 8;
        uint32_t Type   : 16;
    };
};

union sm4_token_instruction
{
    uint32_t b32;
    struct {
        uint32_t Opcode   : 11;
        uint32_t Data     : 13;
        uint32_t Length   : 7;
        uint32_t Extended : 1;
    };
    struct {
        uint32_t Opcode            : 11;
        uint32_t ResinfoReturnType : 2;
        uint32_t Saturate          : 1;
        uint32_t Unused            : 4;
        uint32_t TestNZ            : 1;
        uint32_t PrecisionMask     : 4;
    } Insn;
    struct {
        uint32_t Opcode         : 11;
        uint32_t ThreadsInGroup : 1;
        uint32_t SharedMemory   : 1;
        uint32_t UAVGroup       : 1;
        uint32_t UAVGlobal      : 1;
    } Sync;
    struct {
        uint32_t Opcode                        : 11;
        uint32_t AllowRefactoring              : 1;
        uint32_t FP64                          : 1;
        uint32_t EarlyFragmentTests            : 1;
        uint32_t EnableRawAndStructuredInNonCS : 1;
    } DclGlobalFlags;
    struct {
        uint32_t Opcode    : 11;
        uint32_t Target    : 5;
        uint32_t NrSamples : 7;
    } DclResource;
    struct {
        uint32_t Opcode : 11;
        uint32_t Shadow : 1;
        uint32_t Mono   : 1;
    } DclSampler;
    struct {
        uint32_t Opcode        : 11;
        uint32_t Interpolation : 5;
    } DclInputPS;
    struct {
        uint32_t Opcode  : 11;
        uint32_t Dynamic : 1;
    } DclConstBuf;
    struct {
        uint32_t Opcode    : 11;
        uint32_t Primitive : 6;
    } DclGSInputPrimitive;
    struct {
        uint32_t Opcode   : 11;
        uint32_t Topology : 7;
    } DclGSOutputPrimitiveTopology;
    struct {
        uint32_t Opcode : 11;
        uint32_t Count  : 6;
    } DclInputControlPointCount;
    struct {
        uint32_t Opcode : 11;
        uint32_t Count  : 6;
    } DclOutputControlPointCount;
    struct {
        uint32_t Opcode : 11;
        uint32_t Domain : 3;
    } DclTessDomain;
    struct {
        uint32_t Opcode       : 11;
        uint32_t Partitioning : 3;
    } DclTessPartitioning;
    struct {
        uint32_t Opcode    : 11;
        uint32_t Primitive : 3;
    } DclTessOutputPrimitive;
};

union sm4_token_instruction_ext
{
    uint32_t b32;
    struct {
        uint32_t Type     : 6;
        uint32_t Data     : 25;
        uint32_t Extended : 1;
    };
    struct {
        uint32_t Type    : 6;
        uint32_t Unused  : 3;
        uint32_t OffsetU : 4;
        uint32_t OffsetV : 4;
        uint32_t OffsetW : 4;
    } SampleControl;
    struct {
        uint32_t Type   : 6;
        uint32_t Target : 5;
    } ResourceTarget;
    struct {
        uint32_t Type : 6;
        uint32_t X    : 4;
        uint32_t Y    : 4;
        uint32_t Z    : 4;
        uint32_t W    : 4;
    } ReturnType;
};

union sm4_token_return_type
{
    uint32_t b32;
    struct {
        uint32_t X : 4;
        uint32_t Y : 4;
        uint32_t Z : 4;
        uint32_t W : 4;
    };
};

union sm4_token_operand
{
    uint32_t b32;
    struct {
        uint32_t Comps      : 2;
        uint32_t Mode       : 2;
        uint32_t Select     : 8;
        uint32_t File       : 8;
        uint32_t NumIndices : 2;
        uint32_t Index0Repr : 3;
        uint32_t Index1Repr : 3;
        uint32_t Index2Repr : 3;
        uint32_t Extended   : 1;
    };
};

struct sm4_token_operand_ext
{
    uint32_t Type     : 6;
    uint32_t Negate   : 1;
    uint32_t Absolute : 1;
};

union sm4_numeric
{
    double f64;
    float f32;
    int64_t s64;
    int32_t s32;
    uint64_t u64;
    uint32_t u32;
};

struct sm4_program;
struct sm4_operand;
struct sm4_declaration;
struct sm4_instruction;

struct sm4_operand
{
    uint8_t File;
    uint8_t Select;

    unsigned Mask     : 4;
    unsigned Absolute : 1;
    unsigned Negate   : 1;

    union sm4_numeric Value[4];
};

struct sm4_declaration
{
};

struct sm4_instruction
{
    struct sm4_token_instruction Insn;
    struct sm4_token_instruction_ext Ext;
    struct sm4_operand Dst[2];
    struct sm4_operand Src[4];
    struct sm4_declaration Dcl;
};

struct sm4_program
{
    struct ureg_program *ureg;

    struct sm4_token_version version;

    struct sm4_declaration *decl;
    struct sm4_instruction *insn;
    unsigned num_decl;
    unsigned num_insn;
    unsigned max_insn;
    unsigned max_decl;

    uint32_t *icb_data;
    uint32_t icb_size;
    
    struct {
        D3D11_SIGNATURE_PARAMETER_DESC *in;
        D3D11_SIGNATURE_PARAMETER_DESC *out;
        D3D11_SIGNATURE_PARAMETER_DESC *pc;
        uint8_t num_in;
        uint8_t num_out;
        uint8_t num_pc;
    } sig;

    union {
        struct {
            uint8_t input_prim;
            uint8_t output_prim;
        } gs;
        struct {
            uint8_t input_size;
            uint8_t output_size;
            float max_tessfactor;
        } hs;
        struct {
            uint8_t domain;
            uint8_t partitioning;
            uint8_t output_prim;
        } ds;
        struct {
            uint32_t instance_count;
            uint32_t max_vertices;
            uint8_t input_prim;
            uint8_t output_topology;
        } gs;
        struct {
            boolean early_tests;
        } fs;
        struct {
            uint32_t block_size[3];
        } cs;
    } prop;

    struct {
        unsigned num_temps;
        uint16_t samplers_used;
        uint16_t samplers_shadow;
        uint16_t samplers_mono;
        uint16_t constbuf_used;
        uint8_t interp_mode[PIPE_MAX_SHADER_INPUTS];
    } regs;

    struct {
        boolean allow_refactoring;
        boolean fp64;
        boolean raw_structured_en;
    } options;

    struct sm4_shader_blob *info;
};

struct sm4_parser
{
    struct sm4_program *prog;

    uint32_t *toks;
    uint32_t *toks_end;
    uint32_t *tok_next;
    uint32_t toks_length;

    struct sm4_instruction insn;
};

static struct sm4_program *
sm4_program_create(struct sm4_shader_blob *blob)
{
    struct sm4_program *prog = CALLOC_STRUCT(sm4_program);
    if (!prog)
        return NULL;

    prog->max_insn = 8;
    prog->insn = MALLOC(prog->max_insn * sizeof(*prog->insn));
    prog->max_decl = 4;
    prog->insn = MALLOC(prog->max_decl * sizeof(*prog->decl));

    prog->info = blob;

    prog->sig.in = blob->in;
    prog->sig.out = blob->out;
    prog->sig.pc = blob->pc;

    return prog;
}

static void
sm4_program_delete(struct sm4_program *prog)
{
    if (prog->decl) FREE(prog->decl);
    if (prog->insn) FREE(prog->decl);

    if (prog->sig.in) FREE(prog->sig.in);
    if (prog->sig.out) FREE(prog->sig.out);
    if (prog->sig.pc) FREE(prog->sig.pc);

    FREE(prog);
}

static void
sm4_program_add_insn(struct sm4_program *prog, struct sm4_instruction *insn)
{
    if (prog->num_insn == prog->max_insn) {
        prog->insn = REALLOC(prog->insn,
                             prog->max_insn * 1 * sizeof(*prog->insn);
                             prog->max_insn * 2 * sizeof(*prog->insn));
    }
    prog->insn[prog->num_insn++] = *insn;
}

static INLINE boolean TOK_END(struct sm4_parser *parse)
{
    return parse->toks == parse->toks_end;
}
static INLINE uint32_t TOK_NEXT32(struct sm4_parser *parse)
{
    uint32_t tok = LE32_TO_CPU(parse->toks[0]);
    parse->toks++;
    return tok;
}
static INLINE uint64_t TOK_NEXT64(struct sm4_parser *parse)
{
    uint32_t tokl = TOK_READ32(parse);
    uint32_t tokh = TOK_READ32(parse);
    return ((uint64_t)tokh << 32) | tokl;
}
static INLINE void TOK_SKIP(struct sm4_parser *parse, unsigned n)
{
    parse->toks += n;
}
static INLINE void TOK_JUMP(struct sm4_parser *parse)
{
    assert(parse->tok_next);
    if (parse->toks != parse->tok_next) {
        WARN("skipping %u tokens\n", parse->tok_next - parse->toks);
        parse->toks = parse->tok_next;
    }
    parse->tok_next = NULL;
}
static INLINE void TOK_MARK(struct sm4_parser *parse, unsigned n)
{
    parse->tok_next = parse->toks + n;
}

static void
sm4_parse_operand(struct sm4_parser *parse)
{
}

static void
sm4_parse_instruction(struct sm4_parser *parse)
{
}

static HRESULT
sm4_parse_next(struct sm4_parser *parse)
{
    struct sm4_token_instruction *insn = &parse->insn.Insn;
    struct sm4_token_instruction_ext ext;
    struct sm4_program *prog = parse->prog;

    insn.b32 = TOK_NEXT32(parse);

    TOK_MARK(parse, insn.Length - 1);

    switch (insn.Opcode) {
    /* Properties and options: */
    case SM4_OPCODE_DCL_GLOBAL_FLAGS:
        prog->options.allow_refactoring = insn->DclGlobalFlags.AllowRefactoring;
        prog->options.fp64 = insn->DclGlobalFlags.FP64;
        if (prog->type == D3D_SHADER_TYPE_PIXEL)
            prog->prop.ps.early_tests = insn->DclGlobalFlags.EarlyFragmentTests;
        if (prog->type != D3D_SHADER_TYPE_COMPUTE)
            prog->options.raw_structured_en =
                insn->DclGlobalFlags.EnableRawAndStructuredInNonCS;
        break;
    case SM4_OPCODE_DCL_HS_MAX_TESSFACTOR:
        prog->prop.hs.max_tessfactor = asfloat(TOK_NEXT32(parse));
        break;
    case SM4_OPCODE_DCL_INPUT_CONTROL_POINT_COUNT:
        prog->prop.hs.output_size = insn->DclOutputControlPointCount.Count;
        break;
    case SM4_OPCODE_DCL_OUTPUT_CONTROL_POINT_COUNT:
        prog->prop.hs.input_size = insn->DclInputControlPointCount.Count;
        break;
    case SM4_OPCODE_DCL_TESS_DOMAIN:
        prop->prop.ds.domain = insn->DclTessDomain.Domain;
        break;
    case SM4_OPCODE_DCL_TESS_PARTITIONING:
        prop->prop.ds.partitioning = insn->DclTessPartitioning.Partitioning;
        break;
    case SM4_OPCODE_DCL_TESS_OUTPUT_PRIMITIVE:
        prog->prop.ds.output_prim = insn->DclTessOutputPrimitive.Primitive;
        break;
    case SM4_OPCODE_DCL_GS_INPUT_PRIMITIVE:
        prog->prop.gs.input_prim = insn->DclGSInputPrimitive.Primitive;
        break;
    case SM4_OPCODE_DCL_GS_INSTANCE_COUNT:
        prog->prop.gs.instance_count = TOK_NEXT32(parse);
        break;
    case SM4_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY:
        prog->prop.gs.output_topology = insn->DclGSOutputPrimitiveTopology.Topology;
        break;
    case SM4_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT:
        prog->prop.gs.max_vertices = TOK_NEXT32(parse);
        break;
    case SM4_OPCODE_DCL_THREAD_GROUP:
        prog->prop.cs.block_size[0] = TOK_NEXT32(parse);
        prog->prop.cs.block_size[1] = TOK_NEXT32(parse);
        prog->prop.cs.block_size[2] = TOK_NEXT32(parse);
        break;

    case SM4_OPCODE_CUSTOMDATA:
        prog->icb_size = (TOK_NEXT32(parse) - 2) * 4;
        prog->icb_data = MALLOC(prog->icb_size);
        if (!prog->icb_data)
            return E_OUTOFMEMORY;
        memcpy(prog->icb_data, parse->tok, prog->icb_size);
        TOK_SKIP(prog->icb_size / 4);
        break;

    /* Registers and resources: */
    case SM4_OPCODE_DCL_TEMPS:
        prog->regs.num_temps += TOK_NEXT32(parse);
        break;
    case SM4_OPCODE_DCL_RESOURCE:
        break;
    case SM4_OPCODE_DCL_SAMPLER:
        break;
    case SM4_OPCODE_DCL_INPUT:
    case SM4_OPCODE_DCL_INPUT_PS:
        break;
    case SM4_OPCODE_DCL_INPUT_SIV:
    case SM4_OPCODE_DCL_INPUT_SGV:
    case SM4_OPCODE_DCL_INPUT_PS_SIV:
    case SM4_OPCODE_DCL_INPUT_PS_SGV:
        break;
    case SM4_OPCODE_DCL_OUTPUT:
        break;
    case SM4_OPCODE_DCL_OUTPUT_SIV:
    case SM4_OPCODE_DCL_OUTPUT_SGV:
        break;
    case SM4_OPCODE_DCL_INDEX_RANGE:
        break;
    case SM4_OPCODE_DCL_INDEXABLE_TEMP:
        break;

    /* Crazy stuff: */
    case SM4_OPCODE_DCL_INTERFACE:

    default:
        ext.b32 = insn->b32;
        while (ext.Extended:
            ext.b32 = TOK_NEXT32(parse);
            if (ext.Type == SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_SAMPLE_CONTROLS:
            } else
            if (ext.Type == SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_RESOURCE_DIM:
            } else
            if (ext.Type == SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_RESOURCE_RETURN_TYPE:
            }
        }

        if (insn->Opcode == SM4_OPCODE_INTERFACE_CALL:
        }

        while (:
            sm4_parse_operand(parse);
        }
    }
    TOK_JUMP(parse);

    return S_OK;
}

struct sm4_program *
sm4_parse(d3d11_shader_blob *info)
{
    struct sm4_parser parse;

    parse.prog = sm4_program_create(info);
    if (!parse.prog)
        return NULL;
    parse.tok = info->bytecode.toks;

    parse.prog->version.b32 = TOK_NEXT32(&parse);
    if (parse.prog->version.Major < 4 || parse.prog->version.Major > 5) {
        ERROR("invalid shader version: %u.%u\n",
              parse.prog->version.Major,
              parse.prog->version.Minor);
        goto fail;
    }

    parse.toks_length = TOK_NEXT32(&parse);
    parse.toks_end = parse.tok - 2 + parse.toks_length;

    info->version.major = parse.prog->version.Major;
    info->version.minor = parse.prog->version.Minor;
    info->type = parse.prog->version.Type;

    if (info->type > 5) {
        ERROR("invalid shader type: %u\n", info->kind);
        goto fail;
    }
    if (info->type == D3D_SHADER_TYPE_COMPUTE)
        parse.prog->options.raw_structured_en = TRUE;

    while (!TOK_END(&parse) && sm4_parse_next(&parse));
    if (!TOK_END(&parse))
        goto fail;

    return parse.prog;
fail:
    if (parse.prog)
        sm4_program_delete(parse.prog);
    return NULL;
}
