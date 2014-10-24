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

uint16_t Instruction::treat_xnnn(uint16_t x, int i)
{
    if (argc_ < i + 1)
        return 0xFFFF;
    else
    {
        uint16_t ret = (x << 12) & 0xF000;
        ret |= std::stoi(argv_[i], nullptr, 16);
        return ret;
    }
}

uint16_t Instruction::treat_xy(uint16_t around, uint16_t x, uint16_t y)
{
    uint16_t ret = around & 0xF000;
    ret |= (x & 0x000F) << 8;
    ret |= (y & 0x000F) << 4;
    ret |= around & 0x000F;
    return ret;
}

uint16_t Instruction::treat_xkk(uint16_t around, uint16_t x, uint16_t kk)
{
    uint16_t ret = around & 0xF000;
    ret |= (x & 0x000F) << 8;
    ret |= (kk & 0x00FF);
    return ret;
}

uint16_t Instruction::treat_x(uint16_t around, uint16_t x)
{
    uint16_t ret = around & 0xF0FF;
    ret |= (x & 0x000F) << 8;
    return ret;
}

uint16_t Instruction::treat_jp()
{
    if (argc_ == 2)
        return treat_xnnn(0x1, 1);
    else if (argc_ == 3 && !argv_[1].compare("V0"))
        return treat_xnnn(0xB, 2);
    else
        return 0xFFFF;
}

uint16_t Instruction::treat_se()
{
    if (argv_[1][0] == 'V' && argv_[2][0] =='V')
        return treat_xy(0x5000, argv_[1][1] - '0', argv_[2][1] - '0');
    else if (argv_[1][0] == 'V')
        return treat_xkk(0x3000, argv_[1][1] - '0', std::stoi(argv_[2], nullptr, 16));
    else
        return 0xFFFF;
}

uint16_t Instruction::treat_sne()
{
    if (argv_[1][0] == 'V' && argv_[2][0] =='V')
        return treat_xy(0x9000, argv_[1][1] - '0', argv_[2][1] - '0');
    else if (argv_[1][0] == 'V')
        return treat_xkk(0x4000, argv_[1][1] - '0', std::stoi(argv_[2], nullptr, 16));
    else
        return 0xFFFF;
}

uint16_t Instruction::treat_ld()
{
    if (argv_[1][0] == 'V' && argv_[2][0] == 'V')
        return treat_xy(0x8000, argv_[1][1] - '0', argv_[2][1] - '0');
    else if (argv_[1][0] == 'I')
        return treat_xnnn(0xA, 2);
    else if (argv_[1][0] == 'V')
    {
        if (!argv_[2].compare("DT"))
            return treat_x(0xF007, argv_[1][1] - '0');
        else if (!argv_[2].compare("K"))
            return treat_x(0xF00A, argv_[1][1] - '0');
        else if (!argv_[2].compare("[I]"))
            return treat_x(0xF065, argv_[1][1] - '0');
        else
            return treat_xkk(0x6000, argv_[1][1] - '0', std::stoi(argv_[2], nullptr, 16));
    }
    else if (argv_[2][0] == 'V')
    {
        if (!argv_[1].compare("DT"))
            return treat_x(0xF015, argv_[1][1] - '0');
        else if (!argv_[1].compare("ST"))
            return treat_x(0xF018, argv_[1][1] - '0');
        else if (!argv_[1].compare("F"))
            return treat_x(0xF029, argv_[1][1] - '0');
        else if (!argv_[1].compare("B"))
            return treat_x(0xF033, argv_[1][1] - '0');
        else if (!argv_[1].compare("[I]"))
            return treat_x(0xF055, argv_[1][1] - '0');
        else
            return 0xFFFF;
    }
    else
        return 0xFFFF;
}

uint16_t Instruction::get_opcode()
{
    if (!argv_[0].compare("CLS"))
        return 0x00E0;
    else if (!argv_[0].compare("RET"))
        return 0x00EE;
    else if (!argv_[0].compare("SYS"))
        return treat_xnnn(0x0, 1);
    else if (!argv_[0].compare("JP"))
        return treat_jp();
    else if (!argv_[0].compare("CALL"))
        return treat_xnnn(0x2, 1);
    else if (!argv_[0].compare("SE") && argc_ == 3)
        return treat_se();
    else if (!argv_[0].compare("SNE") && argc_ == 3)
        return treat_sne();
    else if (!argv_[0].compare("LD") && argc_ == 3)
        return treat_ld();
    else
        return 0xFFFF;
}
