#include "cpu.h"
#include "ram.h"

instruction_s instruction_decode(uint32_t inst)
{
        instruction_s decoded_inst;
        decoded_inst.opcode = (inst >> 0) & 0x3f;

        switch (decoded_inst.opcode)
        {
        case R_TYPE:
                decoded_inst.rd = (inst >> 7) & 0x1f;
                decoded_inst.fucnt_3 = (inst  >> 12) & 0x7;
                decoded_inst.rs1 = (inst >> 15) & 0x1f;
                decoded_inst.rs2 = (inst >> 20) & 0x1f;
                decoded_inst.funct_7 = (inst >> 25) & 0x7f;
                decoded_inst.imm = 0; 
                break;
        case I_TYPE:
        case LOAD:
        case ECALL:
        case JALR:
                decoded_inst.rd = (inst >> 7) & 0x1f;
                decoded_inst.fucnt_3 = (inst >> 12) & 0x7;
                decoded_inst.rs1 = (inst >> 15) & 0x1f;
                decoded_inst.rs2 = 0;
                decoded_inst.funct_7 = 0;
                decoded_inst.imm = (inst >> 20) & 0xfff; 
                break;
        case S_TYPE:
                decoded_inst.rd = 0;
                decoded_inst.fucnt_3 = (inst >> 12) & 0x7;
                decoded_inst.rs1 = (inst >> 15) & 0x1f;
                decoded_inst.rs2 = (inst >> 20) & 0x1f;
                decoded_inst.funct_7 = 0;
                decoded_inst.imm = (((inst >> 25) & 0x7f) << 5) | ((inst >> 7) & 0x1f); 
                break;
        case B_TYPE:
                decoded_inst.rd = 0;
                decoded_inst.fucnt_3 = (inst >> 12) & 0x7;
                decoded_inst.rs1 = (inst >> 15) & 0x1f;
                decoded_inst.rs2 = (inst >> 20) & 0x1f;
                decoded_inst.funct_7 = 0;
                decoded_inst.imm = (((inst >> 30) & 0x1) << 12) | (((inst >> 7) & 0x1) << 11) | (((inst >> 25) & 0x3f) << 5) | (((inst >> 8) & 0xf) << 1);
                break;

        default:
                break;
        }
}

uint32_t instruction_code(instruction_s *inst, instruction_type_e inst_type)
{
        uint32_t instruction = 0;
        instruction |= (inst_type & 0x3f) << 0;
        
        switch (inst_type)
        {
        case R_TYPE:
                instruction |= (inst->rd) << 7;
                instruction |= (inst->fucnt_3) << 12;
                instruction |= (inst->rs1) << 15;
                instruction |= (inst->rs2) << 20;
                instruction |= (inst->funct_7) << 25;
                break;
        
        default:
                break;
        }
}

void cpu_init(cpu_s *cpu)
{
        cpu->gpr[ZERO]  = 0x0;
        cpu->gpr[SP]    = RAM_DATA_ADDR;
        cpu->pc         = RAM_INSTRUCTION_ADDR;
}

