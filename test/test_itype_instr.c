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

void test_add()
{
        TEST_ASSERT_EQUAL(15, cpu.gpr[S0]);
}

int main()
{
        UNITY_BEGIN();
        RUN_TEST(test_add);
        return UNITY_END();
}