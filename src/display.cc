#include "../includes/display.hh"

Display::Display()
{
}

Display::Display(SDL_Surface* screen)
    : screen_(screen)
{
}

Display::~Display()
{
}
