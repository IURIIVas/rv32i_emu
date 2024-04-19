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
        SUB = 0x200,
        XOR = 0x4,
        OR = 0x6,
        AND = 0x7,
        SLL = 0x1,
        SRL = 0x5,
        SRA = 0x205,
        SLT = 0x2,
        SLTU = 0x3
} r_type_instruction_e;

typedef enum i_type_instruction_e {
        ADDI = 0x0,
        XORI = 0x4,
        ORI = 0x6,
        ANDI = 0x7,
        SLLI = 0x1,
        SRLI = 0x5,
        SRAI = 0x205,
        SLTI = 0x2,
        SLTUI = 0x3
} i_type_instruction_e;

typedef enum load_type_instruction_e {
        LB = 0x0,
        LH = 0x1,
        LW = 0x2,
        LBU = 0x4,
        LHU = 0x5
} load_type_instruction_e;

typedef enum s_type_instruction_e {
        SB = 0x0,
        SH = 0x1,
        SW = 0x2
} s_type_instruction_e;

typedef enum b_type_instruction_e {
        BEQ = 0x0,
        BNE = 0x1,
        BLT = 0x4,
        BGE = 0x5,
        BLTU = 0x6,
        BGEU = 0x7
} b_type_instruction_e;

typedef struct instruction_s {
        uint8_t opcode;
        uint8_t rd;
        uint8_t funct_3;
        uint8_t rs1;
        uint8_t rs2;
        uint8_t funct_7;
        uint8_t imm;
} instruction_s;

#endif 