#ifndef DISASS_HH
# define DISASS_HH

# include <fstream>
# include <iostream>
# include "RAM.hh"

class Disass
{
    public:
        Disass();
        Disass(char* file, int count);
        ~Disass();

    public:
        void run();
        char* handle_instruction(uint16_t instr);

    private:
        std::ifstream file_;
        int count_;
};

#endif /* !DISASS_HH */
