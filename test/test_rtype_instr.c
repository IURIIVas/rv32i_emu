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
        case R_TYPE:
                decoded_instr.rd = (instr >> 7) & 0x1f;
                decoded_instr.funct_3 = (instr >> 12) & 0x7;
                decoded_instr.rs1 = (instr >> 15) & 0x1f;
                decoded_instr.rs2 = (instr >> 20) & 0x1f;
                decoded_instr.funct_7 = (instr >> 25) & 0x7f;
                decoded_instr.imm = 0; 
                break;

*/
void test_add_instr()
{      
/*
cpu->gpr[instr->rd] = (uint32_t) ((int32_t) cpu->gpr[instr->rs1] + (int32_t) cpu->gpr[instr->rs2]);
*/
        cpu.gpr[S0] = 5;
        cpu.gpr[S1] = 10;
        uint32_t instr = (S0 << 20) | (S1 << 15) | (S0 << 7) | R_TYPE;

        uint32_t instructions[] = {instr};
        instructions_store(cpu.ram, instructions, 1);
        cpu_start(&cpu);

        TEST_ASSERT_EQUAL(15, cpu.gpr[S0]);
}

int main(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_add_instr);
    return UNITY_END();
}