#include <time.h>
#include <stdlib.h>

#include "unity/unity.h"
#include "cpu.h"
#include "ram.h"
#include "regmap.h"

ram_s ram;
cpu_s cpu;

void setUp(void)
{
        for (size_t reg; reg < REGS_NUM; reg++) {
                cpu.gpr[reg] = 0;
        }
        cpu_init(&cpu, &ram);
}

void tearDown(void) 
{
    // clean stuff up here
}

/*
 * decoded_instr.opcode = (instr >> 0) & 0x3f;
 * i_type_instruction_e i_type_instr = ((instr->imm >> 5) & 0x7f) << 4 | (instr->funct_3);
 *        case I_TYPE:
 *              decoded_instr.rd = (instr >> 7) & 0x1f;
 *              decoded_instr.funct_3 = (instr >> 12) & 0x7;
 *              decoded_instr.rs1 = (instr >> 15) & 0x1f;
 *              decoded_instr.rs2 = 0;
 *              decoded_instr.funct_7 = 0;
 *              decoded_instr.imm = (instr >> 20) & 0xfff; 
 *
*/

/*
 * rd = rs1 + imm
*/
void test_addi_instr()
{
        uint32_t rand_num = ((uint32_t) rand()) & 0xfff;

        uint32_t instr = (rand_num << 20) | (S1 << 15) | (ADDI << 12) | (S0 << 7) | I_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();
                rand_num = ((uint32_t) rand()) & 0xfff;
                instr = (rand_num << 20) | (S1 << 15) | (ADDI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] + rand_num;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}


int main()
{
        UNITY_BEGIN();
        RUN_TEST(test_addi_instr);
        return UNITY_END();
}