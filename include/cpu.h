#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

#include "regmap.h"
#include "instruction.h"
#include "ram.h"

typedef struct cpu_s {
        uint32_t gpr[REGS_NUM];
        uint32_t pc;
        ram_s *ram;
} cpu_s;

instruction_s instruction_decode(uint32_t inst);
uint32_t instruction_code(instruction_s *inst, instruction_type_e inst_type);

#endif 