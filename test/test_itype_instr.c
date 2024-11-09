#include <time.h>
#include <stdlib.h>

#include "unity/unity.h"
#include "cpu.h"
#include "ram.h"
#include "regmap.h"

#define MAX_NUM   (2147483648)

ram_s ram;
cpu_s cpu;

void setUp(void)
{
        for (size_t reg = 0; reg < REGS_NUM; reg++) {
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
        uint16_t rand_num = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};
        uint8_t neg = 0;

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();

                // generate random 12 bit signed number
                rand_num = ((uint16_t) rand()) & 0x7ff;

                // if negative set sign bit (12)
                uint16_t num_to_load = rand_num;
                if (neg) {
                        num_to_load |= 1 << 11;
                }

                instr = ((num_to_load & 0xfff) << 20) | (S1 << 15) | (ADDI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1];
                if (neg) {
                        res -= rand_num;
                } else {
                        res += rand_num;
                }

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);
                
                neg ^= 1;
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 ^ imm (signed)
*/
void test_xori_instr()
{
        uint16_t rand_num = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};
        uint8_t neg = 0;

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();

                // generate random 12 bit signed number
                rand_num = ((uint16_t) rand()) & 0x7ff;

                // if negative set sign bit (12)
                uint16_t num_to_load = rand_num;
                if (neg) {
                        num_to_load |= 1 << 11;
                }

                instr = ((num_to_load & 0xfff) << 20) | (S1 << 15) | (XORI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] ^ num_to_load;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);
                
                neg ^= 1;
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 | imm (signed)
*/
void test_ori_instr()
{
        uint16_t rand_num = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};
        uint8_t neg = 0;

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();

                // generate random 12 bit signed number
                rand_num = ((uint16_t) rand()) & 0x7ff;

                // if negative set sign bit (12)
                uint16_t num_to_load = rand_num;
                if (neg) {
                        num_to_load |= 1 << 11;
                }

                instr = ((num_to_load & 0xfff) << 20) | (S1 << 15) | (ORI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] | num_to_load;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);
                
                neg ^= 1;
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 & imm (signed)
*/
void test_andi_instr()
{
        uint16_t rand_num = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};
        uint8_t neg = 0;

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();

                // generate random 12 bit signed number
                rand_num = ((uint16_t) rand()) & 0x7ff;

                // if negative set sign bit (12)
                uint16_t num_to_load = rand_num;
                if (neg) {
                        num_to_load |= 1 << 11;
                }

                instr = ((num_to_load & 0xfff) << 20) | (S1 << 15) | (ANDI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] & num_to_load;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);
                
                neg ^= 1;
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 << imm 
*/
void test_slli_instr()
{
        uint16_t shamt = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();
                shamt = ((uint16_t) rand()) & 0x1f;

                instr = (shamt << 20) | (S1 << 15) | (SLLI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] << shamt;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);
                
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 >> imm logical
*/
void test_srli_instr()
{
        uint16_t shamt = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                cpu.gpr[S1] = (uint32_t) rand();
                shamt = ((uint16_t) rand()) & 0x1f;

                instr = ((SRLI >> 4) << 27) | (shamt << 20) | (S1 << 15) | (SRI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint32_t res = cpu.gpr[S1] >> shamt;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = rs1 >> imm arithmetic
*/
void test_srai_instr()
{
        uint16_t shamt = 0;
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};
        uint8_t neg = 0;

        for (size_t i = 0; i < 100; i++) {
                uint32_t num_to_shift = (uint32_t) rand();
                if (neg) {
                        num_to_shift |= 1 << 31;
                }

                cpu.gpr[S1] = num_to_shift;
                shamt = ((uint16_t) rand()) & 0x1f;

                instr = ((SRAI >> 4) << 27) | (shamt << 20) | (S1 << 15) | (SRI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                uint8_t sign_bit = (cpu.gpr[S1] >> 31) & 1;
                uint32_t res = cpu.gpr[S1] >> shamt;

                if (sign_bit) {
                        res |= 1 << 31;
                }

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                neg ^= 1;
                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = 1 if rs < imm else 0 signed
*/
void test_slti_instr()
{
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                int32_t compare_num =  (MAX_NUM / 2) - rand();
                int32_t rand_num = ((MAX_NUM / 2) & 0x7ff) - (rand() & 0x7ff);

                uint32_t res = compare_num < rand_num ? 1 : 0;

                uint32_t compare_num_unsigned = compare_num;
                if (compare_num < 0) {
                        compare_num_unsigned = 0xffffffff + compare_num;
                        compare_num_unsigned |= 1 << 31;
                }

                if (rand_num < 0) {
                        rand_num = 0xfff + rand_num;
                        rand_num |= 1 << 11;
                }
                cpu.gpr[S1] = compare_num_unsigned;

                instr = (rand_num << 20) | (S1 << 15) | (SLTI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}

/*
 * rd = 1 if rs < imm else 0 unsigned
*/
void test_sltui_instr()
{
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        for (size_t i = 0; i < 100; i++) {
                uint32_t compare_num =  rand();
                uint16_t rand_num = rand() & 0xfff;

                uint32_t res = compare_num < rand_num ? 1 : 0;

                cpu.gpr[S1] = compare_num;

                instr = (rand_num << 20) | (S1 << 15) | (SLTUI << 12) | (S0 << 7) | I_TYPE;
                instructions[0] = instr;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                TEST_ASSERT_EQUAL(res, cpu.gpr[S0]);
        }
}


int main()
{
        srand(time(NULL));
        UNITY_BEGIN();
        RUN_TEST(test_addi_instr);
        RUN_TEST(test_xori_instr);
        RUN_TEST(test_ori_instr);
        RUN_TEST(test_slli_instr);
        RUN_TEST(test_srli_instr);
        RUN_TEST(test_srai_instr);
        RUN_TEST(test_slti_instr);
        RUN_TEST(test_sltui_instr);
        return UNITY_END();
}