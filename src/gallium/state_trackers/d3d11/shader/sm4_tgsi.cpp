
struct g3d11_shader_info
{
   void *bytecode;
};

#define INTERP_CASE(a, b, c) case SM4_INTERPOLATION_##a: \
   tgsi.Interpolate = TGSI_INTERPOLATE_##b; tgsi.Centroid = c; break;
static INLINE void
translate_interpolation(struct tgsi_declaration_interp tgsi,
                        enum sm4_interpolation mode)
{
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
}

static INLINE unsigned
translate_target(enum sm4_target targ)
{
   switch (targ) {
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

static INLINE unsigned
translate_semantic(enum sm4_sv sv)
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

class ShaderTranslator
{
};


struct sm4_token_version
{
   unsigned Minor  : 4;
   unsigned Major  : 4;
   unsigned Format : 4;
   unsigned Type   : 16;
};

struct sm4_token_instruction
{
   // Length and Extended are always valid, but to reduce duplication they
   // aren not included in the named structs.
   struct {
      unsigned Opcode   : 11;
      unsigned _11_23   : 13;
      unsigned Length   : 7;
      unsigned Extended : 1;
   };

   struct {
      unsigned Opcode     : 11;
      unsigned ReturnType : 2; // RESINFO
      unsigned Saturate   : 1;
      unsigned TestNZ     : 1;
      unsigned Precision  : 4;
      unsigned _23        : 1;
   } Insn;

   struct {
      unsigned Opcode         : 11;
      unsigned ThreadsInGroup : 1;
      unsigned SharedMemory   : 1;
      unsigned UAVGroup       : 1;
      unsigned UAVGlobal      : 1;
      unsigned _15_17         : 3;
   } Sync;

   struct {
      unsigned Opcode                        : 11;
      unsigned AllowRefactoring              : 1;
      unsigned FP64                          : 1;
      unsigned EarlyFragmentTests            : 1;
      unsigned EnableRawAndStructuredInNonCS : 1;
   } DclGlobalFlags;

   struct {
      unsigned Opcode : 11;
      unsigned Target : 5;
      unsigned NrSamples : 7;
   } DclResource;

   struct {
      unsigned Opcode : 11;
      unsigned Shadow : 1;
      unsigned Mono   : 1;
   } DclSampler;

   struct {
      unsigned Opcode        : 11;
      unsigned Interpolation : 5;
   } DclInputPS;

   struct {
      unsigned Opcode : 11;
      unsigned Dynamic : 1;
   } DclConstBuf;
};

struct sm4_token_operand
{
   unsigned Comps      : 2;
   unsigned Mode       : 2;
   unsigned Sel        : 2;
   unsigned File       : 8;
   unsigned NumIndices : 2;
   unsigned Index0Repr : 3;
   unsigned Index1Repr : 3;
   unsigned Index2Repr : 3;
   unsigned Extended   : 1;
};

static void
ShaderTranslator::Opcode1To1()
{
   ureg_insn(ureg, current->tgsi, dst, numDst, src, numSrc);
}

struct sm4_op_info
{
   enum sm4_opcode opcode;
   unsigned tgsi;
   unsigned numDst;
   unsigned numSrc;
};

/*
        if (ureg_src_undefined(tx->regs.primid))
            tx->regs.sv_primid = ureg_DECL_system_value(ureg, tx->regs.num_sv++, TGSI_SEMANTIC_PRIMID, 0);
        return tx->regs.sv_primid;
*/
struct ureg_src
sm4_tx_src(struct sm4_token_operand op)
{
    switch (op.File) {
    case SM4_FILE_CONSTANT_BUFFER:
    case SM4_FILE_CYCLE_COUNTER:
    case SM4_FILE_FUNCTION_BODY:
    case SM4_FILE_FUNCTION_INPUT:
    case SM4_FILE_FUNCTION_OUTPUT:
    case SM4_FILE_FUNCTION_TABLE:
    case SM4_FILE_IMMEDIATE32:
    case SM4_FILE_IMMEDIATE64:
    case SM4_FILE_IMMEDIATE_CONSTANT_BUFFER:
    case SM4_FILE_INDEXABLE_TEMP:
    case SM4_FILE_INPUT:
    case SM4_FILE_INPUT_CONTROL_POINT:
    case SM4_FILE_INPUT_COVERAGE_MASK:
    case SM4_FILE_INPUT_DOMAIN_POINT:
    case SM4_FILE_INPUT_FORK_INSTANCE_ID:
    case SM4_FILE_INPUT_GS_INSTANCE_ID:
    case SM4_FILE_INPUT_JOIN_INSTANCE_ID:
    case SM4_FILE_INPUT_PATCH_CONSTANT:
    case SM4_FILE_INPUT_PRIMITIVEID:
    case SM4_FILE_INPUT_THREAD_GROUP_ID:
    case SM4_FILE_INPUT_THREAD_ID:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP_FLATTENED:
    case SM4_FILE_INTERFACE:
    case SM4_FILE_LABEL:
    case SM4_FILE_OUTPUT:
    case SM4_FILE_OUTPUT_CONTROL_POINT:
    case SM4_FILE_OUTPUT_CONTROL_POINT_ID:
    case SM4_FILE_OUTPUT_COVERAGE_MASK:
    case SM4_FILE_OUTPUT_DEPTH:
    case SM4_FILE_OUTPUT_DEPTH_GREATER_EQUAL:
    case SM4_FILE_OUTPUT_DEPTH_LESS_EQUAL:
    case SM4_FILE_RESOURCE:
    case SM4_FILE_SAMPLER:
    case SM4_FILE_STREAM:
    case SM4_FILE_TEMP:
    case SM4_FILE_THIS_POINTER:
    case SM4_FILE_THREAD_GROUP_SHARED_MEMORY:
    case SM4_FILE_UNORDERED_ACCESS_VIEW:
        break;
    default:
        break;
    }
}

struct ureg_dst
sm4_tx_dst(struct sm4_token_operand op)
{
    switch (op.File) {
    case SM4_FILE_CONSTANT_BUFFER:
    case SM4_FILE_CYCLE_COUNTER:
    case SM4_FILE_FUNCTION_BODY:
    case SM4_FILE_FUNCTION_INPUT:
    case SM4_FILE_FUNCTION_OUTPUT:
    case SM4_FILE_FUNCTION_TABLE:
    case SM4_FILE_IMMEDIATE32:
    case SM4_FILE_IMMEDIATE64:
    case SM4_FILE_IMMEDIATE_CONSTANT_BUFFER:
    case SM4_FILE_INDEXABLE_TEMP:
    case SM4_FILE_INPUT:
    case SM4_FILE_INPUT_CONTROL_POINT:
    case SM4_FILE_INPUT_COVERAGE_MASK:
    case SM4_FILE_INPUT_DOMAIN_POINT:
    case SM4_FILE_INPUT_FORK_INSTANCE_ID:
    case SM4_FILE_INPUT_GS_INSTANCE_ID:
    case SM4_FILE_INPUT_JOIN_INSTANCE_ID:
    case SM4_FILE_INPUT_PATCH_CONSTANT:
    case SM4_FILE_INPUT_PRIMITIVEID:
    case SM4_FILE_INPUT_THREAD_GROUP_ID:
    case SM4_FILE_INPUT_THREAD_ID:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP:
    case SM4_FILE_INPUT_THREAD_ID_IN_GROUP_FLATTENED:
    case SM4_FILE_INTERFACE:
    case SM4_FILE_LABEL:
    case SM4_FILE_OUTPUT:
    case SM4_FILE_OUTPUT_CONTROL_POINT:
    case SM4_FILE_OUTPUT_CONTROL_POINT_ID:
    case SM4_FILE_OUTPUT_COVERAGE_MASK:
    case SM4_FILE_OUTPUT_DEPTH:
    case SM4_FILE_OUTPUT_DEPTH_GREATER_EQUAL:
    case SM4_FILE_OUTPUT_DEPTH_LESS_EQUAL:
    case SM4_FILE_RESOURCE:
    case SM4_FILE_SAMPLER:
    case SM4_FILE_STREAM:
    case SM4_FILE_TEMP:
    case SM4_FILE_THIS_POINTER:
    case SM4_FILE_THREAD_GROUP_SHARED_MEMORY:
    case SM4_FILE_UNORDERED_ACCESS_VIEW:
        break;
    default:
        break;
    }
}

OPCODE_HANDLER(LABEL)
{
}

OPCODE_HANDLER(CUSTOMDATA)
{
}

OPCODE_HANDLER(DCL_GLOBAL_FLAGS)
{
    return S_OK;
}

OPCODE_HANDLER(DCL_RESOURCE)
{
    unsigned s = num_resources++;
    unsigned target = translate_target(insn.DclResource.Target);
    unsigned ret_x = translate_type(insn.DclResource.ReturnX);
    unsigned ret_y = translate_type(insn.DclResource.ReturnY);
    unsigned ret_z = translate_type(insn.DclResource.ReturnZ);
    unsigned ret_w = translate_type(insn.DclResource.ReturnW);

    ureg_DECL_sampler_view(ureg, s, target, ret_x, ret_y, ret_z, ret_w);

    return S_OK;
}

OPCODE_HANDLER(DCL_CONSTANT_BUFFER)
{
}

OPCODE_HANDLER(DCL_SAMPLER)
{
    ureg_DECL_sampler();
}

OPCODE_HANDLER(DCL_INPUT_PS)
{
    
}

unsigned sm4_to_tgsi_opcode[] =
{
   /* ADD */         TGSI_OPCODE_ADD, 1, 2
   /* AND */         TGSI_OPCODE_AND, 1, 2
   /* BREAK */       TGSI_OPCODE_BRK, 0, 0,
   /* BREAKC */      TGSI_OPCODE_BREAKC, 0, 1,
   /* CALL */        TGSI_OPCODE_CAL, 0, 0,
   /* CALLC */       TGSI_OPCODE_CAL, 0, 1,
   /* CASE */        TGSI_OPCODE_CASE, 0, 1,
   /* CONTINUE */    TGSI_OPCODE_CONT, 0, 0,
   /* CONTINUEC */   TGSI_OPCODE_CONT, 0, 1,
   /* CUT */         TGSI_OPCODE_ENDPRIM, 0, 0,
   /* DEFAULT */     TGSI_OPCODE_DEFAULT, 0, 0,
   /* DERIV_RTX */   TGSI_OPCODE_DDX, 1, 1,
   /* DERIV_RTY */   TGSI_OPCODE_DDY, 1, 1,
   /* DISCARD */     TGSI_OPCODE_KILP, 0, 0,
   /* DIV */         TGSI_OPCODE_DIV, 1, 2,
   /* DP2 */         TGSI_OPCODE_DP2, 1, 2,
   /* DP3 */         TGSI_OPCODE_DP3, 1, 2,
   /* DP4 */         TGSI_OPCODE_DP4, 1, 2,
   /* ELSE */        TGSI_OPCODE_ELSE, 0, 0,
   /* EMIT */        TGSI_OPCODE_EMIT, 0, 0,
   /* EMITTHENCUT */ TGSI_OPCODE_EMIT, 0, 0,
   /* ENDIF */       TGSI_OPCODE_ENDIF, 0, 0,
   /* ENDLOOP */     TGSI_OPCODE_ENDLOOP, 0, 0,
   /* ENDSWITCH */   TGSI_OPCODE_ENDSWITCH, 0, 0,
   /* EQ */          TGSI_OPCODE_FSEQ, 1, 2,
   /* EXP */         TGSI_OPCODE_EX2, 1, 1,
   /* FRC */         TGSI_OPCODE_FRC, 1, 1,
   /* FTOI */        TGSI_OPCODE_F2I, 1, 1,
   /* FTOU */        TGSI_OPCODE_F2U, 1, 1,
   /* GE */          TGSI_OPCODE_FSGE, 1, 2,
   /* IADD */        TGSI_OPCODE_UADD, 1, 2,
   /* IF */          TGSI_OPCODE_IF,
   /* IEQ */         TGSI_OPCODE_USEQ, 1, 2,
   /* IGE */         TGSI_OPCODE_ISGE, 1, 2,
   /* ILT */         TGSI_OPCODE_ISLT, 1, 2,
   /* IMAD */        TGSI_OPCODE_UMAD, 1, 3,
   /* IMAX */        TGSI_OPCODE_IMAX, 1, 2,
   /* IMIN */        TGSI_OPCODE_IMIN, 1, 2,
   /* IMUL */        TGSI_OPCODE_UMUL, 1, 2,
   /* INE */         TGSI_OPCODE_USNE, 1, 2,
   /* INEG */        TGSI_OPCODE_INEG, 1, 1,
   /* ISHL */        TGSI_OPCODE_SHL, 1, 2,
   /* ISHR */        TGSI_OPCODE_ISHR, 1, 2,
   /* ITOF */        TGSI_OPCODE_I2F, 1, 1,
   /* LABEL */       0,
   /* LD */          TGSI_OPCODE_SAMPLE_I,
   /* LD_MS */       TGSI_OPCODE_SAMPLE_I_MS,
   /* LOG */         TGSI_OPCODE_LG2, 1, 1,
   /* LOOP */        TGSI_OPCODE_BGNLOOP,
   /* LT */          TGSI_OPCODE_FSLT, 1, 2,
   /* MAD */         TGSI_OPCODE_MAD, 1, 3,
   /* MIN */         TGSI_OPCODE_MIN, 1, 2,
   /* MAX */         TGSI_OPCODE_MAX, 1, 2,
   /* CUSTOMDATA */  0,
   /* MOV */         TGSI_OPCODE_MOV, 1, 1,
   /* MOVC */        TGSI_OPCODE_UCMP, 1, 3,
   /* MUL */         TGSI_OPCODE_MUL, 1, 2,
   /* NE */          TGSI_OPCODE_FSNE, 1, 2,
   /* NOP */         TGSI_OPCODE_NOP,
   /* NOT */         TGSI_OPCODE_NOT, 1, 1,
   /* OR */          TGSI_OPCODE_OR, 1, 2,
   /* RESINFO */     TGSI_OPCODE_SVIEWINFO,
   /* RET */         TGSI_OPCODE_RET,
   /* RETC */        TGSI_OPCODE_RET,
   /* ROUND_NE */    TGSI_OPCODE_ROUND, 1, 1,
   /* ROUND_NI */    TGSI_OPCODE_FLOOR, 1, 1,
   /* ROUND_PI */    TGSI_OPCODE_CEIL, 1, 1,
   /* ROUND_Z */     TGSI_OPCODE_TRUNC, 1, 1,
   /* RSQ */         TGSI_OPCODE_RSQ, 1, 1,
   /* SAMPLE */      TGSI_OPCODE_SAMPLE,
   /* SAMPLE_C */    TGSI_OPCODE_SAMPLE_C,
   /* SAMPLE_C_LZ */ TGSI_OPCODE_SAMPLE_C_LZ,
   /* SAMPLE_L */    TGSI_OPCODE_SAMPLE_L,
   /* SAMPLE_D */    TGSI_OPCODE_SAMPLE_D,
   /* SAMPLE_B */    TGSI_OPCODE_SAMPLE_B,
   /* SQRT */        TGSI_OPCODE_SQRT, 1, 1,
   /* SWITCH */      TGSI_OPCODE_SWITCH,
   /* SINCOS */      TGSI_OPCODE_SCS, 2, 1,
   /* UDIV */        TGSI_OPCODE_UDIV, 1, 2,
   /* ULT */         TGSI_OPCODE_USLT, 1, 2,
   /* UGE */         TGSI_OPCODE_USGE, 1, 2,
   /* UMUL */        TGSI_OPCODE_UMUL, 1, 2,
   /* UMAD */        TGSI_OPCODE_UMAD, 1, 3,
   /* UMAX */        TGSI_OPCODE_UMAX, 1, 2,
   /* UMIN */        TGSI_OPCODE_UMIN, 1, 2,
   /* USHR */        TGSI_OPCODE_USHR, 1, 2,
   /* UTOF */        TGSI_OPCODE_U2F, 1, 1,
   /* XOR */         TGSI_OPCODE_XOR, 1, 2,

   /* DCL_RESOURCE */                     0,
   /* DCL_CONSTANT_BUFFER */              0,
   /* DCL_SAMPLER */                      0,
   /* DCL_INDEX_RANGE */                  0,
   /* DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY */ 0,
   /* DCL_GS_INPUT_PRIMITIVE */           0,
   /* DCL_MAX_OUTPUT_VERTEX_COUNT */      0,
   /* DCL_INPUT */                        0,
   /* DCL_INPUT_SGV */                    0,
   /* DCL_INPUT_SIV */                    0,
   /* DCL_INPUT_PS */                     0,
   /* DCL_INPUT_PS_SGV */                 0,
   /* DCL_INPUT_PS_SIV */                 0,
   /* DCL_OUTPUT */                       0,
   /* DCL_OUTPUT_SGV */                   0,
   /* DCL_OUTPUT_SIV */                   0,
   /* DCL_TEMPS */                        0,
   /* DCL_INDEXABLE_TEMP */               0,
   /* DCL_GLOBAL_FLAGS */                 0,

   /* D3D10_COUNT */ 0,

   /* LOD */         TGSI_OPCODE_LAST,
   /* GATHER4 */     TGSI_OPCODE_GATHER4,
   /* SAMPLE_POS */  TGSI_OPCODE_SAMPLE_POS,
   /* SAMPLE_INFO */ TGSI_OPCODE_SAMPLE_INFO,

   /* D3D10_1_COUNT */ 0,

   /* HS_DECLS */               0,
   /* HS_CONTROL_POINT_PHASE */ 0,
   /* HS_FORK_PHASE */          0,
   /* HS_JOIN_PHASE */          0,

   /* EMIT_STREAM */        TGSI_OPCODE_EMIT,
   /* CUT_STREAM */         TGSI_OPCODE_ENDPRIM,
   /* EMITTHENCUT_STREAM */ TGSI_OPCODE_EMIT,
   /* INTERFACE_CALL */     TGSI_OPCODE_CAL,
   /* BUFINFO */            TGSI_OPCODE_SVIEWINFO,
   /* DERIV_RTX_COARSE */   TGSI_OPCODE_DDX, 1, 1,
   /* DERIV_RTX_FINE */     TGSI_OPCODE_DDX, 1, 1,
   /* DERIV_RTY_COARSE */   TGSI_OPCODE_DDY, 1, 1,
   /* DERIV_RTY_FINE */     TGSI_OPCODE_DDY, 1, 1,
   /* GATHER4_C */          TGSI_OPCODE_GATHER4,
   /* GATHER4_PO */         TGSI_OPCODE_GATHER4,
   /* GATHER4_PO_C */       TGSI_OPCODE_GATHER4,
   /* RCP */                TGSI_OPCODE_RCP, 1, 1,
   /* F32TOF16 */           TGSI_OPCODE_LAST,
   /* F16TOF32 */           TGSI_OPCODE_LAST,
   /* UADDC */              TGSI_OPCODE_UADD, 1, 2,
   /* USUBB */              TGSI_OPCODE_UADD, 1, 2,
   /* COUNTBITS */          TGSI_OPCODE_POPC,
   /* FIRSTBIT_HI */        TGSI_OPCODE_BSF, 1, 1,
   /* FIRSTBIT_LO */        TGSI_OPCODE_BSR, 1, 1,
   /* FIRSTBIT_SHI */       TGSI_OPCODE_LAST,
   /* UBFE */               TGSI_OPCODE_UBFE,
   /* IBFE */               TGSI_OPCODE_IBFE,
   /* BFI */                TGSI_OPCODE_BFI,
   /* BFREV */              TGSI_OPCODE_BFREV,
   /* SWAPC */              TGSI_OPCODE_SWAPC,

   /* DCL_STREAM */                                0,
   /* DCL_FUNCTION_BODY */                         TGSI_OPCODE_BGNSUB,
   /* DCL_FUNCTION_TABLE */                        0,
   /* DCL_INTERFACE */                             0,
   /* DCL_INPUT_CONTROL_POINT_COUNT */             0,
   /* DCL_OUTPUT_CONTROL_POINT_COUNT */            0,
   /* DCL_TESS_DOMAIN */                           0,
   /* DCL_TESS_PARTITIONING */                     0,
   /* DCL_TESS_OUTPUT_PRIMITIVE */                 0,
   /* DCL_HS_MAX_TESSFACTOR */                     0,
   /* DCL_HS_FORK_PHASE_INSTANCE_COUNT */          0,
   /* DCL_HS_JOIN_PHASE_INSTANCE_COUNT */          0,
   /* DCL_THREAD_GROUP */                          0,
   /* DCL_UNORDERED_ACCESS_VIEW_TYPED */           0,
   /* DCL_UNORDERED_ACCESS_VIEW_RAW */             0,
   /* DCL_UNORDERED_ACCESS_VIEW_STRUCTURED */      0,
   /* DCL_THREAD_GROUP_SHARED_MEMORY_RAW */        0,
   /* DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED */ 0,
   /* DCL_RESOURCE_RAW */                          0,
   /* DCL_RESOURCE_STRUCTURED */                   0,

   /* LD_UAV_TYPED */     TGSI_OPCODE_LOAD,
   /* STORE_UAV_TYPED */  TGSI_OPCODE_STORE,
   /* LD_RAW */           TGSI_OPCODE_LOAD,
   /* STORE_RAW */        TGSI_OPCODE_STORE,
   /* LD_STRUCTURED */    TGSI_OPCODE_LOAD,
   /* STORE_STRUCTURED */ TGSI_OPCODE_STORE,
   /* ATOMIC_AND */       TGSI_OPCODE_ATOMAND,
   /* ATOMIC_OR */        TGSI_OPCODE_ATOMOR,
   /* ATOMIC_XOR */       TGSI_OPCODE_ATOMXOR,
   /* ATOMIC_CMP_STORE */ TGSI_OPCODE_ATOMCAS,
   /* ATOMIC_IADD */      TGSI_OPCODE_ATOMUADD,
   /* ATOMIC_IMAX */      TGSI_OPCODE_ATOMIMAX,
   /* ATOMIC_IMIN */      TGSI_OPCODE_ATOMIMIN,
   /* ATOMIC_UMAX */      TGSI_OPCODE_ATOMUMAX,
   /* ATOMIC_UMIN */      TGSI_OPCODE_ATOMUMIN,

   /* IMM_ATOMIC_ALLOC */    TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_CONSUME */  TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_IADD */     TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_AND */      TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_OR */       TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_XOR */      TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_EXCH */     TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_CMP_EXCH */ TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_IMAX */     TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_IMIN */     TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_UMAX */     TGSI_OPCODE_LAST,
   /* IMM_ATOMIC_UMIN */     TGSI_OPCODE_LAST,

   /* SYNC */  TGSI_OPCODE_BARRIER,
   /* DADD */  TGSI_OPCODE_DADD, 1, 2,
   /* DMAX */  TGSI_OPCODE_DMAX, 1, 2,
   /* DMIN */  TGSI_OPCODE_DMIN, 1, 2,
   /* DMUL */  TGSI_OPCODE_DMUL, 1, 2,
   /* DEQ */   TGSI_OPCODE_DSEQ, 1, 2,
   /* DGE */   TGSI_OPCODE_DSGE, 1, 2,
   /* DLT */   TGSI_OPCODE_DSLT, 1, 2,
   /* DNE */   TGSI_OPCODE_DSNE, 1, 2,
   /* DMOV */  TGSI_OPCODE_MOV, 1, 1,
   /* DMOVC */ TGSI_OPCODE_UCMP, 1, 3,
   /* DTOF */  TGSI_OPCODE_D2F, 1, 1,
   /* FTOD */  TGSI_OPCODE_F2D, 1, 1,

   /* EVAL_SNAPPED */      TGSI_OPCODE_LAST,
   /* EVAL_SAMPLE_INDEX */ TGSI_OPCODE_LAST,
   /* EVAL_CENTROID */     TGSI_OPCODE_LAST,

   /* DCL_GS_INSTANCE_COUNT */ 0
};

void process_sm4()
{
    struct sm4_token_instruction *insn;

    while (insn->get())
        insn->process();
}

