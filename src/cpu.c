#include <stdio.h>

#include "cpu.h"
#include "ram.h"

/* ===========================================
*         INSTRUCTION IMPLEMENTATION
*  ===========================================
*/ 
static void _addi_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] + (int32_t) instr->imm);
        printf("addi\n");
}

static void _add_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] + (int32_t) cpu->gpr[instr->rs2]);
        printf("add\n");
}

static void _sub_exec(cpu_s *cpu, instruction_s *instr)
{
        cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] - (int32_t) cpu->gpr[instr->rs2]);
        printf("sub\n");
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
                                default:
                                        break;
                                }
                                break;
                case I_TYPE:
                        i_type_instruction_e i_type_instr = instr->funct_3;
                        switch (i_type_instr) {
                                case ADDI:
                                        _addi_exec(cpu, instr);
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