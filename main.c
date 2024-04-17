#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "include/ram.h"
#include "include/parser.h"

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
}