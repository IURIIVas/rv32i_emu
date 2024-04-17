#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "ram.h"
#include "parser.h"
#include "cpu.h"

uint32_t instructions[(RAM_DATA_ADDR - RAM_INSTRUCTION_ADDR) / 4];

int main(int argc, char *argv[])
{
        // if (argc < 2) {
        //         printf("Usage: ./riscv_emu *file_path*\n");
        //         return -1;
        // }

        // int asm_fd = open(argv[1], O_RDONLY);
        // if (asm_fd == -1) {
        //         printf("Err while open file\n");
        //         return -2;
        // }

        file_parse(4, instructions);

        ram_s ram;
        cpu_s cpu;
        cpu_init(&cpu, &ram);
        instructions_store(cpu.ram, instructions, 3);

        cpu_start(&cpu);

        printf("t0: %d, t1: %d\n", cpu.gpr[T0], cpu.gpr[T1]);

        return 0;
}