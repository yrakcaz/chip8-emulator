#include <display.hh>

#include <cstring>

Display::Display()
    : renderer_(nullptr)
    , ram_(nullptr)
{
    std::memset(pixels_, 0, sizeof(pixels_));
}

Display::Display(RAM* ram)
    : renderer_(nullptr)
    , ram_(ram)
{
    std::memset(pixels_, 0, sizeof(pixels_));
}

Display::~Display()
{
}

void Display::renderer_set(SDL_Renderer* renderer)
{
    renderer_ = renderer;
}

void Display::clear()
{
    std::memset(pixels_, 0, sizeof(pixels_));
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void Display::putpixel(int x, int y, int white, CPU* cpu)
{
    int lx = x % CHIP8_WIDTH;
    int ly = y % CHIP8_HEIGHT;
    bool prev = pixels_[lx][ly];
    if (prev && white)
        cpu->registers_set(0xF, 1);
    pixels_[lx][ly] = static_cast<bool>(white) ^ prev;
    uint8_t val = pixels_[lx][ly] ? 0xFF : 0x00;
    SDL_SetRenderDrawColor(renderer_, val, val, val, 255);
    SDL_Rect rect = { lx * SCALE, ly * SCALE, SCALE, SCALE };
    SDL_RenderFillRect(renderer_, &rect);
}

void Display::draw_sprite(int x, int y, int pos, int len, CPU* cpu)
{
    cpu->registers_set(0xF, 0);
    for (int i = pos; i < pos + len; i++)
    {
        uint8_t val = ram_->ram_get(i);
        for (int j = 7; j >= 0; j--)
            putpixel(x + (7 - j), y + (i - pos), (val >> j) & 1, cpu);
    }
}
