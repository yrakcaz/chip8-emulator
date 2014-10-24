#ifndef INTERP_HH
# define INTERP_HH

# include <iostream>
# include <fstream>
# include <cstring>
# include <string>

# include "instruction.hh"

class Interp
{
    public:
        Interp();
        Interp(char* file);
        ~Interp();

    public:
        void interpret();

    private:
        std::string normalize(std::string line);

    private:
        std::ifstream in_;
        std::ofstream out_;
        char* file_;
};

#endif /* !INTERP_HH */
