#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "ram.h"
#include "parser.h"
#include "cpu.h"

uint32_t instructions[(RAM_DATA_ADDR - RAM_INSTRUCTION_ADDR) / 4];

static void _regmap_print(cpu_s *cpu)
{
        printf("==================================================================\n");
        printf("                               REGMAP                             \n");
        printf("==================================================================\n");
        printf("| %-15s: %13s | %13s | %13s |\n", "REG", "DEC", "HEX", "OCT");
        printf("| %-15s: %13d | %13x | %13o |\n", "RA(X1)", cpu->gpr[RA], cpu->gpr[RA], cpu->gpr[RA]);
        printf("| %-15s: %13d | %13x | %13o |\n", "SP(X2)", cpu->gpr[SP], cpu->gpr[SP], cpu->gpr[SP]);
        printf("| %-15s: %13d | %13x | %13o |\n", "GP(X3)", cpu->gpr[GP], cpu->gpr[GP], cpu->gpr[GP]);
        printf("| %-15s: %13d | %13x | %13o |\n", "TP(X4)", cpu->gpr[TP], cpu->gpr[TP], cpu->gpr[TP]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T0(X5)", cpu->gpr[T0], cpu->gpr[T0], cpu->gpr[T0]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T1(X6)", cpu->gpr[T1], cpu->gpr[T1], cpu->gpr[T1]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T2(X7)", cpu->gpr[T2], cpu->gpr[T2], cpu->gpr[T2]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S0(X8)", cpu->gpr[S0], cpu->gpr[S0], cpu->gpr[S0]);
        printf("| %-15s: %13d | %13x | %13o |\n", "FP(X8)", cpu->gpr[FP], cpu->gpr[FP], cpu->gpr[FP]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S1(X9)", cpu->gpr[S1], cpu->gpr[S1], cpu->gpr[S1]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A0(X10)", cpu->gpr[A0], cpu->gpr[A0], cpu->gpr[A0]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A1(X11)", cpu->gpr[A1], cpu->gpr[A1], cpu->gpr[A1]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A2(X12)", cpu->gpr[A2], cpu->gpr[A2], cpu->gpr[A2]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A3(X13)", cpu->gpr[A3], cpu->gpr[A3], cpu->gpr[A3]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A4(X14)", cpu->gpr[A4], cpu->gpr[A4], cpu->gpr[A4]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A5(X15)", cpu->gpr[A5], cpu->gpr[A5], cpu->gpr[A5]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A6(X16)", cpu->gpr[A6], cpu->gpr[A6], cpu->gpr[A6]);
        printf("| %-15s: %13d | %13x | %13o |\n", "A7(X17)", cpu->gpr[A7], cpu->gpr[A7], cpu->gpr[A7]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S2(X18)", cpu->gpr[S2], cpu->gpr[S2], cpu->gpr[S2]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S3(X19)", cpu->gpr[S3], cpu->gpr[S3], cpu->gpr[S3]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S4(X20)", cpu->gpr[S4], cpu->gpr[S4], cpu->gpr[S4]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S5(X21)", cpu->gpr[S5], cpu->gpr[S5], cpu->gpr[S5]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S6(X22)", cpu->gpr[S6], cpu->gpr[S6], cpu->gpr[S6]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S7(X23)", cpu->gpr[S7], cpu->gpr[S7], cpu->gpr[S7]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S8(X24)", cpu->gpr[S8], cpu->gpr[S8], cpu->gpr[S8]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S9(X25)", cpu->gpr[S9], cpu->gpr[S9], cpu->gpr[S9]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S10(X26)", cpu->gpr[S10], cpu->gpr[S10], cpu->gpr[S10]);
        printf("| %-15s: %13d | %13x | %13o |\n", "S11(X27)", cpu->gpr[S11], cpu->gpr[S11], cpu->gpr[S11]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T3(X28)", cpu->gpr[T3], cpu->gpr[T3], cpu->gpr[T3]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T4(X29)", cpu->gpr[T4], cpu->gpr[T4], cpu->gpr[T4]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T5(X30)", cpu->gpr[T5], cpu->gpr[T5], cpu->gpr[T5]);
        printf("| %-15s: %13d | %13x | %13o |\n", "T6(X31)", cpu->gpr[T6], cpu->gpr[T6], cpu->gpr[T6]);
}

int main(int argc, char *argv[])
{
        if (argc < 2) {
                printf("Usage: ./riscv_emu *file_path*\n");
                return -1;
        }

        file_parse(argv[1], instructions);

        ram_s ram;
        cpu_s cpu;
        cpu_init(&cpu, &ram);
        instructions_store(cpu.ram, instructions, 3);

        cpu_start(&cpu);

        _regmap_print(&cpu);

        return 0;
}