#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

#include "cpu.h"


static uint32_t _instruction_parse(char *inst) 
{
        char *inst_mnemonic = (char *) malloc(15 * sizeof(char));
        size_t inst_mnem_idx = 0;
        uint8_t inst_mnemonic_readed = 0;

        while (*inst) {
                while ((*inst) == ' ' || (*inst) == '\t') {
                        inst++;
                }
                while (((*inst) != ' ' || (*inst) != '\t') && !inst_mnemonic_readed) {
                        inst_mnemonic[inst_mnem_idx++] = (*inst++);
                }
                inst_mnemonic_readed = 1;
                inst++;
        }

        return 1;
}

void file_parse(int fd, uint32_t *instruction_arr)
{
        char *instruction = "add t0,1,1";
        char *instruction_1 = "sub t0, 1, 1";
        char *instruction_2 = " ";

        _instruction_parse(instruction);
}