#ifndef KEYBOARD_HH
# define KEYBOARD_HH

# include <SDL/SDL.h>

class Keyboard
{
    public:
        Keyboard();
        ~Keyboard();

        int is_key_pressed(char c);
        char get_key_pressed();

    private:
        SDL_Event event_;
};

#endif /* !KEYBOARD_HH */
