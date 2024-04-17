#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <stdint.h>

typedef enum instruction_type_e {
        R_TYPE = 0b0110011,
        I_TYPE = 0b0010011,
        LOAD = 0b0000011,
        S_TYPE = 0b0100011,
        B_TYPE = 0b1100011,
        JAL = 0b1101111,
        JALR = 0b1100111,
        LUI = 0b0110111,
        AUIPC = 0b0010111,
        ECALL = 0b1110011,
        EBREAK = 0b1110011
} instruction_type_e;

typedef enum r_type_instruction_e {
        ADD = 0x0,
        SUB = 0x200


} r_type_instruction_e;

typedef struct instruction_s {
        uint8_t opcode;
        uint8_t rd;
        uint8_t fucnt_3;
        uint8_t rs1;
        uint8_t rs2;
        uint8_t funct_7;
        uint8_t imm;
} instruction_s;

#endif 