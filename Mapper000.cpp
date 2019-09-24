#include "Mapper000.h"

Mapper000::Mapper000(uint8_t _prg_banks, uint8_t _chr_banks) : Mapper(_prg_banks, _chr_banks)
{
    //ctor
}

Mapper000::~Mapper000()
{
    //dtor
}

uint16_t Mapper000::map_cpu_read(uint16_t addr) {
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        return addr & (prg_banks > 1 ? 0x7FFF : 0x3FFF);
    }
    else return 0;
}

uint16_t Mapper000::map_cpu_write(uint16_t addr) {
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        return addr & (prg_banks > 1 ? 0x7FFF : 0x3FFF);
    }
    return 0;
}

uint16_t Mapper000::map_ppu_read(uint16_t addr) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        return addr;
    }
    else return 0;
}

uint16_t Mapper000::map_ppu_write(uint16_t addr) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        return addr;
    }
    else return 0;
}
