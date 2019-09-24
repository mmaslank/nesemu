#ifndef NES_H
#define NES_H
#include "cpu.h"
#include "ppu.h"
#include "loader.h"

class Nes
{
    public:
        Nes();
        ~Nes();
    CPU cpu;
    PPU ppu;
    Loader* cart;
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void insert_cart(Loader* x);
    protected:

    private:
};

#endif // NES_H
