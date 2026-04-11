#ifndef KEYBOARD_HH
# define KEYBOARD_HH

# include <SDL2/SDL.h>
# include <map>

class Keyboard
{
    public:
        Keyboard();
        ~Keyboard();

        void poll();
        bool should_quit() const;
        int is_key_pressed(int c);
        int get_key_pressed();

    private:
        bool quit_;
        std::map<char, int> key_map_;
};

#endif /* !KEYBOARD_HH */
