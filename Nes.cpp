#include "Nes.h"

Nes::Nes()
{
    cpu.connect_bus(this);
}

Nes::~Nes()
{
    //dtor
}

uint8_t Nes::read(uint16_t addr) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        return cpu.read(addr & 0x07FF);
    }
    else if(addr >= 0x2000 && addr <= 0x3FFF) {
        return ppu.cpu_read(addr & 0x2007);
    }
    else if(addr <= 0x4017) {
        //apu
    }
    else {
        return cart->cpu_read(addr);
    }
}

void Nes::write(uint16_t addr, uint8_t data) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        cpu.write(addr & 0x07FF, data);
    }
    else if(addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpu_write(addr & 0x2007, data);
    }
    else if(addr <= 0x4017) {
        //apu
    }
    else {
        cart->cpu_write(addr, data);
    }
}

void Nes::insert_cart(Loader* x) {
    cart = x;
}


