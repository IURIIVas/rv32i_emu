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
 * load_type_instruction_e load_type_instruction = instr->funct_3;
 * 
 * LOAD:
 * decoded_instr.rd = (instr >> 7) & 0x1f;
 * decoded_instr.funct_3 = (instr >> 12) & 0x7;
 * decoded_instr.rs1 = (instr >> 15) & 0x1f;
 * decoded_instr.rs2 = 0;
 * decoded_instr.funct_7 = 0;
 * decoded_instr.imm = (instr >> 20) & 0xfff;
 * 
 * STORE
 * decoded_instr.rd = 0;
 * decoded_instr.funct_3 = (instr >> 12) & 0x7;
 * decoded_instr.rs1 = (instr >> 15) & 0x1f;
 * decoded_instr.rs2 = (instr >> 20) & 0x1f;
 * decoded_instr.funct_7 = 0;
 * decoded_instr.imm = (((instr >> 25) & 0x7f) << 5) | ((instr >> 7) & 0x1f); 
 *
*/

/*
*      M[x[rs1] + sext(offset)] = x[rs2][7:0]
*/
void test_sb_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:  MEM;  EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t num_s0 = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = ((offset >> 5) << 25) | (S0 << 20) | (S1 << 15) | (SB << 12) | ((offset & 0x1f) << 7) | S_TYPE;
                instructions[0] = instr;

                cpu.gpr[S0] = num_s0;
                cpu.gpr[S1] = base;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t ram_val = cpu.ram->mem[base + offset - RAM_START_ADDR];

                printf("0x%x: 0x%02x; 0x%02x\n", base + offset, ram_val, num_s0 & 0xff);
                TEST_ASSERT_EQUAL(num_s0 & 0xff, ram_val);
        }
}

/*
*      M[x[rs1] + sext(offset)] = x[rs2][15:0]
*/
void test_sh_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:    MEM;    EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t num_s0 = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = ((offset >> 5) << 25) | (S0 << 20) | (S1 << 15) | (SH << 12) | ((offset & 0x1f) << 7) | S_TYPE;
                instructions[0] = instr;

                cpu.gpr[S0] = num_s0;
                cpu.gpr[S1] = base;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t byte0_addr = base + offset - RAM_START_ADDR;
                uint32_t ram_val = (cpu.ram->mem[byte0_addr + 1] << 8) | cpu.ram->mem[byte0_addr];

                printf("0x%x: 0x%04x; 0x%04x\n", base + offset, ram_val, num_s0 & 0xffff);
                TEST_ASSERT_EQUAL(num_s0 & 0xffff, ram_val);
        }
}

/*
*      M[x[rs1] + sext(offset)] = x[rs2][31:0]
*/
void test_sw_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t num_s0 = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = ((offset >> 5) << 25) | (S0 << 20) | (S1 << 15) | (SW << 12) | ((offset & 0x1f) << 7) | S_TYPE;
                instructions[0] = instr;

                cpu.gpr[S0] = num_s0;
                cpu.gpr[S1] = base;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t byte0_addr = base + offset - RAM_START_ADDR;
                uint32_t ram_val = (cpu.ram->mem[byte0_addr + 3] << 24) | (cpu.ram->mem[byte0_addr + 2] << 16) |
                                   (cpu.ram->mem[byte0_addr + 1] << 8)  | cpu.ram->mem[byte0_addr];

                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, ram_val, num_s0);
                TEST_ASSERT_EQUAL(num_s0, ram_val);
        }
}

int main()
{
        srand(time(NULL));
        UNITY_BEGIN();
        RUN_TEST(test_sb_instr);
        RUN_TEST(test_sh_instr);
        RUN_TEST(test_sw_instr);
        return UNITY_END();
}