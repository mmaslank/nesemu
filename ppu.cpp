#include "ppu.h"



PPU::PPU() : rgb_palette{{84,84,84}, {0,30,116}, {8,16,144}, {48,0,136}, {68,0,100}, {92,0,48}, {84,4,0}, {60,24,0}, {32,42,0}, {8,58,0}, {0,64,0}, {0,60,0}, {0,50,60}, {0,0,0},{0,0,0},{0,0,0},
                     {152,150,152}, {8,76,196}, {48,50,236}, {92,30,228}, {136,20,176}, {160,20,100}, {152,34,32}, {120,60,0}, {84,90,0}, {40,114,0}, {8,124,0}, {0,118,40}, {0,102,120}, {0,0,0},{0,0,0},{0,0,0},
                     {236,238,236}, {76,154,236}, {120,124,236}, {176,98,236}, {228,84,236}, {236,88,180}, {236,106,100}, {212,136,32}, {160,170,0}, {116,196,0}, {76,208,32}, {56,204,108}, {56,180,204}, {60,60,60},{0,0,0},{0,0,0},
                     {236,238,236}, {168,204,236}, {188,188,236}, {212,178,236}, {236,174,236}, {236,174,212}, {236,180,176}, {228,196,144}, {204,210,120}, {180,222,120}, {168,226,144}, {152,226,180}, {160,214,228}, {160,162,160},{0,0,0},{0,0,0}}
{
    //ctor
}

PPU::~PPU()
{
    //dtor
}


uint8_t PPU::cpu_read(uint16_t addr) {
    uint8_t data = 0;
    switch(addr) {
    case 0x2000:
        break; //write only
    case 0x2001:
        break; //write only
    case 0x2002:
        data = (PPUSTATUS.reg & 0xE0) | (buffer & 0x1F);    //unused bits are filled by the data buffer
        PPUSTATUS.vblank = 0;
        loopy_latch = 0;
        break;
    case 0x2003:
        break; //write only
    case 0x2004:
        data = spr_ram[OAMADDR];    //write only in early versions of Famicom and NES, mostly unused
    case 0x2005:
        break; //write only
    case 0x2006:
        break; //write only
    case 0x2007:
        data = buffer;  //reads are delayed by one cycle, UNLESS the thing being read is a palette address
        buffer = ppu_read(v.reg);
        if(v.reg >= 0x3F00) data = buffer;
        v.reg += (PPUCTRL.addr_incr ? 32 : 1);
    }
    return data;
}

void PPU::cpu_write(uint16_t addr, uint8_t data) {
    switch(addr) {
    case 0x2000:
        PPUCTRL.reg = data;
        t.nametable_x = PPUCTRL.nametable_x;
        t.nametable_y = PPUCTRL.nametable_y;
        break;
    case 0x2001:
        PPUMASK.reg = data;
        break;
    case 0x2002:
        break; //read only
    case 0x2003:
        OAMADDR = data;
        break;
    case 0x2004:
        spr_ram[OAMADDR++] = data; //mostly unused, usually DMA is used
        break;
    case 0x2005:
        if(loopy_latch == 0) {
            fine_x = data & 0x07;
            t.coarse_x = data >> 3;
            loopy_latch = 1;
        }
        else {
            t.fine_y = data & 0x07;
            t.coarse_y = data >> 3;
            loopy_latch = 0;
        }
        break;
    case 0x2006:
        if(loopy_latch == 0) {
            t.reg = (uint16_t)((data & 0x3F) << 8) | (t.reg & 0x00FF);  //upper byte first, mirrored down to 0x3F
            loopy_latch = 1;
        }
        else {
            t.reg = (t.reg & 0xFF00) | data;
            v = t;
            loopy_latch = 0;
        }
        break;
    case 0x2007:
        ppu_write(v.reg, data);
        v.reg += (PPUCTRL.addr_incr ? 32 : 1);
        break;
    }
}

uint8_t PPU::ppu_read(uint16_t addr) {
    uint8_t data = 0;
    addr &= 0x3FFF;
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        data = cart->ppu_read(addr);
        //data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if(addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
        if(cart->get_mirror() == 0)  {//horizontal
            if(addr >= 0x0000 && addr <= 0x03FF)
                data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x4000 && addr <= 0x7FFF)
                data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x8000 && addr <= 0x0BFF)
                data = nametable[1][addr & 0x03FF];
            else if(addr >= 0xC000 && addr <= 0x0FFF)
                data = nametable[1][addr & 0x03FF];
        }
        if(cart->get_mirror() == 1)  {//vertical
            if(addr >= 0x0000 && addr <= 0x03FF)
                data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x4000 && addr <= 0x7FFF)
                data = nametable[1][addr & 0x03FF];
            else if(addr >= 0x8000 && addr <= 0x0BFF)
                data = nametable[0][addr & 0x03FF];
            else if(addr >= 0xC000 && addr <= 0x0FFF)
                data = nametable[1][addr & 0x03FF];
        }
    }
    else if(addr >= 0x3F00) {
        addr &= 0x001F;
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;
        data = palette[addr] & (PPUMASK.greyscale ? 0x30 : 0x3F);
    }
    return data;
}

void PPU::ppu_write(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        cart->ppu_write(addr, data);
        //tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if(addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
        if(cart->get_mirror() == 0)  {//horizontal
            if(addr >= 0x0000 && addr <= 0x03FF)
                nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0x4000 && addr <= 0x7FFF)
                nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0x8000 && addr <= 0x0BFF)
                nametable[1][addr & 0x03FF] = data;
            else if(addr >= 0xC000 && addr <= 0x0FFF)
                nametable[1][addr & 0x03FF] = data;
        }
        if(cart->get_mirror() == 1)  {//vertical
            if(addr >= 0x0000 && addr <= 0x03FF)
                nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0x4000 && addr <= 0x7FFF)
                nametable[1][addr & 0x03FF] = data;
            else if(addr >= 0x8000 && addr <= 0x0BFF)
                nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0xC000 && addr <= 0x0FFF)
                nametable[1][addr & 0x03FF] = data;
        }
    }
    else if(addr >= 0x3F00) {
        addr &= 0x001F;
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;
        palette[addr] = data;
    }
}

void PPU::insert_cart(Loader* _cart) {
    cart = _cart;
}

void PPU::reset() {
    fine_x = 0x00;
	loopy_latch = 0x00;
	buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	PPUSTATUS.reg = 0x00;
	PPUMASK.reg = 0x00;
	PPUCTRL.reg = 0x00;
	v.reg = 0x0000;
	t.reg = 0x0000;
}

void PPU::inc_x() {
    if(PPUMASK.show_bg || PPUMASK.show_spr) {
        if(v.coarse_x == 31) {
            v.coarse_x = 0;
            v.nametable_x = ~v.nametable_x;
        }
        else v.coarse_x++;
    }
}

void PPU::inc_y() {
    if(PPUMASK.show_bg || PPUMASK.show_spr) {
        if(v.fine_y < 7) v.fine_y++;
        else {
            v.fine_y = 0;
            if(v.coarse_y == 29) {
                v.coarse_y = 0;
                v.nametable_y = ~v.nametable_y;
            }
            else if(v.coarse_y == 31)
                v.coarse_y = 0;
            else v.coarse_y++;
        }
    }
}

void PPU::step() {
    if(scanline >= -1 && scanline <= 239) {
        if(scanline == 0 && cycle == 0) cycle = 1;
        if(scanline == -1 && cycle == 1) PPUSTATUS.vblank = 0;
        if((cycle >= 2 && cycle <= 257) || (cycle >= 321 && cycle <= 337)) {
            if(PPUMASK.show_bg) {
                bg_shifter_pattern_lo <<= 1;
                bg_shifter_pattern_hi <<= 1;
                bg_shifter_attrib_lo <<= 1;
                bg_shifter_attrib_hi <<= 1;
            }
            switch((cycle - 1) % 8) {
            case 0:
                bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
                bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
                bg_shifter_attrib_lo = (bg_shifter_pattern_lo & 0xFF00) | ((bg_next_tile_attrib & 1) ? 0xFF : 0);
                bg_shifter_attrib_hi = (bg_shifter_pattern_hi & 0xFF00) | ((bg_next_tile_attrib & 2) ? 0xFF : 0);
                bg_next_tile_id = ppu_read(0x2000 | (v.reg & 0x0FFF));
                break;
            case 2:
                bg_next_tile_attrib = ppu_read(0x23C0 | (v.nametable_y << 11) | (v.nametable_x << 10) | ((v.coarse_y >> 2) << 3) | (v.coarse_x >> 2));
                if(v.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                if(v.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                bg_next_tile_attrib &= 0x03;
                break;
            case 4:
                bg_next_tile_lsb = ppu_read((PPUCTRL.bg_pattern_table << 12) + ((uint16_t)bg_next_tile_id << 4) + v.fine_y);
                break;
            case 6:
                bg_next_tile_msb = ppu_read((PPUCTRL.bg_pattern_table << 12) + ((uint16_t)bg_next_tile_id << 4) + v.fine_y + 8);
                break;
            case 7:
                inc_x();
                break;
            }
        }
        if(cycle == 256) inc_y();
        if(cycle == 257) {
            bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
            bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
            bg_shifter_attrib_lo = (bg_shifter_pattern_lo & 0xFF00) | ((bg_next_tile_attrib & 1) ? 0xFF : 0);
            bg_shifter_attrib_hi = (bg_shifter_pattern_hi & 0xFF00) | ((bg_next_tile_attrib & 2) ? 0xFF : 0);
            if(PPUMASK.show_bg || PPUMASK.show_spr) {
                v.nametable_x = t.nametable_x;
                v.coarse_x = t.coarse_x;
            }
        }
        if(cycle == 338 || cycle == 340) {
            bg_next_tile_id = ppu_read(0x2000 | (v.reg & 0x0FFF));
        }
        if(scanline == -1 && cycle >= 280 && cycle < 305) {
            if(PPUMASK.show_bg || PPUMASK.show_spr) {
                v.nametable_y = t.nametable_y;
                v.coarse_y = t.coarse_y;
                v.fine_y = t.fine_y;
            }
        }
    }
    if(scanline >= 241 && scanline < 261) {
        if(scanline == 241 && cycle == 1) {
            PPUSTATUS.vblank = 1;
            if(PPUCTRL.nmi)
                nmi = true;
        }
    }
    uint8_t rpixel = 0, rpalette = 0;
    if(PPUMASK.show_bg) {
        uint16_t mux = 0x8000 >> fine_x;
        uint8_t p0 = (bg_shifter_pattern_lo & mux) > 0;
        uint8_t p1 = (bg_shifter_pattern_hi & mux) > 0;
        rpixel = (p1 << 1) || p0;
        uint8_t pal0 = (bg_shifter_attrib_lo & mux) > 0;
        uint8_t pal1 = (bg_shifter_attrib_hi & mux) > 0;
        rpalette = (pal1 << 1) || pal0;
    }
    if(cycle > 0 && scanline >= 0 && cycle <= 256 && scanline <= 240) set_pixel_color(cycle-1, scanline, SDL_MapRGB(surface->format, get_colour(rpalette, rpixel)[2], get_colour(rpalette, rpixel)[1],get_colour(rpalette, rpixel)[0]));
    cycle++;
    if(cycle >= 341) {
        cycle = 0;
        scanline++;
        if(scanline >= 261) {
            scanline = -1;
            frame_finished = true;
        }
    }
}

uint8_t* PPU::get_colour(uint8_t pal, uint8_t pixel) {
    return rgb_palette[ppu_read(0x3F00 + (pal << 2) + pixel)];
}

void PPU::set_pixel_color(int x, int y, uint32_t color) {
    SDL_LockSurface(surface);
    uint32_t *pixels = (uint32_t*)surface->pixels;
    int dx, dy;
    for(dx=0; dx<(surface->w/256); ++dx) {
        for(dy=0; dy<(surface->h/240); ++dy) {
            int index = ((y*surface->w/256)+dy) * surface->w  + (x*surface->h/240)+dx;
            if(index >= surface->w * surface->h) index = surface->w * surface->h - 1;
            pixels[ index ] = color;
        }
    }
    SDL_UnlockSurface(surface);
}

void PPU::connect_screen(SDL_Surface* _surface) {
    surface = _surface;
}

void PPU::dump_memory() {
    for(int i = 0; i < 32; i++) printf("%02x ", palette[i]);
    printf("\n\n\n");
}
