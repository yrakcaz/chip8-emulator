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
}

Emulator::~Emulator()
{
}

int Emulator::run()
{
    uint32_t last = SDL_GetTicks();
    while (!state_)
    {
        if (kb_.is_key_pressed(SDLK_ESCAPE))
            break;
        if (cpu_.timer_get() > 0)
            cpu_.timer_set(cpu_.timer_get() - 1);
        if (cpu_.sound_get() > 0)
        {
            std::cout << '\a' << std::endl;
            cpu_.sound_set(cpu_.sound_get() - 1);
        }

        // Fetch, Decode, Execute here!

        while (SDL_GetTicks() - last < 16)
            continue;
        last = SDL_GetTicks();
    }
    SDL_Quit();
    return 0;
}
