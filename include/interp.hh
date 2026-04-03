#ifndef INTERP_HH
# define INTERP_HH

# include <cstring>
# include <fstream>
# include <iostream>
# include <string>

# include <instruction.hh>

class Interp
{
    public:
        Interp();
        Interp(char* file);
        ~Interp();

    public:
        int interpret();

    private:
        std::string normalize(const std::string& line);

    private:
        std::ifstream in_;
        std::ofstream out_;
        char* file_;
};

#endif /* !INTERP_HH */
