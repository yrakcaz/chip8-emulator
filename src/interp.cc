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
    int count = 0;
    int begin_flag = 0;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '#' || line[i] == '\n')
            return ret;
        if ((line[i] == ' ' || line[i] == '\t') &&
                (line[i + 1] == ' ' || line[i + 1] == '\t'))
            continue;
        if ((line[i] < 'A' || line[i] > 'Z') && !begin_flag)
            continue;
        if ((line[i] >= 'A' && line[i] <= 'Z') && !begin_flag)
            begin_flag++;
        ret += line[i] == '\t' ? ' ' : line[i];
        count++;
    }
    return ret;
}

void Interp::interpret()
{
    for (std::string line; getline(in_, line);)
    {
        line = normalize(line);
        out_ << line << std::endl;
        //treat normalized line!
    }
}
