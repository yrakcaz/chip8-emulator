#ifndef DISPLAY_HH
# define DISPLAY_HH

# include <SDL/SDL.h>
# include <iostream>
# include <cstdint>
# include "RAM.hh"

# define SCALE 20
# define SCREEN_WIDTH  (64 * SCALE)
# define SCREEN_HEIGHT (32 * SCALE)

class Display
{
    public:
        Display();
        Display(RAM* ram);
        ~Display();

        void screen_set(SDL_Surface* screen);
        void draw_sprite(int x, int y, int pos, int len);

    private:
        uint32_t getpixel_unit(int x, int y);
        void putpixel_unit(int x, int y, uint32_t pixel);
        void putpixel(int x, int y, int white);

    private:
        SDL_Surface* screen_;
        RAM* ram_;
};

#endif /* !DISPLAY_HH */
