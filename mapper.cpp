#include "mapper.h"

Mapper::Mapper(uint8_t _prg_banks, uint8_t _chr_banks)
{
    prg_banks = _prg_banks;
    chr_banks = _chr_banks;
}

Mapper::~Mapper()
{
    //dtor
}
