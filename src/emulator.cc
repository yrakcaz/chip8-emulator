#include "../includes/emulator.hh"

#define SCALE         20
#define SCREEN_WIDTH  (64 * SCALE)
#define SCREEN_HEIGHT (32 * SCALE)

Emulator::Emulator()
{
}

Emulator::Emulator(char* file)
    : state_(0)
    , ram_(file)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    screen_ = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_ANYFORMAT);
    SDL_WM_SetCaption("Chip8 Emulator", NULL);
    ram_.print();
}

Emulator::~Emulator()
{
}

int Emulator::run()
{
    while (!state_)
    {
        if (kb_.is_key_pressed(SDLK_ESCAPE))
            break;
    }
    SDL_Quit();
    return 0;
}
