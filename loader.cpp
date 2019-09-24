#include <iostream>
#include <fstream>
#include <cstdio>
#include "loader.h"

using namespace std;

Loader::Loader(string _filename) {
    filename = _filename;
    prg_banks = 0;
    chr_banks = 0;
}

bool Loader::load() {
    struct iNESheader {
        char nes[4];
        uint8_t prg_chunks;
        uint8_t chr_chunks;
        uint8_t mapperlo;
        uint8_t mapperhi;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char padding[5];
    } header;
    std::ifstream f;
    f.open(filename, std::ifstream::binary);
    if(f.is_open()) {
        f.read((char*)&header, sizeof(iNESheader));
        if(header.mapperlo & 0x04)
            f.seekg(512, std::ios_base::cur); //skipping trainer
        mapperID = (header.mapperlo >> 4) | (header.mapperhi & 0xF0);
        mirror = header.mapperlo & 1;
        prg_banks = header.prg_chunks;
        prg_memory.resize(prg_banks * 16384); // PRG chunks are 16 kB in size
        f.read((char*)prg_memory.data(), prg_memory.size());
        chr_banks = header.chr_chunks;
        chr_memory.resize(chr_banks * 8192); // CHR chunks are 8 kB in size
        f.read((char*)chr_memory.data(), chr_memory.size());
        switch(mapperID) {
        case 0:
            mapper = new Mapper000(prg_banks, chr_banks);
            break;
        }
        f.close();
        return true;
    }
    else {
        cerr << "Nie znaleziono pliku!";
        return false;
    }
}

uint8_t Loader::cpu_read(uint16_t addr) {
    uint16_t map_addr = mapper->map_cpu_read(addr);
    return prg_memory[map_addr];
}

void Loader::cpu_write(uint16_t addr, uint8_t data) {
    uint16_t map_addr = mapper->map_cpu_write(addr);
    prg_memory[map_addr] = data;
}

uint8_t Loader::ppu_read(uint16_t addr) {
    uint16_t map_addr = mapper->map_ppu_read(addr);
    return chr_memory[map_addr];
}

void Loader::ppu_write(uint16_t addr, uint8_t data) {
    uint16_t map_addr = mapper->map_ppu_write(addr);
    chr_memory[map_addr] = data;
}

uint8_t Loader::get_mirror() {
    return mirror;
}
