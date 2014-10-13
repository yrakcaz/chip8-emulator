#include "../includes/disass.hh"

Disass::Disass()
{
}

Disass::Disass(char* file)
    : file_(file, std::ifstream::in)
{
}

Disass::~Disass()
{
    file_.close();
}

void Disass::run()
{
    uint16_t instr;
    int count = 200;

    do
    {
        instr = (file_.get() << 8) | file_.get();
        char* litteral = treat_instruction(instr);
        std::cout << count << ":\t" << litteral << std::endl;
        delete litteral;
        count += 2;
    } while (file_.good());
}

char* Disass::treat_instruction(uint16_t instr)
{
    char* ret = new char[128];
    uint8_t instruction[2];
    instruction[0] = ((instr >> 8) & 0x00FF);
    instruction[1] = (instr & 0x00FF);
    switch (instruction[0] >> 4)
    {
        case 0x0:
            if (instruction[1] == 0xE0)
                sprintf(ret, "CLS");
            else if (instruction[1] == 0xEE)
                sprintf(ret, "RET");
            else
                sprintf(ret, "SYS\t%#x", instr & 0x0FFF);
            break;
        case 0x1:
            sprintf(ret, "JP\t%#x", instr & 0x0FFF);
            break;
        case 0x2:
            sprintf(ret, "CALL\t%#x", instr & 0x0FFF);
            break;
        case 0x3:
            sprintf(ret, "SE\tV%x, %#x", instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x4:
            sprintf(ret, "SNE\tV%x, %#x", instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x5:
            sprintf(ret, "SE\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            break;
        case 0x6:
            sprintf(ret, "LD\tV%x, %#x", instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x7:
            sprintf(ret, "ADD\tV%x, %#x", instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x8:
            if ((instruction[1] & 0x0F) == 0x0)
                sprintf(ret, "LD\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x1)
                sprintf(ret, "OR\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x2)
                sprintf(ret, "AND\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x3)
                sprintf(ret, "XOR\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x4)
                sprintf(ret, "ADD\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x5)
                sprintf(ret, "SUB\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x6)
                sprintf(ret, "SHR\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0x7)
                sprintf(ret, "SUBN\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            else if ((instruction[1] & 0x0F) == 0xE)
                sprintf(ret, "SHL\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            break;
        case 0x9:
                sprintf(ret, "SNE\tV%x, V%x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F);
            break;
        case 0xA:
            sprintf(ret, "LD\tI, %#x", instr & 0x0FFF);
            break;
        case 0xB:
            sprintf(ret, "JP\tV0, %#x", instr & 0x0FFF);
            break;
        case 0xC:
            sprintf(ret, "RND\tV%x, %#x", instruction[0] & 0x0F, instruction[1]);
            break;
        case 0xD:
            sprintf(ret, "DRW\tV%x, V%x, %#x", instruction[0] & 0x0F, (instruction[1] >> 4) & 0x0F, instruction[1] & 0x0F);
            break;
        case 0xE:
            if (instruction[1] == 0x9E)
                sprintf(ret, "SKP\tV%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0xA1)
                sprintf(ret, "SKNP\tV%x", instruction[0] & 0x0F);
            break;
        case 0xF:
            if (instruction[1] == 0x07)
                sprintf(ret, "LD\tV%x, DT", instruction[0] & 0x0F);
            else if (instruction[1] == 0x0A)
                sprintf(ret, "LD\tV%x, K", instruction[0] & 0x0F);
            else if (instruction[1] == 0x15)
                sprintf(ret, "LD\tDT, V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x18)
                sprintf(ret, "LD\tST, V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x1E)
                sprintf(ret, "ADD\tI, V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x29)
                sprintf(ret, "LD\tF, V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x33)
                sprintf(ret, "LD\tB, V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x55)
                sprintf(ret, "LD\t[I], V%x", instruction[0] & 0x0F);
            else if (instruction[1] == 0x65)
                sprintf(ret, "LD\tV%x, [I]", instruction[0] & 0x0F);
            break;
        default:
            break;
    }
    return ret;
}
