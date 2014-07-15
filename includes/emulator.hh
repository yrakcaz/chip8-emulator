#ifndef EMULATOR_HH
# define EMULATOR_HH

# include "keyboard.hh"
# include "CPU.hh"
# include "display.hh"
# include <iostream>

class Emulator
{
    public:
        Emulator();
        Emulator(char* file);
        ~Emulator();

        int run();

    private:
        void fetch_decode_execute();

    private:
        int state_;
        SDL_Surface* screen_;
        Keyboard kb_;
        CPU cpu_;
        RAM* ram_;
        Display display_;
};

#endif /* !EMULATOR_HH */
