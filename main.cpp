#include <iostream>
#include "Nes.h"
#include "loader.h"
#include <SDL2/SDL.h>

int main(int argv, char** args) {
    SDL_Window* window = nullptr;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("abc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 768, 720, SDL_WINDOW_SHOWN);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    Nes nes;
    Loader loader("dkong.nes");
    loader.load();
    nes.insert_cart(&loader);
    nes.ppu.insert_cart(&loader);
    nes.ppu.connect_screen(surface);
    nes.ppu.reset();
    nes.cpu.reset6502();
    int cyclecounter = 0;
    for(;;) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {}
        int cycles = nes.cpu.step(0);
        for(int i = cycles*3; i >= 0; i--) nes.ppu.step();
        if(nes.ppu.nmi) {
            nes.ppu.nmi = false; nes.cpu.nmi6502();}
        cyclecounter++;
        if(nes.ppu.frame_finished) {
            SDL_UpdateWindowSurface(window); nes.ppu.frame_finished = false;}

    }
    return 0;
}
