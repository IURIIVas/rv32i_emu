#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

#include "cpu.h"
#include "instruction.h"

size_t file_parse(char *filename, uint32_t *instruction_arr)
{
        FILE *file;
        uint32_t file_len;

        file = fopen(filename, "rb");
        if (!file) {
                fprintf(stderr, "Unable to open file %s", filename);
        }

        fseek(file, 0, SEEK_END);
        file_len = ftell(file);
        fseek(file, 0, SEEK_SET);


        uint8_t *buffer = (uint8_t *)malloc(file_len + 1);
        fread(buffer, file_len, 1, file);
        fclose(file);

        memcpy(instruction_arr, buffer, file_len * sizeof(uint8_t));
        free(buffer);

        return file_len / 4;
}