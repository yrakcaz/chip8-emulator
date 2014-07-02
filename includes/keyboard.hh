#ifndef KEYBOARD_HH
# define KEYBOARD_HH

# include <SDL/SDL.h>
# include <map>

class Keyboard
{
    public:
        Keyboard();
        ~Keyboard();

        int is_key_pressed(int c);
        int get_key_pressed();

    private:
        SDL_Event event_;
        std::map<char, int> key_map_;
};

#endif /* !KEYBOARD_HH */
