#include <keyboard.hh>

Keyboard::Keyboard()
    : quit_(false)
{
    key_map_['a'] = 0x01;
    key_map_['z'] = 0x02;
    key_map_['e'] = 0x03;
    key_map_['q'] = 0x04;
    key_map_['s'] = 0x05;
    key_map_['d'] = 0x06;
    key_map_['w'] = 0x07;
    key_map_['x'] = 0x08;
    key_map_['c'] = 0x09;
    key_map_[' '] = 0x00;
    key_map_['t'] = 0x0A;
    key_map_['y'] = 0x0B;
    key_map_['u'] = 0x0C;
    key_map_['i'] = 0x0D;
    key_map_['o'] = 0x0E;
    key_map_['p'] = 0x0F;
}

Keyboard::~Keyboard()
{
}

void Keyboard::poll()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            quit_ = true;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            quit_ = true;
    }
}

bool Keyboard::should_quit() const
{
    return quit_;
}

int Keyboard::is_key_pressed(int c)
{
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    for (auto& kv : key_map_)
    {
        if (kv.second == c)
            return state[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(kv.first))];
    }
    return 0;
}

int Keyboard::get_key_pressed()
{
    SDL_Event event;
    while (true)
    {
        if (SDL_WaitEvent(&event) && event.type == SDL_KEYDOWN)
        {
            SDL_Keycode sym = event.key.keysym.sym;
            for (auto& kv : key_map_)
            {
                if (static_cast<SDL_Keycode>(kv.first) == sym)
                    return kv.second;
            }
        }
    }
}
