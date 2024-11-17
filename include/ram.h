#ifndef _RAM_H_
#define _RAM_H_

#include <stdint.h>
#include <stddef.h>

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

static inline uint32_t _sign_extend(uint32_t val, data_size_e data_size)
{
        if ((val >> (data_size - 1)) & 1) {
                uint32_t base_val = 0;
                switch (data_size)
                {
                case BYTE:
                        base_val = 0xFFFFFF00;
                        break;
                case HALF_WORD:
                        base_val = 0xFFFF0000;
                        break;                
                default:
                        break;
                }
                return base_val | val;
        }
        else
                return (uint32_t) val;
}

typedef struct ram_s {
        uint8_t mem[RAM_SIZE_BYTES];
} ram_s;

uint32_t ram_load(ram_s *ram, uint32_t addr, data_size_e size);
void ram_store(ram_s *ram, uint32_t addr, data_size_e size, uint32_t value);
uint32_t instruction_load(ram_s *ram, uint32_t addr);
void instructions_store(ram_s *ram, uint32_t *instructions, size_t instructions_num);

#endif 