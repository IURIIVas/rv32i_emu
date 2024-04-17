#include "ram.h"

static uint8_t _ram_load_byte(ram_s *ram, uint32_t addr)
{
        return ram->mem[addr - RAM_START_ADDR];
}

static uint16_t _ram_load_half_word(ram_s *ram, uint32_t addr)
{
        uint16_t data_return =  ram->mem[addr - RAM_START_ADDR]
                | ram->mem[addr - RAM_START_ADDR + 1] << 8;
        return data_return;
}

static uint32_t _ram_load_word(ram_s *ram, uint32_t addr)
{
        uint32_t data_return = ram->mem[addr - RAM_START_ADDR]          
                | ram->mem[addr - RAM_START_ADDR + 1] << 8 
                | ram->mem[addr - RAM_START_ADDR + 2] << 16
                | ram->mem[addr - RAM_START_ADDR + 3] << 24;
        return data_return;
}

static void _ram_store_byte(ram_s *ram, uint32_t addr, uint32_t value) 
{
    ram->mem[addr - RAM_START_ADDR] = (uint8_t) (value & 0xff);
}
static void _ram_store_half_word(ram_s *ram, uint32_t addr, uint32_t value) 
{
    ram->mem[addr - RAM_START_ADDR] =     (uint8_t) (value & 0xff);
    ram->mem[addr - RAM_START_ADDR + 1] = (uint8_t) ((value >> 8) & 0xff);
}
static void _ram_store_word(ram_s *ram, uint32_t addr, uint32_t value) 
{
    ram->mem[addr - RAM_START_ADDR] =     (uint8_t) (value & 0xff);
    ram->mem[addr - RAM_START_ADDR + 1] = (uint8_t) ((value >> 8) & 0xff);
    ram->mem[addr - RAM_START_ADDR + 2] = (uint8_t) ((value >> 16) & 0xff);
    ram->mem[addr - RAM_START_ADDR + 3] = (uint8_t) ((value >> 24) & 0xff);
}

void dram_store(ram_s *ram, uint32_t addr, data_size_e size, uint32_t value) 
{
    switch (size) {
        case BYTE:  
                _ram_store_byte(ram, addr, value);  
                break;
        case HALF_WORD: 
                _ram_store_half_word(ram, addr, value); 
                break;
        case WORD: 
                _ram_store_word(ram, addr, value); 
                break;
        default: 
                break;
    }
}

uint32_t ram_load(ram_s *ram, uint32_t addr, data_size_e size)
{
        uint32_t data_load = 0;
        switch (size)
        {
        case BYTE:
                data_load = (uint32_t) _ram_load_byte(ram, addr);
                break;
        case HALF_WORD:
                data_load = (uint32_t) _ram_load_half_word(ram, addr);
                break;
        case WORD:
                data_load = (uint32_t) _ram_load_word(ram, addr);
                break;
        default:
                break;
        }

        return data_load;
}