#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>

size_t file_parse(char *filename, uint32_t *instruction_arr);

#endif