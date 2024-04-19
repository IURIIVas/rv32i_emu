#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

#include "cpu.h"

static uint32_t _instruction_code(instruction_s *instr, instruction_type_e inst_type)
{
        uint32_t instruction = 0;
        instruction |= (inst_type & 0x3f) << 0;
        
        switch (inst_type)
        {
        case R_TYPE:
                instruction |= (instr->rd & 0x1f) << 7;
                instruction |= (instr->funct_3 & 0x7) << 12;
                instruction |= (instr->rs1 & 0x1f) << 15;
                instruction |= (instr->rs2 & 0x1f) << 20;
                instruction |= (instr->funct_7 & 0x7f) << 25;
                break;
        case I_TYPE:
        case LOAD:
        case ECALL:
        case JALR:
                instruction |= (instr->rd & 0x1f) << 7;
                instruction |= (instr->funct_3 & 0x7) << 12;
                instruction |= (instr->rs1 & 0x1f) << 15;
                instruction |= (instr->imm & 0xfff) << 20;
                break;
        case S_TYPE:
                instruction |= (instr->funct_3 & 0x7) << 12;
                instruction |= (instr->rs1 & 0x1f) << 15;
                instruction |= (instr->rs2 & 0x1f) << 20;
                uint32_t imm0 = (instr->imm & 0x1f) << 7;
                uint32_t imm1 = ((instr->imm >> 5) & 0x7f) << 25;
                instruction |= (imm0 | imm1);
                break;
        case B_TYPE:
                instruction |= (instr->funct_3 & 0x7) << 12;
                instruction |= (instr->rs1 & 0x1f) << 15;
                instruction |= (instr->rs2 & 0x1f) << 20;
                uint32_t imm0 = ((instr->imm >> 1) & 0xf) << 8;
                uint32_t imm1 = ((instr->imm >> 5) & 0x3f) << 25;
                uint32_t imm2 = ((instr->imm >> 11) & 0x1) << 7;
                uint32_t imm3 = ((instr->imm >> 12) & 0x1) << 30;
                instruction |= (imm0 | imm1 | imm2 | imm3);
                break;
        case JAL:
                instruction |= (instr->rd & 0x1f) << 7;
                instruction |= (instr->imm & 0xfffff) << 12;
                break;
        case LUI:
        case AUIPC:
                instruction |= (instr->rd & 0x1f) << 7;
                instruction |= (instr->imm & 0xfffff) << 12;
                break;
        default:
                break;
        }

        return instruction;
}

static uint32_t _instruction_parse(char *instr) 
{
        char *inst_mnemonic = (char *) malloc(15 * sizeof(char));
        size_t inst_mnem_idx = 0;
        uint8_t inst_mnemonic_readed = 0;

        while (*instr) {
                while ((*instr) == ' ' || (*instr) == '\t') {
                        instr++;
                }
                while (((*instr) != ' ' || (*instr) != '\t') && !inst_mnemonic_readed) {
                        inst_mnemonic[inst_mnem_idx++] = (*instr++);
                }
                inst_mnemonic_readed = 1;
                instr++;
        }

        return 1;
}

void file_parse(int fd, uint32_t *instruction_arr)
{
        instruction_s first_instr = {.opcode=I_TYPE, .funct_3=0, .rd=T0, .rs1=ZERO, .imm=3};
        instruction_s second_instr = {.opcode=I_TYPE, .funct_3=0, .rd=T1, .rs1=ZERO, .imm=2};
        instruction_s third_instr = {.opcode=R_TYPE, .funct_3=0, .funct_7=0, .rd=T0, .rs1=T0, .rs2=T1};
        
        instruction_arr[0] = _instruction_code(&first_instr, I_TYPE);
        instruction_arr[1] = _instruction_code(&second_instr, I_TYPE);
        instruction_arr[2] = _instruction_code(&third_instr, R_TYPE);
}