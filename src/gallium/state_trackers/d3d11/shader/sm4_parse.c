
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
        uint32_t CompsEnum  : 2;
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

union sm4_token_operand_ext
{
    uint32_t b32;
    struct {
        uint32_t Type     : 6;
        uint32_t Negate   : 1;
        uint32_t Absolute : 1;
    };
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
struct sm4_operand;

struct sm4_operand_index
{
    struct sm4_operand *Reg;
    int64_t Disp;
};

struct sm4_operand
{
    unsigned File       : 8;
    unsigned Mask       : 4;
    unsigned NumComps   : 3;
    unsigned Mode       : 3;
    unsigned NumIndices : 2;
    unsigned Absolute   : 1;
    unsigned Negate     : 1;

    uint8_t Swizzle[4];

    union {
        struct sm4_operand_index Index[3];
        union sm4_numeric Value[4];
    };
};

struct sm4_declaration
{
    uint32_t *Data;
    struct sm4_token_return_type ReturnType;
    union {
        uint32_t SV;
        uint32_t BaseIndex;
        struct {
            uint32_t ID;
            uint32_t TableLengthFn;
            uint16_t TableLength;
            uint16_t ArrayLength;
        } Iface;
    };
    uint32_t Num;
    uint8_t NumComps;
};

struct sm4_instruction
{
    struct sm4_token_instruction Insn;
    struct sm4_token_instruction_ext Ext;
    struct sm4_operand Opnd[7];
    struct sm4_declaration Dcl;
};

static void
sm4_operand_free(struct sm4_operand *opnd, boolean free)
{
    unsigned r;

    for (r = 0; r < opnd->NumIndices; ++r)
        if (opnd->Index[r].Reg)
            sm4_operand_free(opnd->Index[r].Reg, TRUE);

    if (free)
        FREE(opnd);
}

static void
sm4_instruction_free(struct sm4_instruction *insn)
{
    unsigned i;

    if (insn->Dcl.Data)
        FREE(insn->Dcl.Data);

    for (i = 0; i < insn->NumOperands; ++i)
        sm4_operand_free(&insn->Opnd[i], FALSE);
}

struct sm4_program
{
    struct ureg_program *ureg;

    struct sm4_token_version version;

    struct sm4_instruction *insn;
    unsigned num_insn;
    unsigned max_insn;

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
        uint8_t target;
        uint8_t nr_samples;
        struct sm4_token_return_type return_type;
    } resource[256];

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

    prog->info = blob;

    prog->sig.in = blob->in;
    prog->sig.out = blob->out;
    prog->sig.pc = blob->pc;

    return prog;
}

static void
sm4_program_delete(struct sm4_program *prog)
{
    unsigned i;

    for (i = 0; i < prog->num_insn; ++i)
        sm4_instruction_free(&prog->insn[i]);

    if (prog->insn) FREE(prog->decl);

    if (prog->sig.in) FREE(prog->sig.in);
    if (prog->sig.out) FREE(prog->sig.out);
    if (prog->sig.pc) FREE(prog->sig.pc);

    FREE(prog);
}

static HRESULT
sm4_program_add_insn(struct sm4_program *prog, struct sm4_instruction *insn)
{
    if (prog->num_insn == prog->max_insn) {
        prog->insn = REALLOC(prog->insn,
                             prog->max_insn * 1 * sizeof(*prog->insn);
                             prog->max_insn * 2 * sizeof(*prog->insn));
        if (!prog->insn)
            return Error(E_OUTOFMEMORY, "");
    }
    prog->insn[prog->num_insn++] = *insn;
    return S_OK;
}

static INLINE boolean TOK_END(struct sm4_parser *parse)
{
    return parse->toks == parse->toks_end;
}
static INLINE uint32_t TOK_READ32(struct sm4_parser *parse)
{
    uint32_t tok = LE32_TO_CPU(parse->toks[0]);
    parse->toks++;
    return tok;
}
static INLINE uint64_t TOK_READ64(struct sm4_parser *parse)
{
    uint32_t tokl = TOK_READ32(parse);
    uint32_t tokh = TOK_READ32(parse);
    return ((uint64_t)tokh << 32) | tokl;
}
static INLINE void TOK_SKIP(struct sm4_parser *parse, unsigned n)
{
    parse->toks += n;
}
static INLINE void TOK_JMP(struct sm4_parser *parse)
{
    assert(parse->tok_next);
    if (parse->toks != parse->tok_next) {
        WARN("skipping %u tokens\n", parse->tok_next - parse->toks);
        parse->toks = parse->tok_next;
    }
    parse->tok_next = NULL;
}
static INLINE boolean TOK_NEXTI(struct sm4_parsrer *parse)
{
    return parse->toks == parse->toks_next;
}
static INLINE void TOK_SETJMP(struct sm4_parser *parse, unsigned n)
{
    parse->tok_next = parse->toks + n;
}

static struct sm4_operand *
sm4_parse_operand(struct sm4_parser *parse, struct sm4_operand *opnd)
{
    union sm4_token_operand tok = { .b32 = TOK_READ32(parse); }
    unsigned i;

    if (!opnd)
        opnd = CALLOC_STRUCT(sm4_operand);

    assert(tok.File < SM4_FILE_COUNT);
    opnd->File = tok.File;
    opnd->Mask = 0xf;
    opnd->NumIndices = tok.NumIndices;

    switch (tok.CompsEnum) {
    case SM4_OPERAND_COMPNUM_0:
        assert(opnd->NumComps == 0);
        break;
    case SM4_OPERAND_COMPNUM_1:
        opnd->NumComps = 1;
        break;
    case SM4_OPERAND_COMPNUM_4:
        opnd->NumComps = 4;
        opnd->Mode = tok.Mode;
        switch (tok.Mode) {
        case SM4_OPERAND_MODE_MASK:
            opnd->Mask = tok.Select & 0xf;
            break;
        case SM4_OPERAND_MODE_SWIZZLE:
            opnd->Swizzle[0] = (tok.Select >> 0) & 0x3;
            opnd->Swizzle[1] = (tok.Select >> 2) & 0x3;
            opnd->Swizzle[2] = (tok.Select >> 4) & 0x3;
            opnd->Swizzle[3] = (tok.Select >> 6) & 0x3;
            break;
        case SM4_OPERAND_MODE_SCALAR:
            for (i = 0; i < 4; ++i)
                opnd->Swizzle[i] = tok.Select & 0x3;
            break;
        default:
            assert(0);
            break;
        }
        break;
    case SM4_OPERAND_COMPNUM_N:
        return Error(E_NOTIMPL, "");
    default:
        assert(0);
        break;
    }

    if (tok.Extended) {
        union sm4_token_operand_ext ext = { .b32 = TOK_READ32(parse) }
        switch (ext.Type) {
        case 0:
            break;
        case 1:
            opnd->Absolute = ext.Absolute;
            opnd->Negate = ext.Negate;
            break;
        default:
            return Error(E_NOTIMPL, "");
        }
    }

    for (i = 0; i < tok.NumIndices; ++i) {
        unsigned r;
        if (i == 0) r = tok.Index0Repr; else
        if (i == 1) r = tok.Index1Repr; else
        if (i == 2) r = tok.Index2repr; else return Error(E_NOTIMPL, "");
        switch (r) {
        case SM4_OPERAND_INDEX_REPR_IMM32:
            opnd->Index[i].Disp = TOK_READ32(parse);
            break;
        case SM4_OPERAND_INDEX_REPR_IMM64:
            opnd->Index[i].Disp = TOK_READ64(parse);
            break;
        case SM4_OPERAND_INDEX_REPR_REG:
            opnd->Index[i].Disp = 0;
            opnd->Index[i].Reg = sm4_parse_operand(parse, NULL);
            break;
        case SM4_OPERAND_INDEX_REPR_REG_IMM32:
            opnd->Index[i].Disp = TOK_READ32(parse);
            opnd->Index[i].Reg = sm4_parse_operand(parse, NULL);
            break;
        case SM4_OPERAND_INDEX_REPR_REG_IMM64:
            opnd->Index[i].Disp = TOK_READ64(parse);
            opnd->Index[i].Reg = sm4_parse_operand(parse, NULL);
            break;
        }
    }

    if (opnd->File == SM4_FILE_IMMEDIATE32) {
        assert(!tok.NumIndices);
        for (i = 0; i < opnd.NumComps; ++i)
            opnd.Value[i].u32 = TOK_READ32(parse);
    } else
    if (opnd->File == SM4_FILE_IMMEDIATE64) {
        assert(!tok.NumIndices);
        for (i = 0; i < opnd.NumComps; ++i)
            opnd.Value[i].u64 = TOK_READ64(parse);
    }

    return opnd;
}

static HRESULT
sm4_parse_instruction(struct sm4_parser *parse)
{
    struct sm4_token_instruction *insn = &parse->insn.Insn;
    boolean keep = FALSE;
    struct sm4_token_instruction_ext ext;
    uint32_t value;
    struct sm4_program *prog = parse->prog;

    insn.b32 = TOK_READ32(parse);

    TOK_SETJMP(parse, insn.Length - 1);

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
        prog->prop.hs.max_tessfactor = asfloat(TOK_READ32(parse));
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
        prog->prop.gs.instance_count = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY:
        prog->prop.gs.output_topology = insn->DclGSOutputPrimitiveTopology.Topology;
        break;
    case SM4_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT:
        prog->prop.gs.max_vertices = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_THREAD_GROUP:
        prog->prop.cs.block_size[0] = TOK_READ32(parse);
        prog->prop.cs.block_size[1] = TOK_READ32(parse);
        prog->prop.cs.block_size[2] = TOK_READ32(parse);
        break;

    case SM4_OPCODE_CUSTOMDATA:
        prog->icb_size = (TOK_READ32(parse) - 2) * 4;
        prog->icb_data = MALLOC(prog->icb_size);
        if (!prog->icb_data)
            return E_OUTOFMEMORY;
        memcpy(prog->icb_data, parse->tok, prog->icb_size);
        TOK_SKIP(prog->icb_size / 4);
        break;

    /* Markers: */
    case SM4_OPCODE_HS_FORK_PHASE:
    case SM4_OPCODE_HS_JOIN_PHASE:
        keep = TRUE;
        break;

    /* Registers and resources: */
    case SM4_OPCODE_DCL_TEMPS:
        prog->regs.num_temps += TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_INDEXABLE_TEMP:
        i = prog->regs.num_arrays++;
        if (prog->regs.num_arrays >= prog->regs.max_arrays) {
            prog->regs.max_arrays += 8;
            prog->regs.array = REALLOC(prog->regs.array,
                (prog->regs.max_array - 8) * sizeof(prog->regs.array[0]),
                (prog->regs.max_array + 0) * sizeof(prog->regs.array[0]));
        }
        prog->regs.array[i].Size = TOK_READ32(parse);
        prog->regs.array[i].Comp = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_RESOURCE:
        sm4_parse_operand(&insn->Opnd[0], parse);
        i = insn->Opnd[0].Index[0].Disp;
        assert(i < Elements(prog->resource));
        prog->resource[i].return_type = TOK_READ32(parse);
        prog->resource[i].target = insn->DclResource.Target;
        prog->resource[i].nr_samples = insn->DclResource.NrSamples;
        break;
    case SM4_OPCODE_DCL_SAMPLER:
        sm4_parse_operand(&insn->Opnd[0], parse);
        i = insn->Opnd[0].Index[0].Disp;
        assert(i < 16);
        prog->regs.samplers_used |= 1 << i;
        if (insn->DclSampler.Shadow)
            prog->regs.samplers_shadow |= 1 << i;
        if (insn->DclSampler.Mono)
            prog->regs.samplers_mono |= 1 << i;
        break;
    case SM4_OPCODE_DCL_INPUT:
    case SM4_OPCODE_DCL_INPUT_PS:
        sm4_parse_operand(&insn->Opnd[0], parse);
        if (IS_PS)
            prog->regs.input[i].interp = insn->DclInputPS.Interpolation;
        break;
    case SM4_OPCODE_DCL_INPUT_SIV:
    case SM4_OPCODE_DCL_INPUT_SGV:
    case SM4_OPCODE_DCL_INPUT_PS_SIV:
    case SM4_OPCODE_DCL_INPUT_PS_SGV:
        sm4_parse_operand(&insn->Opnd[0], parse);
        if (IS_PS)
            prog->regs.input[i].interp = insn->DclInputPS.Interpolation;
        prog->regs.input[i].sv = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_OUTPUT:
        sm4_parse_operand(&insn->Opnd[0], parse);
        break;
    case SM4_OPCODE_DCL_OUTPUT_SIV:
    case SM4_OPCODE_DCL_OUTPUT_SGV:
        sm4_parse_operand(&insn->Opnd[0], parse);
        prog->regs.output[i].sv = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_INDEX_RANGE:
        sm4_parse_operand(&insn->Opnd[0], parse);
        insn->Dcl.Num = TOK_READ32(parse);
        break;

    /* Slightly crazy stuff: */
    case SM4_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED:
        break;
    case SM4_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW:
        break;
    case SM4_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED:
        break;
    case SM4_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_RAW:
        break;
    case SM4_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED:
        break;
    case SM4_OPCODE_DCL_RESOURCE_RAW:
        break;
    case SM4_OPCODE_DCL_RESOURCE_STRUCTURED:
        break;
    case SM4_OPCODE_DCL_STREAM:
        break;

    /* Crazy stuff: */
    case SM4_OPCODE_DCL_FUNCTION_BODY:
        insn->Dcl.Num = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_FUNCTION_TABLE:
        insn->Dcl.Num = TOK_READ32(parse);
        insn->Dcl.Data = (uint32_t *)MALLOC(insn->Dcl.Num * sizeof(uint32_t));
        for (i = 0; i < insn->Dcl.Num; ++i)
            insn->Dcl.Data[i] = TOK_READ32(parse);
        break;
    case SM4_OPCODE_DCL_INTERFACE:
        insn->Dcl.Iface.ID = TOK_READ32(parse);
        insn->Dcl.Iface.TableLengthFn = TOK_READ32(parse);
        value = TOK_READ32(parse);
        insn->Dcl.Iface.ArrayLength = value >> 16;
        insn->Dcl.Iface.TableLength = value & 0xffff;
        value &= 0xffff;
        insn->Dcl.Data = (uint32_t *)MALLOC(value * sizeof(uint32_t));
        for (i = 0; i < insn->Dcl.Iface.TableLength; ++i)
            insn->Dcl.Data[i] = TOK_READ32(parse);
        break;

    default:
        ext.b32 = insn->b32;
        while (ext.Extended) {
            ext.b32 = TOK_READ32(parse);
            switch (ext.Type) {
            case SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_SAMPLE_CONTROLS:
                break;
            case SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_RESOURCE_DIM:
                break;
            case SM4_TOKEN_INSTRUCTION_EXTENDED_TYPE_RESOURCE_RETURN_TYPE:
                break;
            default:
                break;
            }
        }

        if (insn->Opcode == SM4_OPCODE_INTERFACE_CALL) {
            insn->Dcl.Num = TOK_READ32(parse);
        }

        while (!TOK_NEXTI(parse)) {
            if (insn->NumOperands >= Elements(insn->Opnd))
                return Error(D3DERR_DRIVERINTERNALERROR, "static allocation");
            sm4_parse_operand(&insn->Opnd[insn->NumOperands++], parse);
        }
        keep = TRUE;
    }
    TOK_JMP(parse);

    return keep ? sm4_program_add_insn(prog, insn) : S_OK;
}

struct sm4_program *
sm4_parse(d3d11_shader_blob *info)
{
    struct sm4_parser parse;

    parse.prog = sm4_program_create(info);
    if (!parse.prog)
        return NULL;
    parse.tok = info->bytecode.toks;

    parse.prog->version.b32 = TOK_READ32(&parse);
    if (parse.prog->version.Major < 4 || parse.prog->version.Major > 5) {
        ERROR("invalid shader version: %u.%u\n",
              parse.prog->version.Major,
              parse.prog->version.Minor);
        goto fail;
    }

    parse.toks_length = TOK_READ32(&parse);
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
