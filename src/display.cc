#include "../includes/display.hh"

Display::Display()
{
}

Display::Display(RAM* ram)
    : ram_(ram)
{
}

Display::~Display()
{
}

void Display::screen_set(SDL_Surface* screen)
{
    screen_ = screen;
}

void Display::clear()
{
    SDL_FillRect(screen_, NULL, SDL_MapRGB(screen_->format, 0, 0, 0));
}

uint32_t Display::getpixel_unit(int x, int y)
{
    int bpp = screen_->format->BytesPerPixel;
    uint8_t *p = (uint8_t *)screen_->pixels + y * screen_->pitch + x * bpp;
    switch(bpp) 
    {
        case 1:
            return *p;
        case 2:
            return *(uint16_t *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(uint32_t *)p;
        default:
            return 0;
    }
}

void Display::putpixel_unit(int x, int y, uint32_t pixel)
{
    int bpp = screen_->format->BytesPerPixel;
    uint8_t* p = (uint8_t *)screen_->pixels + y * screen_->pitch + x * bpp;
    switch(bpp) 
    {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(uint16_t *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } 
            else 
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(uint32_t *)p = pixel;
            break;
        default:
            break;
    }
}

void Display::putpixel(int x, int y, int white, CPU* cpu)
{
    int oldc = getpixel_unit((x % SCREEN_WIDTH) * SCALE, (y % SCREEN_HEIGHT) * SCALE) == 0 ? 0 : 1;
    if (oldc == 1)
        cpu->registers_set(0xF, 1);
    int val = white ^ oldc ? 0xFF : 0x00;
    uint32_t color = SDL_MapRGB(screen_->format, val, val, val);
    for (int i = x * SCALE; i < (x * SCALE) + SCALE; i++)
        for (int j = y * SCALE; j < (y * SCALE) + SCALE; j++)
            putpixel_unit(i % SCREEN_WIDTH, j % SCREEN_HEIGHT, color);
}

void Display::draw_sprite(int x, int y, int pos, int len, CPU* cpu)
{
    int realX = x;
    int realY = y;
    cpu->registers_set(0xF, 0);
    for (int i = pos; i < pos + len; i++)
    {
        uint8_t val = ram_->ram_get(i);
        for (int j = 7; j >= 0; j--)
            putpixel(realX++, realY, (val >> j) & 1, cpu);
        realX = x;
        realY++;
    }
}
