#ifndef MAPPER000_H
#define MAPPER000_H

#include <Mapper.h>


class Mapper000 : public Mapper
{
    public:
        Mapper000(uint8_t _prg_banks, uint8_t _chr_banks);
        ~Mapper000();
        uint16_t map_cpu_read(uint16_t addr) override;
        uint16_t map_cpu_write(uint16_t addr) override;
        uint16_t map_ppu_read(uint16_t addr) override;
        uint16_t map_ppu_write(uint16_t addr) override;
    protected:

    private:
};

#endif // MAPPER000_H
