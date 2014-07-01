#ifndef EMULATOR_HH
# define EMULATOR_HH

# include "keyboard.hh"
# include <iostream>

class Emulator
{
    public:
        Emulator();
        Emulator(char* file);
        ~Emulator();

        int run();

    private:
        int state_;
        SDL_Surface* screen_;
        Keyboard kb_;
};

#endif /* !EMULATOR_HH */
