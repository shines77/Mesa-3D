
#define INTERP_CASE(a, b, c) case SM4_INTERPOLATION_##a: \
   ret.Interpolate = TGSI_INTERPOLATE_##b; ret.Centroid = c; break;
#define SEMANTIC_CASE(a, b, c) case D3D_NAME_##a: \
    ret.Name = TGSI_SEMANTIC_##b; ret.Index = c; break;

static INLINE struct tgsi_declaration_interp
convert_interpolation(enum sm4_interpolation mode)
{
    struct tgsi_declaration_interp ret;
    switch (mode) {
    INTERP_CASE(CONSTANT, CONSTANT, 0);
    INTERP_CASE(LINEAR, PERPSETICVE, 0);
    INTERP_CASE(LINEAR_CENTROID, PERPSETICVE, 1);
    INTERP_CASE(LINEAR_NOPERSPECTIVE_CENTROID, LINEAR, 1);
    INTERP_CASE(LINEAR_SAMPLE, PERSPECTIVE, 0);
    INTERP_CASE(LINEAR_NOPERSPECTIVE_SAMPLE, LINEAR, 0);
    default:
    INTERP_CASE(UNDEFINED, PERSPECTIVE, 0);
    }
    return ret;
}

static INLINE struct tgsi_declaration_semantic
convert_fs_semantic(const D3D11_SIGNATURE_PARAMETER_DESC *parm)
{
    struct tgsi_declaration_semantic ret;
    switch (parm->SystemValueType) {
    SEMANTIC_CASE(UNDEFINED, COLOR);
    SEMANTIC_CASE(POSITION, POSITION);
    SEMANTIC_CASE(VERTEX_ID, VERTEXID);
    SEMANTIC_CASE(PRIMITIVE_ID, PRIMID);
    SEMANTIC_CASE(INSTANCE_ID, INSTANCEID);
    SEMANTIC_CASE(IS_FRONT_FACE, FACE);
    SEMANTIC_CASE(DEPTH, POSITION);
    SEMANTIC_CASE(DEPTH_GREATER_EQUAL, POSITION);
    SEMANTIC_CASE(DEPTH_LESS_EQUAL, POSITION);
    default:
        break;
    }
    return ret;
}

static INLINE struct tgsi_declaration_semantic
convert_vs_semantic(const D3D11_SIGNATURE_PARAMETER_DESC *parm)
{
    struct tgsi_declaration_semantic ret;
    switch (parm->SystemValueType) {
    SEMANTIC_CASE(UNDEFINED, GENERIC);
    SEMANTIC_CASE(CULL_DISTANCE, CLIPDIST);
    SEMANTIC_CASE(CLIP_DISTANCE, CULLDIST);
    SEMANTIC_CASE(RENDER_TARGET_ARRAY_INDEX, LAYER);
    SEMANTIC_CASE(VIEWPORT_ARRAY_INDEX, VIEWPORTINDEX);
    SEMANTIC_CASE(TARGET, COLOR);
    SEMANTIC_CASE(COVERAGE, SAMPLEMASK);
    default:
        return convert_fs_semantic(parm);
    }
    return ret;
}

static INLINE unsigned convert_rrtype(D3D_RESOURCE_RETURN_TYPE type)
{
    switch (type) {
    case D3D_RETURN_TYPE_UNORM: return PIPE_TYPE_UNORM;
    case D3D_RETURN_TYPE_SNORM: return PIPE_TYPE_SNORM;
    case D3D_RETURN_TYPE_SINT: return PIPE_TYPE_SINT;
    case D3D_RETURN_TYPE_UINT: return PIPE_TYPE_UINT;
    case D3D_RETURN_TYPE_FLOAT: return PIPE_TYPE_FLOAT;
    case D3D_RETURN_TYPE_MIXED:
    case D3D_RETURN_TYPE_DOUBLE:
    case D3D_RETURN_TYPE_CONTINUED:
    default:
        assert(0);
        return 0;
    }
}

static INLINE unsigned convert_target(enum sm4_target target)
{
   switch (target) {
   case SM4_TARGET_TEXTURE1D: return TGSI_TEXTURE_1D;
   case SM4_TARGET_TEXTURE2D: return TGSI_TEXTURE_2D;
   case SM4_TARGET_TEXTURE2DMS: return TGSI_TEXTURE_2D_MSAA;
   case SM4_TARGET_TEXTURE3D: return TGSI_TEXTURE_3D;
   case SM4_TARGET_TEXTURECUBE: return TGSI_TEXTURE_CUBE;
   case SM4_TARGET_TEXTURE1DARRAY: return TGSI_TEXTURE_1D_ARRAY;
   case SM4_TARGET_TEXTURE2DARRAY: return TGSI_TEXTURE_2D_ARRAY;
   case SM4_TARGET_TEXTURE2DMSARRAY: return TGSI_TEXTURE_2D_ARRAY_MSAA;
   case SM4_TARGET_TEXTURECUBEARRAY: return TGSI_TEXTURE_CUBE_ARRAY;
   case SM4_TARGET_BUFFER: return TGSI_TEXTURE_BUFFER;
   case SM4_TARGET_RAW_BUFFER: return TGSI_TEXTURE_BUFFER;
   case SM4_TARGET_STRUCTURED_BUFFER: return TGSI_TEXTURE_BUFFER;
   case SM4_TARGET_UNKNONW:
   default:
      return TGSI_TEXTURE_UNKNOWN;
   }
}

static INLINE unsigned convert_semantic(enum sm4_sv sv)
{
   switch (sv) {
   SEMANTIC_CASE(POSITION, POSITION, 0);
   SEMANTIC_CASE(CLIP_DISTANCE, CLIPDIST, i);
   SEMANTIC_CASE(CULL_DISTANCE, CULLDIST, i);
   SEMANTIC_CASE(RENDER_TARGET_ARRAY_INDEX, LAYER, 0);
   SEMANTIC_CASE(VIEWPORT_ARRAY_INDEX, VIEWPORT_INDEX, 0);
   SEMANTIC_CASE(VERTEX_ID, VERTEXID, 0);
   SEMANTIC_CASE(PRIMITIVE_ID, PRIMID, 0);
   SEMANTIC_CASE(INSTANCE_ID, INSTANCEID, 0);
   SEMANTIC_CASE(IS_FRONT_FACE, FACE, 0);
   SEMANTIC_CASE(SAMPLE_INDEX, COUNT, 0);
   SEMANTIC_CASE(FINAL_QUAD_U_EQ_0_EDGE_TESSFACTOR, COUNT, 0);
   SEMANTIC_CASE(FINAL_QUAD_V_EQ_0_EDGE_TESSFACTOR, COUNT, 1);
   SEMANTIC_CASE(FINAL_QUAD_U_EQ_1_EDGE_TESSFACTOR, COUNT, 2);
   SEMANTIC_CASE(FINAL_QUAD_V_EQ_1_EDGE_TESSFACTOR, COUNT, 3);
   SEMANTIC_CASE(FINAL_QUAD_U_INSIDE_TESSFACTOR, COUNT, 4);
   SEMANTIC_CASE(FINAL_QUAD_V_INSIDE_TESSFACTOR, COUNT, 5);
   SEMANTIC_CASE(FINAL_TRI_U_EQ_0_EDGE_TESSFACTOR, COUNT, 0);
   SEMANTIC_CASE(FINAL_TRI_V_EQ_0_EDGE_TESSFACTOR, COUNT, 1);
   SEMANTIC_CASE(FINAL_TRI_W_EQ_0_EDGE_TESSFACTOR, COUNT, 2);
   SEMANTIC_CASE(FINAL_TRI_INSIDE_TESSFACTOR, COUNT, 4);
   SEMANTIC_CASE(FINAL_LINE_DETAIL_TESSFACTOR, COUNT, 0);
   SEMANTIC_CASE(FINAL_LINE_DENSITY_TESSFACTOR, COUNT, 4);
   default:
   SEMANTIC_CASE(UNDEFINED, COUNT, 0);
   }
}

#define D3D11_MAX_LOOP_DEPTH 64
#define D3D11_MAX_COND_DEPTH 64
#define D3D11_MAX_INPUTS     32
#define D3D11_MAX_OUTPUTS    32

struct sm4_to_tgsi
{
    struct {
        struct ureg_src v[D3D11_MAX_INPUTS];
        struct ureg_dst o[D3D11_MAX_OUTPUTS];
        struct ureg_src sv[TGSI_SEMANTIC_COUNT];
    } regs;

    uint8_t icb_slot;

    unsigned loop_depth;
    unsigned loop_depth_max;
    unsigned cond_depth;
    unsigned loop_labels[D3D11_MAX_LOOP_DEPTH];
    unsigned cond_labels[D3D11_MAX_COND_DEPTH];
};

static INLINE unsigned *
tx_cond(struct sm4_to_tgsi *tx)
{
    assert(tx->cond_depth <= D3D11_MAX_COND_DEPTH);
    tx->cond_depth++;
    return &tx->cond_labels[tx->cond_depth - 1];
}

static INLINE void
tx_endcond(struct sm4_to_tgsi *tx)
{
    assert(tx->cond_depth);
    tx->cond_depth--;
    ureg_fixup_label(tx->ureg, tx->cond_labels[tx->cond_depth],
                     ureg_get_instruction_number(tx->ureg));
}

static INLINE unsigned *
tx_bgnloop(struct sm4_to_tgsi *tx)
{
    tx->loop_depth++;
    if (tx->loop_depth_max < tx->loop_depth)
        tx->loop_depth_max = tx->loop_depth;
    assert(tx->loop_depht < D3D11_MAX_LOOP_DEPTH);
    return &tx->loop_labels[tx->loop_depth - 1];
}

static INLINE unsigned *
tx_endloop(struct sm4_to_tgsi *tx)
{
    assert(tx->loop_depth);
    tx->loop_depth--;
    ureg_fixup_label(tx->ureg, tx->loop_labels[tx->loop_depth],
                     ureg_get_instruction_number(tx->ureg));
    return &tx->loop_labels[tx->loop_depth];
}


static void
sm4_tgsi_build_vs(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;

    for (i = 0; i < prog->sig.num_in; ++i) {
        ureg_DECL_vs_input(ureg, i);
    }

    ureg_property(ureg, TGSI_PROPERTY_VS_PROHIBIT_UCPS, TRUE);
}

static void
sm4_tgsi_build_hs(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;

    ureg_property(ureg, TGSI_PROPERTY_VS_PROHIBIT_UCPS, TRUE);
    ureg_property(ureg, TGSI_PROPERTY_HS_INPUT_SIZE, prog->prop.hs.input_size);
    ureg_property(ureg, TGSI_PROPERTY_HS_OUTPUT_SIZE, prog->prop.hs.input_size);
}

static void
sm4_tgsi_build_ds(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;

    ureg_property(ureg, TGSI_PROPERTY_VS_PROHIBIT_UCPS, TRUE);
    ureg_property(ureg, TGSI_PROPERTY_DS_DOMAIN, domain);
    ureg_property(ureg, TGSI_PROPERTY_DS_PARTITIONING, part);
    ureg_property(ureg, TGSI_PROPERTY_DS_OUTPUT_PRIM, output_prim);
}

static void
sm4_tgsi_build_gs(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;
    unsigned input_prim;
    unsigned output_prim;

    ureg_property(ureg, TGSI_PROPERTY_GS_INPUT_PRIM, input_prim);
    ureg_property(ureg, TGSI_PROPERTY_GS_OUTPUT_PRIM, output_prim);
    ureg_property(ureg, TGSI_PROPERTY_GS_MAX_OUTPUT_VERTICES, prog->prop.gs.max_vertices);
    ureg_property(ureg, TGSI_PROPERTY_VS_PROHIBIT_UCPS, TRUE);
    ureg_property(ureg, TGSI_PROPERTY_GS_INSTANCE_COUNT, prog->prop.gs.instance_count);
}

static void
sm4_tgsi_build(struct sm4_to_tgsi *tx)
{
    unsigned i;

    for (i = 0; i < PIPE_MAX_SAMPLERS; ++i)
        if (prog->regs.samplers_used & (1 << i))
            ureg_DECL_sampler(ureg, i);

    for (i = 0; i < Elements(prog->resource); ++i) {
        if (prog->resource[i].target == SM4_TARGET_UNKNOWN)
            continue;
        ureg_DECL_sampler_view(
            ureg, i,
            convert_target(prog->resource[i].target),
            convert_rrtype(prog->resource[i].return_type.X),
            convert_rrtype(prog->resource[i].return_type.Y),
            convert_rrtype(prog->resource[i].return_type.Z),
            convert_rrtype(prog->resource[i].return_type.W));
    }
}

static void
sm4_tgsi_build_fs(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;

    for (i = 0; i < prog->sig.num_in; ++i) {
        struct tgsi_declaration_interp interp;
        interp = convert_interpolation(prog->regs.interp[i]);
        ureg_DECL_fs_input_cyl_centroid(ureg,
                                        TGSI_SEMANTIC_GENERIC,
                                        r,
                                        interp.Interpolate,
                                        0,
                                        interp.Centroid);
    }
    for (i = 0; i < prog->sig.num_out; ++i) {
        struct tgsi_declaration_semantic sem;
        sem = convert_semantic(prog->sig.out[i]);
        ureg_DECL_output(ureg, sem.Semantic, sem.SemanticIndex);
    }

    ureg_property(ureg, TGSI_PROPERTY_FS_COORD_ORIGIN, TGSI_FS_COORD_ORIGIN_UPPER_LEFT);
    ureg_property(ureg, TGSI_PROPERTY_FS_COORD_PIXEL_CENTER, TGSI_FS_COORD_PIXEL_CENTER_HALF_INTEGER);
    ureg_property(ureg, TGSI_PROPERTY_FS_DEPTH_LAYOUT, prop->fs.depth_layout);
    ureg_property(ureg, TGSI_PROPERTY_FS_EARLY_FRAGMENT_TESTS, prop->fs.early_tests);
}

static void
sm4_tgsi_build_cs(struct sm4_tgsi *tx)
{
    struct ureg_program *ureg = tx->ureg;
    unsigned i;
}


#define OPCODE_HANDLER(n) static HRESULT oh_##n(struct sm4_to_tgsi *tx, struct sm4_op_info *info)

OPCODE_HANDLER(NULL)
{
    struct ureg_dst dst[2];
    struct ureg_src src[5];
    int i;

    for (i = 0; i < info->ndst && i < Elements(dst); ++i)
        dst[i] = sm4_dst(tx, &tx->insn->dst[i]);
    for (i = 0; i < info->nsrc && i < Elements(src); ++i)
        src[i] = sm4_src(tx, &tx->insn->src[i]);

    ureg_insn(tx->ureg, info->tgsi,
              dst, info->ndst,
              src, info->nsrc);

    return S_OK;
}

OPCODE_HANDLER(EMITTHENCUT)
{
    ureg_EMIT(tx->ureg);
    ureg_ENDPRIM(tx->ureg);
}

OPCODE_HANDLER(BREAKC_CALLC_CONTINUEC_RETC)
{
    struct ureg_src src = sm4_src(tx->insn.src[0]);

    if (tx->insn->Insn.Insn.TestNZ) {
        ureg_UIF(tx->ureg, src, tx_cond(tx));
    } else {
        struct ureg_dst r = tx_scratch_scalar(tx);
        ureg_USEQ(tx->ureg, r, src, ureg_imm1u(tx->ureg, 0));
        ureg_UIF(tx->ureg, tx_src_scalar(r), tx_cond(tx));
    }
    if (tx->insn->opcode == SM4_OPCODE_CALLC) {
        ureg_CAL();
    } else {
        ureg_insn(info->tgsi);
    }
    tx_endcond(tx);
    ureg_ENDIF(tx->ureg);
}

OPCODE_HANDLER(SCALAR_TO_COMPONENTWISE)
{
    struct ureg_dst dst = sm4_dst(tx->insn.dst[0]);
    struct ureg_src src = sm4_src(tx->insn.src[0]);
    int c;
    const uint8_t mask = dst.Mask;

    for (c = 0; c < 4; ++c) {
        if (!(mask & (1 << c)))
            continue;
        dst.Mask = 1 << c;
        ureg_insn(tx->ureg, info->tgsi, &dst, 1, &ureg_scalar(src, c), 1);
    }
}

OPCODE_HANDLER(IMUL_UMUL)
{
    struct ureg_dst dst[2];
    struct ureg_src src[2];
    int n;

    for (n = 0; n < 2; ++n) {
        dst[n] = sm4_dst(tx->insn.dst[n]);
        src[n] = sm4_src(tx->insn.src[n]);
    }
    ureg_insn(tx->ureg, info->tgsi, dst, 2, src, 2);

    if (!ureg_dst_is_undef(tx->insn.dst[1])) {
        assert(tx->insn->Insn.Opcode != SM4_OPCODE_IMUL);
    }
    return S_OK;
}

OPCODE_HANDLER(UDIV)
{
    struct ureg_dst dst[2];
    struct ureg_src src[2];
    int n;

    for (n = 0; n < 2; ++n) {
        dst[n] = sm4_dst(tx->insn.dst[n]);
        src[n] = sm4_src(tx->insn.src[n]);
    }
    if (!ureg_dst_is_undef(dst[0]))
        ureg_UDIV(tx->ureg, dst[0], src[0], src[1]);
    if (!ureg_dst_is_undef(dst[1]))
        ureg_UMOD(tx->ureg, dst[1], src[1], src[1]);

    return S_OK;
}

OPCODE_HANDLER(SINCOS)
{
    struct ureg_dst dst_sin = sm4_dst(tx->insn.dst[0]);
    struct ureg_dst dst_cos = sm4_dst(tx->insn.dst[1]);
    struct ureg_src src = sm4_src(tx->insn.src[0]);
    int c;
    const uint8_t sin_mask = dst_sin.Mask;
    const uint8_t cos_mask = dst_cos.Mask;

    for (c = 0; c < 4; ++c) {
        if (!ureg_is_dst_undef(dst_sin) && (sin_mask & (1 << c))) {
            dst_sin.Mask = 1 << c;
            ureg_SIN(tx->ureg, dst_sin, ureg_scalar(src, c));
        }
        if (!ureg_is_dst_undef(dst_cos) && (cos_mask & (1 << c))) {
            dst_cos.Mask = 1 << c;
            ureg_COS(tx->ureg, dst_cos, ureg_scalar(src, c));
        }
    }
}

OPCODE_HANDLER(IF)
{
    struct ureg_src cnd = sm4_src(&tx->insn.src[0]);

    if (!tx->insn->Insn.Insn.TestNZ) {
        struct ureg_dst tmp = tx_scratch_scalar(tx);
        ureg_USEQ(tx->ureg, tmp, ureg_imm1u(tx->ureg, 0));
        cnd = tx_src_scalar(tmp);
    }
    ureg_UIF(tx->ureg, cnd, tx_cond(tx));
    return S_OK;
}
OPCODE_HANDLER(ENDIF)
{
    tx_endcond(tx);
    ureg_ENDIF(tx->ureg);
    return S_OK;
}

OPCODE_HANDLER(LOOP)
{
    ureg_BGNLOOP(tx->ureg, tx_bgnloop(tx));
    return S_OK;
}
OPCODE_HANDLER(ENDLOOP)
{
    ureg_ENDLOOP(tx->ureg, tx_endloop(tx));
    return S_OK;
}

OPCODE_HANDLER(TEXTURE)
{
    struct ureg_src src[5];
    struct ureg_src off = ureg_src_undef();
    struct ureg_dst dst;
    unsigned i;

    dst = sm4_dst(&tx->insn.dst[0]);

    if (tx->insn->Insn.Extended) {
        if (tx->insn->Ext.SampleControl.OffsetU ||
            tx->insn->Ext.SampleControl.OffsetV ||
            tx->insn->Ext.SampleControl.OffsetW)
            off = ureg_imm3u(tx->ureg,
                             tx->insn->Ext.SampleControl.OffsetU,
                             tx->insn->Ext.SampleControl.OffsetV,
                             tx->insn->Ext.SampleControl.OffsetW);
    }
    for (i = 0; i < info->num_src; ++i)
        src[i] = sm4_src(&tx->insn.src[i]);

    ureg_sample_insn(tx->ureg, info->tgsi, &offset, dst, src, 1, info->num_src);
}

OPCODE_HANDLER(STUB)
{
    return Warn(S_OK, "STUB: %s\n", sm4_opcode_str(info->opcode));
}

struct sm4_op_info
{
   enum sm4_opcode opcode;
   unsigned tgsi;
   unsigned num_dst;
   unsigned num_src;
};

static struct ureg_src
sm4_tgsi_sv(struct sm4_to_tgsi *tx, unsigned sn, unsigned si)
{
    switch (sn) {
    case TGSI_SEMANTIC_BLOCK_ID:
    case TGSI_SEMANTIC_BLOCK_SIZE:
    case TGSI_SEMANTIC_FACE:
    case TGSI_SEMANTIC_GRID_SIZE:
    case TGSI_SEMANTIC_INSTANCEID:
    case TGSI_SEMANTIC_PRIMID:
    case TGSI_SEMANTIC_THREAD_ID:
    case TGSI_SEMANTIC_VERTEXID:
    default:
        if (ureg_src_is_undef(tx->regs.sv[sn])) {
            int n = tx->regs.num_sv++;
            tx->regs.sv[sn] = ureg_DECL_system_value(tx->ureg, n, sn, si);
        }
        return tx->regs.sv[sn];
    }
}

static struct ureg_src
sm4_src(struct sm4_to_tgsi *, const struct sm4_operand *);

static struct ureg_src
sm4_ureg_src(struct sm4_to_tgsi *tx,
             const struct sm4_operand *op, unsigned file, int d)
{
    struct ureg_src src;

    assert(op->Index[0].Disp >= -65536 && op->index[0].Disp <= 65535);
    assert(op->Index[d].Disp >= -65536 && op->index[d].Disp <= 65535);

    src = ureg_src_register(file, op->Index[dim].Disp);
    if (op->Index[d].Reg)
        src = ureg_src_indirect(src, sm4_src(tx, op->Index[d].Reg));

    if (d) {
        if (op->Index[0].Reg)
            src = ureg_src_dimension_indirect(src,
                                              sm4_src(tx, op->Index[0].Reg),
                                              op->Index[0].Disp);
        else
            src = ureg_src_dimension(src, op->Index[0].Disp);
    }
}

struct ureg_src
sm4_src(struct sm4_to_tgsi *tx, const struct sm4_operand *op)
{
    struct ureg_src src = ureg_src_undef();

    switch (op->File) {
    case SM4_FILE_CONSTANT_BUFFER:
        assert(!op->Index[0].Reg);
        src = sm4_ureg_src(tx, op, TGSI_FILE_CONSTANT, 1);
        break;
    case SM4_FILE_CYCLE_COUNTER:
        src = sm4_tgsi_sv(tx, TGSI_SEMANTIC_CYCLES);
        break;
    case SM4_FILE_FUNCTION_BODY:
    case SM4_FILE_FUNCTION_INPUT:
    case SM4_FILE_FUNCTION_TABLE:
    case SM4_FILE_IMMEDIATE32:
        src = ureg_imm4u(tx->ureg,
                         op->Value[0].u32, op->Value[1].u32,
                         op->Value[2].u32, op->Value[3].u32);
        break;
    case SM4_FILE_IMMEDIATE64:
        Error(E_NOTIMPL, "SM4_FILE_%u not implemented\n", op->File);
        break;
    case SM4_FILE_IMMEDIATE_CONSTANT_BUFFER:
        src = sm4_ureg_src(tx, op, TGSI_FILE_CONSTANT, 0);
        src = ureg_src_dimension(src, tx->icb_slot);
        break;
    case SM4_FILE_INDEXABLE_TEMP:
        src = sm4_ureg_src(tx, op, TGSI_FILE_TEMPORARY, 0);
        break;
    case SM4_FILE_INPUT:
        src = tx->regs.v[op->Index[0].Disp];
        break;
    case SM4_FILE_INPUT_CONTROL_POINT:
    case SM4_FILE_INPUT_COVERAGE_MASK:
    case SM4_FILE_INPUT_DOMAIN_POINT:
        break;
    case SM4_FILE_INPUT_FORK_INSTANCE_ID:
        src = tx->regs.forki;
        break;
    case SM4_FILE_INPUT_GS_INSTANCE_ID:
        src = sm4_tgsi_sv(tx, TGSI_SEMANTIC_INSTANCEID, 0);
        break;
    case SM4_FILE_INPUT_JOIN_INSTANCE_ID:
        src = tx->regs.joini;
        break;
    case SM4_FILE_INPUT_PATCH_CONSTANT:
        assert(tx->type == D3D_SHADER_TYPE_DOMAIN);
        src = tx->regs.pc[op->Index[0].Disp];
        break;
    case SM4_FILE_INPUT_PRIMITIVEID:
        src = sm4_tgsi_sv(tx, TGSI_SEMANTIC_PRIMID, 0);
        break;
    case SM4_FILE_INPUT_THREAD_GROUP_ID:
        src = sm4_tgsi_sv(tx, TGSI_SEMANTIC_BLOCK_ID, 0);
        break;
    case SM4_FILE_INPUT_THREAD_ID:
        break;
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP:
        src = sm4_tgsi_sv(tx, TGSI_SEMANTIC_THREAD_ID, 0);
        break;
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP_FLATTENED:
    case SM4_FILE_INTERFACE:
    case SM4_FILE_LABEL:
    case SM4_FILE_OUTPUT_CONTROL_POINT:
    case SM4_FILE_OUTPUT_CONTROL_POINT_ID:
    case SM4_FILE_RESOURCE:
    case SM4_FILE_SAMPLER:
    case SM4_FILE_STREAM:
    case SM4_FILE_TEMP:
        src = tx->regs.t[op->Index[0].Disp];
        break;
    case SM4_FILE_THIS_POINTER:
        src = tx->regs.pthis;
        break;
    case SM4_FILE_THREAD_GROUP_SHARED_MEMORY:
        dst = tx_scratch(tx);
        src = ureg_src(dst);
        ureg_LOAD(dst, tx->regs.rLocal);
        break;
    case SM4_FILE_UNORDERED_ACCESS_VIEW:
        src = tx->regs.uav[op->Index[0].Disp];
        break;

    case SM4_FILE_FUNCTION_OUTPUT:
    case SM4_FILE_OUTPUT:
    case SM4_FILE_OUTPUT_COVERAGE_MASK:
    case SM4_FILE_OUTPUT_DEPTH:
    case SM4_FILE_OUTPUT_DEPTH_GREATER_EQUAL:
    case SM4_FILE_OUTPUT_DEPTH_LESS_EQUAL:
        Error(E_INVALIDARG, "invalid src operand file: %u\n", op->File);
        break;
    default:
        break;
    }

    switch (op->Mode) {
    case op->Mode == SM4_OPERAND_MODE_SWIZZLE:
        src = ureg_swizzle(src,
                           op->Swizzle[0], op->Swizzle[1],
                           op->Swizzle[2], op->Swizzle[3]);
        break;
    case op->Mode == SM4_OPERAND_MODE_SCALAR:
        src = ureg_scalar(src, op->Swizzle[0]);
        break;
    case op->Mode == SM4_OPERAND_MODE_MASK:
        /* immediates are masked to indicate used values */
        assert(op->File == SM4_FILE_IMMEDIATE32 ||
               op->File == SM4_FILE_IMMEDIATE64);
        break;
    default:
        assert(0);
        break;
    }

    if (op->Absolute)
        src = ureg_abs(src);
    if (op->Negate)
        src = ureg_negate(src);

    return src;
}

struct ureg_dst
sm4_dst(struct sm4_to_tgsi *tx, const struct sm4_operand *op)
{
    struct ureg_dst dst = ureg_dst_undef();

    switch (op.File) {
    case SM4_FILE_FUNCTION_BODY:
    case SM4_FILE_FUNCTION_OUTPUT:
    case SM4_FILE_FUNCTION_TABLE:
    case SM4_FILE_INDEXABLE_TEMP:
    case SM4_FILE_INTERFACE:
        break;
    case SM4_FILE_OUTPUT:
        dst = tx->regs.o[op->Index[0].Disp];
        break;
    case SM4_FILE_OUTPUT_CONTROL_POINT:
    case SM4_FILE_OUTPUT_CONTROL_POINT_ID:
        break;
    case SM4_FILE_OUTPUT_COVERAGE_MASK:
        dst = tx->regs.oCov;
        break;
    case SM4_FILE_OUTPUT_DEPTH:
    case SM4_FILE_OUTPUT_DEPTH_GREATER_EQUAL:
    case SM4_FILE_OUTPUT_DEPTH_LESS_EQUAL:
        dst = tx->regs.oDepr;
        break;
    case SM4_FILE_STREAM:
    case SM4_FILE_TEMP:
        dst = tx->regs.r[op->Index[0].Disp];
        break;
    case SM4_FILE_THREAD_GROUP_SHARED_MEMORY:
    case SM4_FILE_UNORDERED_ACCESS_VIEW:
        break;

    case SM4_FILE_INPUT_PATCH_CONSTANT:
        assert(tx->type == D3D_SHADER_TYPE_HULL);
        dst = tx->regs.pc[op->Index[0].Disp];
        break;

    case SM4_FILE_CONSTANT_BUFFER:
    case SM4_FILE_CYCLE_COUNTER:
    case SM4_FILE_FUNCTION_INPUT:
    case SM4_FILE_IMMEDIATE32:
    case SM4_FILE_IMMEDIATE64:
    case SM4_FILE_IMMEDIATE_CONSTANT_BUFFER:
    case SM4_FILE_INPUT:
    case SM4_FILE_INPUT_CONTROL_POINT:
    case SM4_FILE_INPUT_COVERAGE_MASK:
    case SM4_FILE_INPUT_DOMAIN_POINT:
    case SM4_FILE_INPUT_FORK_INSTANCE_ID:
    case SM4_FILE_INPUT_GS_INSTANCE_ID:
    case SM4_FILE_INPUT_JOIN_INSTANCE_ID:
    case SM4_FILE_INPUT_PRIMITIVEID:
    case SM4_FILE_INPUT_THREAD_GROUP_ID:
    case SM4_FILE_INPUT_THREAD_ID:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP_FLATTENED:
    case SM4_FILE_LABEL:
    case SM4_FILE_RESOURCE:
    case SM4_FILE_SAMPLER:
    case SM4_FILE_THIS_POINTER:
        Error(E_INVALIDARG, "invalid dst operand file: %u\n", op->File);
        break;
    default:
        break;
    }

    dst = ureg_writemask(dst, op->Mask);

    if (tx->insn->Insn.Insn.Saturate)
        dst = ureg_saturate(dst);

    return dst;
}


#define _(m,n,d,s,h) { SM4_OPCODE_##m, TGSI_OPCODE_##n, d, s, oh_##h }

unsigned sm4_to_tgsi_opcode[] =
{
   _(BREAK,     BRK,     0, 0, NULL),
   _(BREAKC,    BREAKC,  0, 1, BREAKC_CALLC_CONTINUEC_RETC),
   _(CALL,      CAL,     0, 0, NULL),
   _(CALLC,     CAL,     0, 1, BREAKC_CALLC_CONTINUEC_RETC),
   _(CONTINUE,  CONT,    0, 0, NULL),
   _(CONTINUEC, CONT,    0, 1, BREAKC_CALLC_CONTINUEC_RETC),
   _(DISCARD,   KILL,    0, 0, NULL),
   _(ENDLOOP,   ENDLOOP, 0, 0, NULL),
   _(ELSE,      ELSE,    0, 0, NULL),
   _(ENDIF,     ENDIF,   0, 0, NULL),
   _(IF,        IF,      0, 0, NULL),
   _(LABEL,     LAST,    0, 0, NULL),
   _(LOOP,      BGNLOOP, 0, 0, NULL),
   _(RET,       RET,     0, 0, NULL),
   _(RETC,      RET,     0, 1, BREAKC_CALLC_CONTINUEC_RETC),

   _(CUT,         ENDPRIM, 0, 0, NULL),
   _(EMIT,        EMIT,    0, 0, NULL),
   _(EMITTHENCUT, EMIT,    0, 0, NULL),

   _(DERIV_RTX, DDX, 1, 1, NULL),
   _(DERIV_RTY, DDY, 1, 1, NULL),

   _(ADD,         ADD, 1, 2, NULL),
   _(AND,         AND, 1, 2 NULL),
   _(DIV,         DIV, 1, 2, NULL),
   _(DP2,         DP2, 1, 2, NULL),
   _(DP3,         DP3, 1, 2, NULL),
   _(DP4,         DP4, 1, 2, NULL),
   _(EXP,         EX2, 1, 1, NULL),
   _(FRC,         FRC, 1, 1, NULL),
   _(IADD,        UADD, 1, 2, NULL),
   _(IMAD,        UMAD, 1, 3, NULL),
   _(IMAX,        IMAX, 1, 2, NULL),
   _(IMIN,        IMIN, 1, 2, NULL),
   _(IMUL,        UMUL, 1, 2, NULL),
   _(INEG,        INEG, 1, 1, NULL),
   _(ISHL,        SHL, 1, 2, NULL),
   _(ISHR,        ISHR, 1, 2, NULL),
   _(LOG,         LG2, 1, 1, NULL),
   _(MAD,         MAD, 1, 3, NULL),
   _(MIN,         MIN, 1, 2, NULL),
   _(MAX,         MAX, 1, 2, NULL),
   _(CUSTOMDATA,  LAST, 0, 0, NULL),
   _(MOV,         MOV, 1, 1, NULL),
   _(MOVC,        UCMP, 1, 3, NULL),
   _(MUL,         MUL, 1, 2, NULL),
   _(NOP,         NOP, 0, 0, NULL),
   _(NOT,         NOT, 1, 1, NULL),
   _(OR,          OR, 1, 2, NULL),
   _(RESINFO,     SVIEWINFO, 0, 0, NULL),
   _(XOR,         XOR,    1, 2, NULL),
   _(UDIV,        UDIV,   1, 2, NULL),
   _(UMUL,        UMUL,   1, 2, NULL),
   _(UMAD,        UMAD,   1, 3, NULL),
   _(UMAX,        UMAX,   1, 2, NULL),
   _(UMIN,        UMIN,   1, 2, NULL),
   _(USHR,        USHR,   1, 2, NULL),

   _(FTOI,        F2I, 1, 1, NULL),
   _(FTOU,        F2U, 1, 1, NULL),
   _(ITOF,        I2F, 1, 1, NULL),
   _(UTOF,        U2F,    1, 1, NULL),

   _(EQ,  FSEQ, 1, 2, NULL),
   _(NE,  FSNE, 1, 2, NULL),
   _(LT,  FSLT, 1, 2, NULL),
   _(GE,  FSGE, 1, 2, NULL),
   _(IEQ, USEQ, 1, 2, NULL),
   _(INE, USNE, 1, 2, NULL),
   _(ILT, ISLT, 1, 2, NULL),
   _(IGE, ISGE, 1, 2, NULL),
   _(ULT, USLT, 1, 2, NULL),
   _(UGE, USGE, 1, 2, NULL),

   _(ROUND_NE, ROUND, 1, 1, NULL),
   _(ROUND_NI, FLOOR, 1, 1, NULL),
   _(ROUND_PI, CEIL,  1, 1, NULL),
   _(ROUND_Z,  TRUNC, 1, 1, NULL),

   _(LD,          SAMPLE_I,    0, 0, TEXTURE),
   _(LD_MS,       SAMPLE_I_MS, 0, 0, TEXTURE),
   _(SAMPLE,      SAMPLE,      0, 0, TEXTURE),
   _(SAMPLE_C,    SAMPLE_C,    0, 0, TEXTURE),
   _(SAMPLE_C_LZ, SAMPLE_C_LZ, 0, 0, TEXTURE),
   _(SAMPLE_L,    SAMPLE_L,    0, 0, TEXTURE),
   _(SAMPLE_D,    SAMPLE_D,    0, 0, TEXTURE),
   _(SAMPLE_B,    SAMPLE_B,    0, 0, TEXTURE),

   _(RCP,  RCP,  1, 1, SCALAR_TO_COMPONENTWISE),
   _(RSQ,  RSQ,  1, 1, SCALAR_TO_COMPONENTWISE),
   _(SQRT, SQRT, 1, 1, SCALAR_TO_COMPONENTWISE),

   _(SWITCH,    SWITCH,    0, 0, NULL),
   _(ENDSWITCH, ENDSWITCH, 0, 0, NULL),
   _(CASE,      CASE,      0, 1, NULL),
   _(DEFAULT,   DEFAULT,   0, 0, NULL),

   _(SINCOS, SCS, 2, 1, SINCOS),

   _(DCL_RESOURCE,                     LAST, 0, 0, NULL),
   _(DCL_CONSTANT_BUFFER,              LAST, 0, 0, NULL),
   _(DCL_SAMPLER,                      LAST, 0, 0, NULL),
   _(DCL_INDEX_RANGE,                  LAST, 0, 0, NULL),
   _(DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY, LAST, 0, 0, NULL),
   _(DCL_GS_INPUT_PRIMITIVE,           LAST, 0, 0, NULL),
   _(DCL_MAX_OUTPUT_VERTEX_COUNT,      LAST, 0, 0, NULL),
   _(DCL_INPUT,                        LAST, 0, 0, NULL),
   _(DCL_INPUT_SGV,                    LAST, 0, 0, NULL),
   _(DCL_INPUT_SIV,                    LAST, 0, 0, NULL),
   _(DCL_INPUT_PS,                     LAST, 0, 0, NULL),
   _(DCL_INPUT_PS_SGV,                 LAST, 0, 0, NULL),
   _(DCL_INPUT_PS_SIV,                 LAST, 0, 0, NULL),
   _(DCL_OUTPUT,                       LAST, 0, 0, NULL),
   _(DCL_OUTPUT_SGV,                   LAST, 0, 0, NULL),
   _(DCL_OUTPUT_SIV,                   LAST, 0, 0, NULL),
   _(DCL_TEMPS,                        LAST, 0, 0, NULL),
   _(DCL_INDEXABLE_TEMP,               LAST, 0, 0, NULL),
   _(DCL_GLOBAL_FLAGS,                 LAST, 0, 0, NULL),

   _(D3D10_COUNT, LAST, 0, 0, NULL),

   _(LOD,         LAST,        0, 0, NULL),
   _(GATHER4,     GATHER4,     0, 0, NULL),
   _(SAMPLE_POS,  SAMPLE_POS,  0, 0, NULL),
   _(SAMPLE_INFO, SAMPLE_INFO, 0, 0, NULL),

   _(D3D10_1_COUNT, LAST, 0, 0, NULL),

   _(HS_DECLS,               LAST, 0, 0, NULL),
   _(HS_CONTROL_POINT_PHASE, LAST, 0, 0, NULL),
   _(HS_FORK_PHASE,          LAST, 0, 0, NULL),
   _(HS_JOIN_PHASE,          LAST, 0, 0, NULL),

   _(EMIT_STREAM,        EMIT,      0, 0, NULL),
   _(CUT_STREAM,         ENDPRIM,   0, 0, NULL),
   _(EMITTHENCUT_STREAM, EMIT,      0, 0, NULL),
   _(INTERFACE_CALL,     CAL,       0, 0, NULL),
   _(BUFINFO,            SVIEWINFO, 0, 0, NULL),
   _(DERIV_RTX_COARSE,   DDX,       1, 1, NULL),
   _(DERIV_RTX_FINE,     DDX,       1, 1, NULL),
   _(DERIV_RTY_COARSE,   DDY,       1, 1, NULL),
   _(DERIV_RTY_FINE,     DDY,       1, 1, NULL),
   _(GATHER4_C,          GATHER4,   0, 0, NULL),
   _(GATHER4_PO,         GATHER4,   0, 0, NULL),
   _(GATHER4_PO_C,       GATHER4,   0, 0, NULL),
   _(F32TOF16,           LAST,      0, 0, NULL),
   _(F16TOF32,           LAST,      0, 0, NULL),
   _(UADDC,              UADD,      1, 2, NULL),
   _(USUBB,              UADD,      1, 2, NULL),
   _(COUNTBITS,          POPC,      0, 0, NULL),
   _(FIRSTBIT_HI,        BSF,       1, 1, NULL),
   _(FIRSTBIT_LO,        BSR,       1, 1, NULL),
   _(FIRSTBIT_SHI,       LAST,      0, 0, NULL),
   _(UBFE,               UBFE,      0, 0, NULL),
   _(IBFE,               IBFE,      0, 0, NULL),
   _(BFI,                BFI,       0, 0, NULL),
   _(BFREV,              BFREV,     0, 0, NULL),
   _(SWAPC,              SWAPC,     0, 0, NULL),

   _(DCL_STREAM,                                LAST, 0, 0, NULL), 
   _(DCL_FUNCTION_BODY,                         BGNSUB, 0, 0, NULL), 
   _(DCL_FUNCTION_TABLE,                        LAST, 0, 0, NULL), 
   _(DCL_INTERFACE,                             LAST, 0, 0, NULL), 
   _(DCL_INPUT_CONTROL_POINT_COUNT,             LAST, 0, 0, NULL), 
   _(DCL_OUTPUT_CONTROL_POINT_COUNT,            LAST, 0, 0, NULL), 
   _(DCL_TESS_DOMAIN,                           LAST, 0, 0, NULL), 
   _(DCL_TESS_PARTITIONING,                     LAST, 0, 0, NULL), 
   _(DCL_TESS_OUTPUT_PRIMITIVE,                 LAST, 0, 0, NULL), 
   _(DCL_HS_MAX_TESSFACTOR,                     LAST, 0, 0, NULL), 
   _(DCL_HS_FORK_PHASE_INSTANCE_COUNT,          LAST, 0, 0, NULL), 
   _(DCL_HS_JOIN_PHASE_INSTANCE_COUNT,          LAST, 0, 0, NULL), 
   _(DCL_THREAD_GROUP,                          LAST, 0, 0, NULL), 
   _(DCL_UNORDERED_ACCESS_VIEW_TYPED,           LAST, 0, 0, NULL), 
   _(DCL_UNORDERED_ACCESS_VIEW_RAW,             LAST, 0, 0, NULL), 
   _(DCL_UNORDERED_ACCESS_VIEW_STRUCTURED,      LAST, 0, 0, NULL), 
   _(DCL_THREAD_GROUP_SHARED_MEMORY_RAW,        LAST, 0, 0, NULL), 
   _(DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED, LAST, 0, 0, NULL), 
   _(DCL_RESOURCE_RAW,                          LAST, 0, 0, NULL), 
   _(DCL_RESOURCE_STRUCTURED,                   LAST, 0, 0, NULL), 

   _(LD_UAV_TYPED,     LOAD,  0, 0, NULL),
   _(STORE_UAV_TYPED,  STORE, 0, 0, NULL),
   _(LD_RAW,           LOAD,  0, 0, NULL),
   _(STORE_RAW,        STORE, 0, 0, NULL),
   _(LD_STRUCTURED,    LOAD,  0, 0, NULL),
   _(STORE_STRUCTURED, STORE, 0, 0, NULL),

   _(ATOMIC_AND,       ATOMAND,  0, 0, NULL),
   _(ATOMIC_OR,        ATOMOR,   0, 0, NULL),
   _(ATOMIC_XOR,       ATOMXOR,  0, 0, NULL),
   _(ATOMIC_CMP_STORE, ATOMCAS,  0, 0, NULL),
   _(ATOMIC_IADD,      ATOMUADD, 0, 0, NULL),
   _(ATOMIC_IMAX,      ATOMIMAX, 0, 0, NULL),
   _(ATOMIC_IMIN,      ATOMIMIN, 0, 0, NULL),
   _(ATOMIC_UMAX,      ATOMUMAX, 0, 0, NULL),
   _(ATOMIC_UMIN,      ATOMUMIN, 0, 0, NULL),

   _(IMM_ATOMIC_ALLOC,    LAST, 0, 0, NULL),
   _(IMM_ATOMIC_CONSUME,  LAST, 0, 0, NULL),
   _(IMM_ATOMIC_IADD,     LAST, 0, 0, NULL),
   _(IMM_ATOMIC_AND,      LAST, 0, 0, NULL),
   _(IMM_ATOMIC_OR,       LAST, 0, 0, NULL),
   _(IMM_ATOMIC_XOR,      LAST, 0, 0, NULL),
   _(IMM_ATOMIC_EXCH,     LAST, 0, 0, NULL),
   _(IMM_ATOMIC_CMP_EXCH, LAST, 0, 0, NULL),
   _(IMM_ATOMIC_IMAX,     LAST, 0, 0, NULL),
   _(IMM_ATOMIC_IMIN,     LAST, 0, 0, NULL),
   _(IMM_ATOMIC_UMAX,     LAST, 0, 0, NULL),
   _(IMM_ATOMIC_UMIN,     LAST, 0, 0, NULL),

   _(SYNC, BARRIER, 0, 0, NULL),

   _(DADD,  DADD, 1, 2, NULL),
   _(DMAX,  DMAX, 1, 2, NULL),
   _(DMIN,  DMIN, 1, 2, NULL),
   _(DMUL,  DMUL, 1, 2, NULL),
   _(DEQ,   DSEQ, 1, 2, NULL),
   _(DGE,   DSGE, 1, 2, NULL),
   _(DLT,   DSLT, 1, 2, NULL),
   _(DNE,   DSNE, 1, 2, NULL),
   _(DMOV,  MOV,  1, 1, NULL),
   _(DMOVC, UCMP, 1, 3, NULL),
   _(DTOF,  D2F,  1, 1, NULL),
   _(FTOD,  F2D,  1, 1, NULL),

   _(EVAL_SNAPPED,      LAST, 0, 0, NULL),
   _(EVAL_SAMPLE_INDEX, LAST, 0, 0, NULL),
   _(EVAL_CENTROID,     LAST, 0, 0, NULL),

   _(DCL_GS_INSTANCE_COUNT, LAST, 0, 0, NULL),
};

void process_sm4()
{
    struct sm4_token_instruction *insn;

    while (insn->get())
        insn->process();
}

