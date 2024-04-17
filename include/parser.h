#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>

void file_parse(int fd, uint32_t *instruction_arr);
uint32_t instruction_parse(char *inst);

#endif