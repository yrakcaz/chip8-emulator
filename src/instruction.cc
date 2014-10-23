#include "../includes/instruction.hh"

Instruction::Instruction()
{
}

Instruction::Instruction(std::string line)
{
    int spaces = 0;
    for (size_t i = 0; i < line.size(); i++)
        if (line[i] == ' ')
            spaces++;
    argc_ = spaces + 1;
    argv_ = new std::string[argc_];
    cut_line(line);
}

Instruction::~Instruction()
{
}

void Instruction::cut_line(std::string line)
{
    int pos = 0;
    for (size_t i = 0; i < line.size() && pos < argc_; i++)
    {
        if (line[i] == ' ' || 
                (line[i] == ',' && line[i + 1] != ' '))
        {
            pos++;
            continue;
        }
        else if (line[i] == ',')
            continue;
        else
            argv_[pos] += line[i];
    }
}

uint16_t Instruction::treat_xnnn(uint16_t x)
{
    if (argc_ < 2)
        return 0xFFFF;
    else
    {
        uint16_t ret = (x << 12) & 0xF000;
        ret |= std::stoi(argv_[1], nullptr, 16);
        return ret;
    }
}

uint16_t Instruction::get_opcode()
{
    if (!argv_[0].compare("CLS") || !argv_[0].compare("cls"))
        return 0x00E0;
    else if (!argv_[0].compare("RET") || !argv_[0].compare("ret"))
        return 0x00EE;
    else if (!argv_[0].compare("SYS") || !argv_[0].compare("sys"))
        return treat_xnnn(0x0);
    else
        return 0xFFFF;
}
