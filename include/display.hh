#ifndef DISPLAY_HH
# define DISPLAY_HH

# include <SDL2/SDL.h>
# include <cstdint>

# include <cpu.hh>
# include <ram.hh>

# define CHIP8_WIDTH 64
# define CHIP8_HEIGHT 32
# define SCALE 10
# define SCREEN_WIDTH (CHIP8_WIDTH * SCALE)
# define SCREEN_HEIGHT (CHIP8_HEIGHT * SCALE)

class Display
{
    public:
        Display();
        Display(RAM* ram);
        ~Display();

        void renderer_set(SDL_Renderer* renderer);
        void draw_sprite(int x, int y, int pos, int len, CPU* cpu);
        void clear();

    private:
        void putpixel(int x, int y, int white, CPU* cpu);

    private:
        SDL_Renderer* renderer_;
        RAM* ram_;
        bool pixels_[CHIP8_WIDTH][CHIP8_HEIGHT];
};

#endif /* !DISPLAY_HH */
