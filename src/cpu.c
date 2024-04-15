#include "cpu.h"
#include "ram.h"

instruction_s instruction_decode(uint32_t inst)
{
        instruction_s decoded_inst;
        decoded_inst.opcode = (inst & 0x3f) >> 0;

        switch (decoded_inst.opcode)
        {
        case R_TYPE:
                decoded_inst.rd = (inst & 0x1f) >> 7;
                decoded_inst.fucnt_3 = (inst & 0x7) >> 12;
                decoded_inst.rs1 = (inst & 0x1f) >> 15;
                decoded_inst.rs2 = (inst & 0x1f) >> 20;
                decoded_inst.funct_7 = (inst & 0x7f) >> 25;
                decoded_inst.imm = 0; 
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