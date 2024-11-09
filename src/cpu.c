#include <stdio.h>

#include "cpu.h"
#include "ram.h"

/* ===========================================
*         INSTRUCTION IMPLEMENTATION
*  ===========================================
*/ 
static void _add_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] + (int32_t) cpu->gpr[instr->rs2]);
        #ifdef DEBUG
        printf("add\n");
        #endif
}

static void _sub_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] - (int32_t) cpu->gpr[instr->rs2]);
        #ifdef DEBUG
        printf("sub\n");
        #endif
}

static void _xor_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] ^ cpu->gpr[instr->rs2];
        #ifdef DEBUG
        printf("xor\n");
        #endif
}

static void _or_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] | cpu->gpr[instr->rs2];
        #ifdef DEBUG
        printf("or\n");
        #endif
}

static void _and_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] & cpu->gpr[instr->rs2];
        #ifdef DEBUG
        printf("and\n");
        #endif
}

static void _sll_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] << (cpu->gpr[instr->rs2] & 0x1f);
        #ifdef DEBUG
        printf("sl\n");
        #endif
}

static void _srl_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] >> (cpu->gpr[instr->rs2] & 0x1f);
        #ifdef DEBUG
        printf("srl\n");
        #endif
}

static void _sra_exec(cpu_s *cpu, instruction_s *instr)
{
        uint32_t rs_1 = cpu->gpr[instr->rs1];
        uint32_t rs_2 = cpu->gpr[instr->rs2];
        uint8_t rs_1_sign_bit = (rs_1 >> 31) & 1;
        
        uint32_t rd = (rs_1 >> (rs_2 & 0x1f)) | rs_1_sign_bit << 31; 

        cpu->gpr[instr->rd] = rd;
        #ifdef DEBUG
        printf("sra\n");
        #endif
}

static void _slt_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] =  ((int32_t) cpu->gpr[instr->rs1] < (int32_t) cpu->gpr[instr->rs2]) ? 1 : 0;
        #ifdef DEBUG
        printf("slt\n");
        #endif
}

static void _sltu_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] =  ((uint32_t) cpu->gpr[instr->rs1] < (uint32_t) cpu->gpr[instr->rs2]) ? 1 : 0;
        #ifdef DEBUG
        printf("sltu\n");
        #endif
}

static void _addi_exec(cpu_s *cpu, instruction_s *instr)
{
        uint16_t num = instr->imm;
        uint32_t result = (int32_t) cpu->gpr[instr->rs1];

        // minus if sign bit set else plus
        if (num >> 11 & 1) {
                result -= (num & 0x7ff);
        } else {
                result += (num & 0x7ff);
        }

        cpu->gpr[instr->rd] = result;
        #ifdef DEBUG
        printf("addi\n");
        #endif
}

static void _xori_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] ^ instr->imm;
        #ifdef DEBUG
        printf("xori\n");
        #endif
}

static void _ori_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] | instr->imm;
        #ifdef DEBUG
        printf("ori\n");
        #endif
}

static void _andi_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] & instr->imm;
        #ifdef DEBUG
        printf("ori\n");
        #endif
}

static void _slli_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] << (instr->imm & 0x1f);
        #ifdef DEBUG
        printf("slli\n");
        #endif
}

static void _srli_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] >> (instr->imm & 0x1f);
        #ifdef DEBUG
        printf("srli\n");
        #endif
}

static void _srai_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] >> (int32_t) (instr->imm & 0x1f));
        #ifdef DEBUG
        printf("srai\n");
        #endif
}

static void _slti_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (cpu->gpr[instr->rs1] < instr->imm) ? 1 : 0;
        #ifdef DEBUG
        printf("slti\n");
        #endif
}

static void _sltui_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = ((uint32_t) cpu->gpr[instr->rs1] < (uint32_t) instr->imm) ? 1 : 0;
        #ifdef DEBUG
        printf("sltui\n");
        #endif
}

static void _lb_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (int32_t) ram_load(cpu->ram, (cpu->gpr[instr->rs1] + instr->imm), BYTE);
        
        #ifdef DEBUG
        printf("lb\n");
        #endif
}

static void _lh_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (int32_t) ram_load(cpu->ram, (cpu->gpr[instr->rs1] + instr->imm), HALF_WORD);
        
        #ifdef DEBUG
        printf("lh\n");
        #endif
}

static void _lw_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (int32_t) ram_load(cpu->ram, (cpu->gpr[instr->rs1] + instr->imm), WORD);
        
        #ifdef DEBUG
        printf("lw\n");
        #endif
}

static void _lbu_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ram_load(cpu->ram, (cpu->gpr[instr->rs1] + instr->imm), BYTE);
        
        #ifdef DEBUG
        printf("lbu\n");
        #endif
}

static void _lhu_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ram_load(cpu->ram, (cpu->gpr[instr->rs1] + instr->imm), HALF_WORD);
        
        #ifdef DEBUG
        printf("lhu\n");
        #endif
}

static void _sb_exec(cpu_s *cpu, instruction_s *instr)
{
        ram_store(cpu->ram, (instr->rs1 + instr->imm), BYTE, instr->rs2);

        #ifdef DEBUG
        printf("sb\n");
        #endif
}

static void _sh_exec(cpu_s *cpu, instruction_s *instr)
{
        ram_store(cpu->ram, (instr->rs1 + instr->imm), HALF_WORD, instr->rs2);

        #ifdef DEBUG
        printf("sh\n");
        #endif
}

static void _sw_exec(cpu_s *cpu, instruction_s *instr)
{
        ram_store(cpu->ram, (instr->rs1 + instr->imm), WORD, instr->rs2);

        #ifdef DEBUG
        printf("sw\n");
        #endif
}

static void _beq_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((int32_t) instr->rs1 == (int32_t) instr->rs2) {
                cpu->pc += instr->imm;
        }

        #ifdef DEBUG
        printf("beq\n");
        #endif
}

static void _bne_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((int32_t) instr->rs1 != (int32_t) instr->rs2) {
                cpu->pc += (int32_t) instr->imm;
        }

        #ifdef DEBUG
        printf("beq\n");
        #endif
}

static void _blt_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((int32_t) instr->rs1 < (int32_t) instr->rs2) {
                cpu->pc += (int32_t) instr->imm;
        }

        #ifdef DEBUG
        printf("blt\n");
        #endif
}

static void _bge_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((int32_t) instr->rs1 >= (int32_t) instr->rs2) {
                cpu->pc += (int32_t) instr->imm;
        }

        #ifdef DEBUG
        printf("bge\n");
        #endif
}

static void _bltu_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((uint32_t) instr->rs1 < (uint32_t) instr->rs2) {
                cpu->pc += (int32_t) instr->imm;
        }

        #ifdef DEBUG
        printf("bltu\n");
        #endif
}

static void _bgeu_exec(cpu_s *cpu, instruction_s *instr)
{
        if ((uint32_t) instr->rs1 >= (uint32_t) instr->rs2) {
                cpu->pc += (int32_t) instr->imm;
        }

        #ifdef DEBUG
        printf("bgeu\n");
        #endif
}

static void _jal_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->pc + 4;
        cpu->pc += (int32_t) instr->imm;

        #ifdef DEBUG
        printf("jal\n");
        #endif
}

static void _jalr_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->pc + 4;
        cpu->pc = (int32_t) instr->rs1 + (int32_t) instr->imm;

        #ifdef DEBUG
        printf("jalr\n");
        #endif
}

static void _lui_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = instr->imm << 12;

        #ifdef DEBUG
        printf("lui\n");
        #endif
}

static void _auipc_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->pc + (instr->imm << 12);

        #ifdef DEBUG
        printf("auipc\n");
        #endif
}


/* ===========================================
*                 CPU STAGES
*  ===========================================
*/ 
static void _instruction_exec(cpu_s *cpu, instruction_s *instr)
{
        switch (instr->opcode) {
                case R_TYPE:
                        r_type_instruction_e r_inst_type = (instr->funct_7) << 4 | (instr->funct_3);
                        switch (r_inst_type) {
                                case ADD:
                                        _add_exec(cpu, instr);
                                        break;
                                case SUB:
                                        _sub_exec(cpu, instr);
                                        break;
                                case XOR:
                                        _xor_exec(cpu, instr);
                                        break;
                                case OR:
                                        _or_exec(cpu, instr);
                                        break;
                                case AND:
                                        _and_exec(cpu, instr);
                                        break;
                                case SLL:
                                        _sll_exec(cpu, instr);
                                        break;
                                case SRL:
                                        _srl_exec(cpu, instr);
                                        break;
                                case SRA:
                                        _sra_exec(cpu, instr);
                                        break;
                                case SLT:
                                        _slt_exec(cpu, instr);
                                        break;
                                case SLTU:
                                        _sltu_exec(cpu, instr);
                                        break;
                                default:
                                        break;
                        }
                        break;
                case I_TYPE:
                        i_type_instruction_e i_type_instr = (instr->funct_3);
                        switch (i_type_instr) {
                                case ADDI:
                                        _addi_exec(cpu, instr);
                                        break;
                                case XORI:
                                        _xori_exec(cpu, instr);
                                        break;
                                case ORI:
                                        _ori_exec(cpu, instr);
                                        break;
                                case ANDI:
                                        _andi_exec(cpu, instr);
                                        break;
                                case SLLI:
                                        _slli_exec(cpu, instr);
                                        break;
                                case SRI:
                                        uint32_t sri_type = ((instr->imm >> 5) & 0x7f) << 4 | i_type_instr;
                                        switch (sri_type)
                                        {
                                                case SRLI:
                                                        _srli_exec(cpu, instr);
                                                        break;
                                                case SRAI:
                                                        _srai_exec(cpu, instr);
                                                        break;
                                                 default:
                                                        break;
                                        }
                                case SLTI:
                                        _slti_exec(cpu, instr);
                                        break;
                                case SLTUI:
                                        _sltui_exec(cpu, instr);
                                        break;
                                
                                default:
                                        break;
                        }
                        break;
                case LOAD:
                        load_type_instruction_e load_type_instruction = instr->funct_3;
                        switch (load_type_instruction)
                        {
                        case LB:
                                _lb_exec(cpu, instr);
                                break;
                        case LH:
                                _lh_exec(cpu, instr);
                                break;
                        case LW:
                                _lw_exec(cpu, instr);
                                break;
                        case LBU:
                                _lbu_exec(cpu, instr);
                                break;
                        case LHU:
                                _lhu_exec(cpu, instr);
                                break;
                        default:
                                break;
                        }
                        break;
                case B_TYPE:
                        b_type_instruction_e b_type_instruction = instr->funct_3;
                        switch (b_type_instruction)
                        {
                        case BEQ:
                                _beq_exec(cpu, instr);
                                break;
                        case BNE:
                                _bne_exec(cpu, instr);
                                break;
                        case BLT:
                                _blt_exec(cpu, instr);
                                break;
                        case BGE:
                                _bge_exec(cpu, instr);
                                break;
                        case BLTU:
                                _bltu_exec(cpu, instr);
                                break;
                        case BGEU:
                                _bgeu_exec(cpu, instr);
                                break;
                        
                        default:
                                break;
                        }
                        break;
                case S_TYPE:
                        s_type_instruction_e s_type_instruction = instr->funct_3;
                        switch (s_type_instruction)
                        {
                        case SB:
                                _sb_exec(cpu, instr);
                                break;
                        case SH:
                                _sh_exec(cpu, instr);
                                break;
                        case SW:
                                _sw_exec(cpu, instr);
                                break;
                        default:
                                break;
                        }
                        break;
                case JAL:
                        _jal_exec(cpu, instr);
                        break;
                case JALR:
                        _jalr_exec(cpu, instr);
                        break;
                case LUI:
                        _lui_exec(cpu, instr);
                        break;
                case AUIPC:
                        _auipc_exec(cpu, instr);
                        break;
                default:
                        break;
        }
}

static instruction_s _instruction_decode(uint32_t instr)
{
        instruction_s decoded_instr = {.funct_3=0, .funct_7=0, .imm=0, .opcode=0, .rd=0, .rs1=0, .rs2=0};
        decoded_instr.opcode = (instr >> 0) & 0x3f;

        switch (decoded_instr.opcode)
        {
        case R_TYPE:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = (instr >> 20) & 0x1f;
                decoded_instr.funct_7 = (instr >> 25) & 0x7f;
                decoded_instr.imm = 0; 
                break;
        case I_TYPE:
        case LOAD:
        case ECALL:
        case JALR:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = 0;
                decoded_instr.funct_7 = 0;
                decoded_instr.imm = (instr >> 20) & 0xfff; 
                break;
        case S_TYPE:
                decoded_instr.rd = 0;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = (instr >> 20) & 0x1f;
                decoded_instr.funct_7 = 0;
                decoded_instr.imm = (((instr >> 25) & 0x7f) << 5) | ((instr >> 7) & 0x1f); 
                break;
        case B_TYPE:
                decoded_instr.rd = 0;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = (instr >> 20) & 0x1f;
                decoded_instr.funct_7 = 0;
                decoded_instr.imm = (((instr >> 30) & 0x1) << 12) | (((instr >> 7) & 0x1) << 11) | (((instr >> 25) & 0x3f) << 5) | (((instr >> 8) & 0xf) << 1);
                break;
        case JAL:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.imm = (((instr >> 20) & 0x1) << 20) | (((instr >> 12) & 0xff) << 12) | (((instr >> 9) & 0x1) << 11) | (((instr >> 10) & 0x3f) << 1);
                break;
        case LUI:
        case AUIPC:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.imm = (instr >> 12) & 0xfffff;
                break;
        default:
                break;
        }

        return decoded_instr;
}

/* ===========================================
*             CPU MODEL FUNCTIONS
*  ===========================================
*/ 

void cpu_init(cpu_s *cpu, ram_s *ram)
{
        cpu->ram = ram;
        cpu->gpr[ZERO]  = 0x0;
        cpu->gpr[SP]    = RAM_DATA_ADDR;
        cpu->pc         = RAM_INSTRUCTION_ADDR;
}

void cpu_start(cpu_s *cpu)
{
        while (1) {
                cpu->gpr[ZERO] = 0x0;
                uint32_t instr = instruction_load(cpu->ram, cpu->pc);
                if (0 == instr) {
#ifdef DEBUG
                        printf("EOF\n");
                        printf("PC: %x\n", cpu->pc);
#endif
                        break;
                }
                cpu->pc += 4;

                instruction_s decoded_instr = _instruction_decode(instr);
                _instruction_exec(cpu, &decoded_instr);
        }
}