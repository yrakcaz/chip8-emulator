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

uint16_t Instruction::get_hex(char c)
{
    uint16_t ret;
    if (c > '9')
    {
        if (c >= 'a' && c <= 'z')
            ret = 10 + (c - 'a');
        else
            ret = 10 + (c - 'A');
    }
    else
        ret = c - '0';
    return ret;
}

uint16_t Instruction::handle_xnnn(uint16_t x, int i)
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

uint16_t Instruction::handle_xy(uint16_t around, char x, char y)
{
    uint16_t vx = get_hex(x);
    uint16_t vy = get_hex(y);
    uint16_t ret = around & 0xF000;
    ret |= (vx & 0x000F) << 8;
    ret |= (vy & 0x000F) << 4;
    ret |= around & 0x000F;
    return ret;
}

uint16_t Instruction::handle_xkk(uint16_t around, char x, uint16_t kk)
{
    uint16_t vx = get_hex(x);
    uint16_t ret = around & 0xF000;
    ret |= (vx & 0x000F) << 8;
    ret |= (kk & 0x00FF);
    return ret;
}

uint16_t Instruction::handle_x(uint16_t around, char x)
{
    uint16_t vx = get_hex(x);
    uint16_t ret = around & 0xF0FF;
    ret |= (vx & 0x000F) << 8;
    return ret;
}

uint16_t Instruction::handle_jp()
{
    if (argc_ == 2)
        return handle_xnnn(0x1, 1);
    else if (argc_ == 3 && !argv_[1].compare("V0"))
        return handle_xnnn(0xB, 2);
    else
        return 0xFFFF;
}

uint16_t Instruction::handle_se()
{
    if (argv_[1][0] == 'V' && argv_[2][0] =='V')
        return handle_xy(0x5000, argv_[1][1], argv_[2][1]);
    else if (argv_[1][0] == 'V')
        return handle_xkk(0x3000, argv_[1][1], std::stoi(argv_[2], nullptr, 16));
    else
        return 0xFFFF;
}

uint16_t Instruction::handle_sne()
{
    if (argv_[1][0] == 'V' && argv_[2][0] =='V')
        return handle_xy(0x9000, argv_[1][1], argv_[2][1]);
    else if (argv_[1][0] == 'V')
        return handle_xkk(0x4000, argv_[1][1], std::stoi(argv_[2], nullptr, 16));
    else
        return 0xFFFF;
}

uint16_t Instruction::handle_ld()
{
    if (argv_[1][0] == 'V' && argv_[2][0] == 'V')
        return handle_xy(0x8000, argv_[1][1], argv_[2][1]);
    else if (argv_[1][0] == 'I')
        return handle_xnnn(0xA, 2);
    else if (argv_[1][0] == 'V')
    {
        if (!argv_[2].compare("DT"))
            return handle_x(0xF007, argv_[1][1]);
        else if (!argv_[2].compare("K"))
            return handle_x(0xF00A, argv_[1][1]);
        else if (!argv_[2].compare("[I]"))
            return handle_x(0xF065, argv_[1][1]);
        else
            return handle_xkk(0x6000, argv_[1][1], std::stoi(argv_[2], nullptr, 16));
    }
    else if (argv_[2][0] == 'V')
    {
        if (!argv_[1].compare("DT"))
            return handle_x(0xF015, argv_[2][1]);
        else if (!argv_[1].compare("ST"))
            return handle_x(0xF018, argv_[2][1]);
        else if (!argv_[1].compare("F"))
            return handle_x(0xF029, argv_[2][1]);
        else if (!argv_[1].compare("B"))
            return handle_x(0xF033, argv_[2][1]);
        else if (!argv_[1].compare("[I]"))
            return handle_x(0xF055, argv_[2][1]);
        else
            return 0xFFFF;
    }
    else
        return 0xFFFF;
}

uint16_t Instruction::handle_add()
{
    if (argv_[1][0] == 'V')
    {
        if (argv_[2][0] == 'V')
            return handle_xy(0x8004, argv_[1][1], argv_[2][1]);
        else
            return handle_xkk(0x7000, argv_[1][1], std::stoi(argv_[2], nullptr, 16));
    }
    else if (argv_[2][0] == 'V')
    {
        if (!argv_[1].compare("I"))
            return handle_x(0xF01E, argv_[2][1]);
        else
            return 0xFFFF;
    }
    else
        return 0xFFFF;
}

uint16_t Instruction::handle_drw()
{
    uint16_t ret = 0xD000;
    uint16_t x = get_hex(argv_[1][1]);
    uint16_t y = get_hex(argv_[2][1]);
    uint16_t n = get_hex(argv_[3][0] == '0' ? argv_[3][2] : argv_[3][0]);
    x <<= 8;
    y <<= 4;
    ret |= x;
    ret |= y;
    ret |= n;
    return ret;
}

uint16_t Instruction::get_opcode()
{
    if (!argv_[0].compare("CLS"))
        return 0x00E0;
    else if (!argv_[0].compare("RET"))
        return 0x00EE;
    else if (!argv_[0].compare("SYS"))
        return handle_xnnn(0x0, 1);
    else if (!argv_[0].compare("JP"))
        return handle_jp();
    else if (!argv_[0].compare("CALL"))
        return handle_xnnn(0x2, 1);
    else if (!argv_[0].compare("SE") && argc_ == 3)
        return handle_se();
    else if (!argv_[0].compare("SNE") && argc_ == 3)
        return handle_sne();
    else if (!argv_[0].compare("LD") && argc_ == 3)
        return handle_ld();
    else if (!argv_[0].compare("ADD") && argc_ == 3)
        return handle_add();
    else if (!argv_[0].compare("OR") && argc_ == 3)
        return handle_xy(0x8001, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("AND") && argc_ == 3)
        return handle_xy(0x8002, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("XOR") && argc_ == 3)
        return handle_xy(0x8003, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("SUB") && argc_ == 3)
        return handle_xy(0x8005, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("SHR") && argc_ == 3)
        return handle_xy(0x8006, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("SUBN") && argc_ == 3)
        return handle_xy(0x8007, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("SHL") && argc_ == 3)
        return handle_xy(0x800E, argv_[1][1], argv_[2][1]);
    else if (!argv_[0].compare("RND") && argc_ == 3)
        return handle_xkk(0xC000, argv_[1][1], std::stoi(argv_[2], nullptr, 16));
    else if (!argv_[0].compare("DRW") && argc_ == 4)
            return handle_drw();
    else if (!argv_[0].compare("SKP") && argc_ == 2)
            return handle_x(0xE09E, argv_[1][1]);
    else if (!argv_[0].compare("SKNP") && argc_ == 2)
            return handle_x(0xE0A1, argv_[1][1]);
    else
        return 0xFFFF;
}
