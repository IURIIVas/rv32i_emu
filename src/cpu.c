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
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] << cpu->gpr[instr->rs2];
        #ifdef DEBUG
        printf("sl\n");
        #endif
}

static void _srl_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = cpu->gpr[instr->rs1] >> cpu->gpr[instr->rs2];
        #ifdef DEBUG
        printf("srl\n");
        #endif
}

static void _sra_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] >> (int32_t) cpu->gpr[instr->rs2]);
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
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] + (int32_t) instr->imm);
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
                        i_type_instruction_e i_type_instr = ((instr->imm >> 5) & 0x7f) << 4 | (instr->funct_3);
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
                                case SRLI:
                                        _srli_exec(cpu, instr);
                                        break;
                                case SRAI:
                                        _srai_exec(cpu, instr);
                                        break;
                                case SLTI:
                                        _slti_exec(cpu, instr);
                                        break;
                                case SLTUI:
                                        _sltui_exec(cpu, instr);
                                        break;
                                
                                default:
                                        break;
                        }
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
                        printf("EOF\n");
                        break;
                }
                cpu->pc += 4;

                instruction_s decoded_instr = _instruction_decode(instr);
                _instruction_exec(cpu, &decoded_instr);
        }
}