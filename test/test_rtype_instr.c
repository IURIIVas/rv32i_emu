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
decoded_instr.opcode = (instr >> 0) & 0x3f;
r_inst_type = (instr->funct_7) << 4 | (instr->funct_3);
        case R_TYPE:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = (instr >> 20) & 0x1f;
                decoded_instr.funct_7 = (instr >> 25) & 0x7f;
                decoded_instr.imm = 0; 
                break;

*/

/*
*      uint32_t rd = rs1 + rs2
*/
void test_add_instr()
{      
        uint32_t instr = (S0 << 20) | (S1 << 15) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] + cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = rs1 - rs2
*/
void test_sub_instr()
{      
        uint32_t instr = ((SUB >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SUB & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] - cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 ^ uint32_t rs2
*/
void test_xor_instr()
{      
        uint32_t instr = ((XOR >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((XOR & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] ^ cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 | uint32_t rs2
*/
void test_or_instr()
{      
        uint32_t instr = ((OR >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((OR & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] | cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 & uint32_t rs2
*/
void test_and_instr()
{      
        uint32_t instr = ((AND >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((AND & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] & cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 << uint32_t rs2
*/
void test_sll_instr()
{      
        uint32_t instr = ((SLL >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SLL & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (uint32_t) rand();
                cpu.gpr[S1] = (uint32_t) rand();
                uint32_t res = cpu.gpr[S1] << cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 >> uint32_t rs2
*/
void test_srl_instr()
{      
        uint32_t instr = ((SRL >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SRL & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (uint32_t) rand();
                cpu.gpr[S1] = (uint32_t) rand();
                uint32_t res = cpu.gpr[S1] >> cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = int32_t rs1 >> int32_t rs2
*/
void test_sra_instr()
{      
        uint32_t instr = ((SRA >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SRA & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] >> cpu.gpr[S0];

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = int32_t rs1 < int32_t rs2
*/
void test_slt_instr()
{      
        uint32_t instr = ((SLT >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SLT & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (int32_t) rand();
                cpu.gpr[S1] = (int32_t) rand();
                uint32_t res = cpu.gpr[S1] < cpu.gpr[S0] ? 1 : 0;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
*      uint32_t rd = uint32_t rs1 < uint32_t rs2
*/
void test_sltu_instr()
{      
        uint32_t instr = ((SLTU >> 4) << 25) | (S0 << 20) | (S1 << 15) | ((SLTU & 0x7) << 12) | (S0 << 7) | R_TYPE;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S0] = (uint32_t) rand();
                cpu.gpr[S1] = (uint32_t) rand();
                uint32_t res = cpu.gpr[S1] < cpu.gpr[S0] ? 1 : 0;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

int main(void) 
{
        srand(time(NULL));
        UNITY_BEGIN();
        RUN_TEST(test_add_instr);
        RUN_TEST(test_sub_instr);
        RUN_TEST(test_xor_instr);
        RUN_TEST(test_or_instr);
        RUN_TEST(test_and_instr);
        RUN_TEST(test_sll_instr);
        RUN_TEST(test_srl_instr);
        RUN_TEST(test_sra_instr);
        RUN_TEST(test_slt_instr);
        RUN_TEST(test_sltu_instr);
        return UNITY_END();
}