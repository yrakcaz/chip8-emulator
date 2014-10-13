#ifndef DISASS_HH
# define DISASS_HH

# include <fstream>
# include <iostream>

class Disass
{
    public:
        Disass();
        Disass(char* file);
        ~Disass();

    public:
        void run();

    private:
        char* treat_instruction(uint16_t instr);

    private:
        std::ifstream file_;
};

#endif /* !DISASS_HH */
