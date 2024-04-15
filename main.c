#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>



void file_parse(int fd)
{
        char *instruction = "add t0,1,1";
}

int main(int argc, char *argv[])
{
        if (argc < 2) {
                printf("Usage: ./riscv_emu *file_path*\n");
                return -1;
        }

        int asm_fd = open(argv[1], O_RDONLY);
        if (asm_fd == -1) {
                printf("Err while open file\n");
                return -2;
        }


}