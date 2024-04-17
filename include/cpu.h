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

void cpu_init(cpu_s *cpu, ram_s *ram);
void cpu_start(cpu_s *cpu);

#endif 