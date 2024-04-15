#ifndef _RAM_H_
#define _RAM_H_

#include <stdint.h>

#define RAM_SIZE_BYTES        (0x100000)
#define RAM_START_ADDR        (0x80000000)
#define RAM_INSTRUCTION_ADDR  (0x80000000)
#define RAM_DATA_ADDR         (0x80080000)

/*
 *       +---------------+
 *       | Address space |
 *       |   +-------+   |
 *       |   |       |   |
 *       |   | DATA  |   |
 *       |   |       |   |
 *       |   |       |   |
 *       |   +-------+   |
 *       |   |       |   |
 *       |   |       |   |
 *       |   | INSTR |   |
 *       |   +-------+   |
 *       +---------------+
*/

typedef struct ram_s {
        uint8_t mem[RAM_SIZE_BYTES];
} ram_s;

void ram_load_program(ram_s *);

#endif 