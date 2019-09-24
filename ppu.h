#ifndef PPU_H
#define PPU_H
#include <cstdint>
#include "loader.h"
#include "SDL2/SDL.h"

class PPU
{
    public:
        PPU();
        ~PPU();
        uint8_t cpu_read(uint16_t addr);
        void cpu_write(uint16_t addr, uint8_t data);
        uint8_t ppu_read(uint16_t addr);
        void ppu_write(uint16_t addr, uint8_t data);
        void insert_cart(Loader* _cart);
        void inc_x();
        void inc_y();
        void step();
        void reset();
        bool nmi = false;
        SDL_Surface* surface = nullptr;
        void set_pixel_color(int x, int y, uint32_t color);
        uint8_t* get_colour(uint8_t palette, uint8_t pixel);
        void connect_screen(SDL_Surface* surface);
        void dump_memory();
        bool frame_finished;
    protected:

    private:
        uint8_t rgb_palette[64][3];
        uint8_t spr_ram[64];
        uint8_t nametable[2][1024];
        uint8_t palette[32];
        uint8_t     tblPattern[2][4096];
        Loader* cart;
        union {
            struct {
                uint8_t nametable_x : 1;
                uint8_t nametable_y : 1;
                uint8_t addr_incr : 1;
                uint8_t spr_pattern_table : 1;
                uint8_t bg_pattern_table : 1;
                uint8_t spr_size : 1;
                uint8_t slave : 1;
                uint8_t nmi : 1;
            };
            uint8_t reg;
        } PPUCTRL;
        union {
            struct {
                uint8_t greyscale : 1;
                uint8_t show_bg_corner : 1;
                uint8_t show_spr_corner : 1;
                uint8_t show_bg : 1;
                uint8_t show_spr : 1;
                uint8_t emph_r : 1;
                uint8_t emph_g : 1;
                uint8_t emph_b : 1;
            };
            uint8_t reg;
        } PPUMASK;
        union {
            struct {
                uint8_t unused : 5;
                uint8_t spr_overflow : 1;
                uint8_t spr_zero_hit : 1;
                uint8_t vblank : 1;
            };
            uint8_t reg;
        } PPUSTATUS;
        union loopy {
            struct {

                uint16_t coarse_x : 5;
                uint16_t coarse_y : 5;
                uint16_t nametable_x : 1;
                uint16_t nametable_y : 1;
                uint16_t fine_y : 3;
                uint16_t unused : 1;
            };
            uint16_t reg = 0x0000;
        };
        loopy v;
        loopy t;
        uint8_t fine_x;
        uint8_t OAMADDR;
        uint8_t OAMDATA;
        uint8_t PPUSCROLL;
        uint8_t PPUADDR;
        uint8_t PPUDATA;
        uint8_t OAMDMA;
        uint8_t loopy_latch;
        uint8_t buffer;
        int16_t scanline;
        int16_t cycle;
        uint8_t bg_next_tile_id;
        uint8_t bg_next_tile_attrib;
        uint8_t bg_next_tile_lsb;
        uint8_t bg_next_tile_msb;
        uint16_t bg_shifter_pattern_lo;
        uint16_t bg_shifter_pattern_hi;
        uint16_t bg_shifter_attrib_lo;
        uint16_t bg_shifter_attrib_hi;
};

#endif // PPU_H
