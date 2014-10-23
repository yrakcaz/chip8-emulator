#include "../includes/keyboard.hh"

Keyboard::Keyboard()
{
    for (int i = 0; i < 256; i++)
        key_map_[i] = -1;
    key_map_['a'] = 0x1;
    key_map_['z'] = 0x2;
    key_map_['e'] = 0x3;
    key_map_['q'] = 0x4;
    key_map_['s'] = 0x5;
    key_map_['d'] = 0x6;
    key_map_['w'] = 0x7;
    key_map_['x'] = 0x8;
    key_map_['c'] = 0x9;
    key_map_[' '] = 0x0;
    key_map_['t'] = 0xA;
    key_map_['y'] = 0xB;
    key_map_['u'] = 0xC;
    key_map_['i'] = 0xD;
    key_map_['o'] = 0xE;
    key_map_['p'] = 0xF;
    key_map_[SDLK_ESCAPE] = SDLK_ESCAPE;
}

Keyboard::~Keyboard()
{
}

int Keyboard::is_key_pressed(int c)
{
    SDL_PollEvent(&event_);
    if (c == SDLK_ESCAPE && event_.type == SDL_QUIT)
        return 1;
    std::map<char, int>::iterator it = key_map_.begin();
    char val = -1;
    while (it != key_map_.end())
    {
        if ((*it).second == c)
        {
            val = (*it).first;
            break;
        }
        it++;
    }
    if (event_.type == SDL_KEYDOWN)
        if (event_.key.keysym.sym == val && val != -1)
            return 1;
    return 0;
}

int Keyboard::get_key_pressed()
{
    while (1)
    {
        SDL_PollEvent(&event_);
        if (event_.type == SDL_KEYDOWN)
            return key_map_[event_.key.keysym.sym];
    }
}
