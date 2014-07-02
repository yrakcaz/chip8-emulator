#include "../includes/keyboard.hh"

Keyboard::Keyboard()
{
    for (int i = 0; i < 256; i++)
        key_map_[i] = -1;
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
    key_map_[SDLK_ESCAPE] = SDLK_ESCAPE;
}

Keyboard::~Keyboard()
{
}

int Keyboard::is_key_pressed(int c)
{
    SDL_PollEvent(&event_);
    std::map<char, int>::iterator it = key_map_.begin();
    char val = -1;
    while (it != key_map_.end())
    {
        if ((*it).second == c)
        {
            val = (*it).first;
            break;
        }
    }
    if (event_.type == SDL_KEYDOWN)
        if (event_.key.keysym.sym == c && val != -1)
            return 1;
    return 0;
}

int Keyboard::get_key_pressed()
{
    while (1)
    {
        if (event_.type == SDL_QUIT)
            return SDLK_ESCAPE;
        SDL_PollEvent(&event_);
        if (event_.type == SDL_KEYDOWN)
            return key_map_[event_.key.keysym.sym];
    }
}
