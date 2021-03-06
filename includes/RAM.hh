#ifndef RAM_HH
# define RAM_HH

# include <cstdint>
# include <cstdio>
# include <iostream>
# include <fstream>
# include <string>

# define RAM_BEGIN 0x200

# define RAM_SIZE       4096
# define SPRITES_SIZE   80

class RAM
{
    public:
        RAM();
        RAM(char* file);
        ~RAM();

        uint8_t ram_get(int i);
        void ram_set(int i, uint8_t val);

        void print();

    private:
        uint8_t ram_[RAM_SIZE];
};

#endif /* !RAM_HH */
