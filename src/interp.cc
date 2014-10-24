#include "../includes/interp.hh"

Interp::Interp()
{
}

Interp::Interp(char* file)
    : in_(file, std::ifstream::in)
    , out_(strcat(file, ".out"), std::ofstream::out)
    , file_(file)
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
    std::string line;
    int l = 0;
    int rm_flag = 0;
    while (getline(in_, line))
    {
        l++;
        line = normalize(line);
        if (!line.compare(""))
            continue;
        Instruction instr(line);
        uint16_t opcode = instr.get_opcode();
        if (opcode == 0xFFFF)
        {
            std::cerr << "Line " << l << ": Parse Error!" << std::endl;
            rm_flag = 1;
        }
        out_.write(((char*)&opcode) + 1, 1);
        out_.write((char*)&opcode, 1);
    }
    if (rm_flag)
        remove(file_);
}
