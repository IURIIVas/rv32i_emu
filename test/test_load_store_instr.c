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

/*
*     x[rd] = sext(M[x[rs1] + sext(offset)][7:0])
*/
void test_lb_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t mem_val = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = (offset << 20) | (S1 << 15) | (LB << 12) | (S0 << 7) | LOAD;
                instructions[0] = instr;

                cpu.gpr[S1] = base;
                cpu.ram->mem[base + offset - RAM_START_ADDR] = mem_val & 0xff;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t mem_extended_val = _sign_extend((uint8_t) (mem_val & 0xff), BYTE);
                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, cpu.gpr[S0], mem_extended_val);
                TEST_ASSERT_EQUAL(mem_extended_val, cpu.gpr[S0]);
        }
}

/*
*     x[rd] = sext(M[x[rs1] + sext(offset)][7:0])
*/
void test_lbu_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t mem_val = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = (offset << 20) | (S1 << 15) | (LBU << 12) | (S0 << 7) | LOAD;
                instructions[0] = instr;

                cpu.gpr[S1] = base;
                cpu.ram->mem[base + offset - RAM_START_ADDR] = mem_val & 0xff;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t mem_extended_val = (uint8_t) (mem_val & 0xff);
                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, cpu.gpr[S0], mem_extended_val);
                TEST_ASSERT_EQUAL(mem_extended_val, cpu.gpr[S0]);
        }
}

/*
*     x[rd] = sext(M[x[rs1] + sext(offset)][15:0])
*/
void test_lh_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t mem_val = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = (offset << 20) | (S1 << 15) | (LH << 12) | (S0 << 7) | LOAD;
                instructions[0] = instr;

                cpu.gpr[S1] = base;
                cpu.ram->mem[base + offset - RAM_START_ADDR] = mem_val & 0xff;
                cpu.ram->mem[base + offset - RAM_START_ADDR + 1] = (mem_val >> 8) & 0xff;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t mem_extended_val = _sign_extend((mem_val & 0xffff), HALF_WORD);
                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, cpu.gpr[S0], mem_extended_val);
                TEST_ASSERT_EQUAL(mem_extended_val, cpu.gpr[S0]);
        }
}

/*
*     x[rd] = M[x[rs1] + sext(offset)][15:0]
*/
void test_lhu_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t mem_val = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = (offset << 20) | (S1 << 15) | (LHU << 12) | (S0 << 7) | LOAD;
                instructions[0] = instr;

                cpu.gpr[S1] = base;
                cpu.ram->mem[base + offset - RAM_START_ADDR] = mem_val & 0xff;
                cpu.ram->mem[base + offset - RAM_START_ADDR + 1] = (mem_val >> 8) & 0xff;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t mem_extended_val = mem_val & 0xffff;
                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, cpu.gpr[S0], mem_extended_val);
                TEST_ASSERT_EQUAL(mem_extended_val, cpu.gpr[S0]);
        }
}


/*
*     x[rd] = sext(M[x[rs1] + sext(offset)][31:0])
*/
void test_lw_instr()
{      
        uint32_t instr = 0;
        uint32_t instructions[] = {instr};

        printf("      ADDR:        MEM;        EXP\n");
        for (size_t i = 0; i < 100; i++) {
                uint16_t offset = rand() & 0xfff;
                uint32_t mem_val = rand();
                uint32_t base = (rand() % 0x7f00) | RAM_DATA_ADDR;
                
                instr = (offset << 20) | (S1 << 15) | (LW << 12) | (S0 << 7) | LOAD;
                instructions[0] = instr;

                cpu.gpr[S1] = base;
                cpu.ram->mem[base + offset - RAM_START_ADDR] = mem_val & 0xff;
                cpu.ram->mem[base + offset - RAM_START_ADDR + 1] = (mem_val >> 8) & 0xff;
                cpu.ram->mem[base + offset - RAM_START_ADDR + 2] = (mem_val >> 16) & 0xff;
                cpu.ram->mem[base + offset - RAM_START_ADDR + 3] = (mem_val >> 24) & 0xff;

                cpu.pc = RAM_INSTRUCTION_ADDR;
                instructions_store(cpu.ram, instructions, 1);
                cpu_start(&cpu);

                uint32_t mem_extended_val = mem_val;
                printf("0x%x: 0x%08x; 0x%08x\n", base + offset, cpu.gpr[S0], mem_extended_val);
                TEST_ASSERT_EQUAL(mem_extended_val, cpu.gpr[S0]);
        }
}



int main()
{
        srand(time(NULL));
        UNITY_BEGIN();
        RUN_TEST(test_sb_instr);
        RUN_TEST(test_sh_instr);
        RUN_TEST(test_sw_instr);
        RUN_TEST(test_lb_instr);
        RUN_TEST(test_lbu_instr);
        RUN_TEST(test_lh_instr);
        RUN_TEST(test_lhu_instr);
        RUN_TEST(test_lw_instr);
        return UNITY_END();
}