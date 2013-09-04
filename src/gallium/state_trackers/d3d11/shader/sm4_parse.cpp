
struct sm4_token_version
{
    uint32_t Minor  : 4;
    uint32_t Major  : 4;
    uint32_t Format : 8;
    uint32_t Type   : 16;
};

union sm4_token_instruction
{
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

struct sm4_token_return_type
{
    uint32_t X : 4;
    uint32_t Y : 4;
    uint32_t Z : 4;
    uint32_t W : 4;
};

struct sm4_token_operand
{
    uint32_t Comps      : 2;
    uint32_t Mode       : 2;
    uint32_t Select     : 8;
    uint32_t File       : 8;
    uint32_t NumIndices : 2;
    uint32_t Index0Repr : 3;
    uint32_t Index1Repr : 3;
    uint32_t Index2Repr : 3;
    uint32_T Extended   : 1;
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

struct sm4_parser
{
    struct sm4_program *prog;

    uint32_t *tok;
    uint32_t *tok_end;
};

struct sm4_operand
{
};

struct sm4_instruction
{
};

struct sm4_declaration
{
};

struct sm4_program
{
    struct sm4_token_version version;

    struct sm4_declaration *decl;
    struct sm4_instruction *insn;
    unsigned num_decl;
    unsigned num_insn;
    
    struct {
        D3D11_SIGNATURE_PARAMETER_DESC *in;
        D3D11_SIGNATURE_PARAMETER_DESC *out;
        D3D11_SIGNATURE_PARAMETER_DESC *pc;
        uint8_t num_in;
        uint8_t num_out;
        uint8_t num_pc;
    } sig;

    struct sm4_shader_blob *info;
};

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

struct sm4_program *
sm4_parse(struct d3d11_shader_blob *info)
{
    struct sm4_program *prog = CALLOC_STRUCT(sm4_program);

    prog->sig.in = data->in;
    prog->sig.out = data->out;
    prog->sig.pc = data->pc;
}
