#include "../includes/interp.hh"

Interp::Interp()
{
}

Interp::Interp(char* file)
    : in_(file, std::ifstream::in)
    , out_(strcat(file, ".out"), std::ofstream::out)
{
}

Interp::~Interp()
{
    in_.close();
    out_.close();
}

void Interp::interpret()
{
    for (std::string line; getline(in_, line);)
    {
        //treat line!!
    }
}
