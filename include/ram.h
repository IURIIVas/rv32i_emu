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

typedef enum data_size_e {
        BYTE = 8,
        HALF_WORD = 16,
        WORD = 32
} data_size_e;

typedef struct ram_s {
        uint8_t mem[RAM_SIZE_BYTES];
} ram_s;

uint32_t ram_load(ram_s *ram, uint32_t addr, data_size_e size);
void dram_store(ram_s *ram, uint32_t addr, data_size_e size, uint32_t value);

#endif 