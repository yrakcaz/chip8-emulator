#include "../includes/keyboard.hh"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

int Keyboard::is_key_pressed(char c)
{
    SDL_PollEvent(&event_);
    if (event_.type == SDL_KEYDOWN)
        if (event_.key.keysym.sym == c)
            return 1;
    return 0;
}

char Keyboard::get_key_pressed()
{
    while (1)
    {
        if (event_.type == SDL_QUIT)
            return SDLK_ESCAPE;
        SDL_PollEvent(&event_);
        if (event_.type == SDL_KEYDOWN)
            return event_.key.keysym.sym;
    }
}
