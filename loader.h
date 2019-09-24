#ifndef _LOADER_H_
#define _LOADER_H_
#include <string>
#include <vector>
#include <memory>
#include "Mapper000.h"

class Loader {
    std::string filename;
    std::vector<uint8_t> prg_memory;
    std::vector<uint8_t> chr_memory;
    uint8_t mapperID;
    uint8_t mirror;
    uint8_t prg_banks;
    uint8_t chr_banks;
    Mapper* mapper;
public:
    Loader(std::string _filename);
    bool load();
    uint8_t cpu_read(uint16_t addr);
    void cpu_write(uint16_t addr, uint8_t data);
    uint8_t ppu_read(uint16_t addr);
    void ppu_write(uint16_t addr, uint8_t data);
    uint8_t get_mirror();
};
#endif
