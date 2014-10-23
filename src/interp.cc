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

std::string Interp::normalize(std::string line)
{
    std::string ret;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '#' || line[i] == '\n')
            return ret;
        if ((line[i] == ' ' || line[i] == '\t') &&
                (line[i + 1] == ' ' || line[i + 1] == '\t'))
            continue;
        ret += line[i];
    }
    return ret;
}

void Interp::interpret()
{
    for (std::string line; getline(in_, line);)
    {
        line = normalize(line);
        //treat normalized line!
    }
}
