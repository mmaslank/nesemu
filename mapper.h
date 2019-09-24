#ifndef MAPPER_H
#define MAPPER_H
#include <cstdint>

class Mapper
{
    public:
        Mapper(uint8_t _prg_banks, uint8_t _chr_banks);
        ~Mapper();
        virtual uint16_t map_cpu_read(uint16_t addr) =0;
        virtual uint16_t map_cpu_write(uint16_t addr) =0;
        virtual uint16_t map_ppu_read(uint16_t addr) =0;
        virtual uint16_t map_ppu_write(uint16_t addr) =0;
    protected:
        uint8_t prg_banks = 0;
        uint8_t chr_banks = 0;
    private:

};

#endif // MAPPER_H
