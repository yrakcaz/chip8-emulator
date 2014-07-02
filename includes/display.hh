#ifndef DISPLAY_HH
# define DISPLAY_HH

# include <SDL/SDL.h>

# define SCALE 20

class Display
{
    public:
        Display();
        Display(SDL_Surface* screen);
        ~Display();

    private:
        SDL_Surface* screen_;
};

#endif /* !DISPLAY_HH */
