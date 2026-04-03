#include <disass.hh>

Disass::Disass()
    : count_(0)
    , dispatch_{
        &Disass::fmt_sys,
        &Disass::fmt_jp,
        &Disass::fmt_call,
        &Disass::fmt_se_imm,
        &Disass::fmt_sne_imm,
        &Disass::fmt_se_reg,
        &Disass::fmt_ld_imm,
        &Disass::fmt_add_imm,
        &Disass::fmt_alu,
        &Disass::fmt_sne_reg,
        &Disass::fmt_ld_i,
        &Disass::fmt_jp_v0,
        &Disass::fmt_rnd,
        &Disass::fmt_drw,
        &Disass::fmt_key,
        &Disass::fmt_misc,
    }
{
}

Disass::Disass(const char* file, int count)
    : file_(file, std::ifstream::in)
    , count_(count)
    , dispatch_{
        &Disass::fmt_sys,
        &Disass::fmt_jp,
        &Disass::fmt_call,
        &Disass::fmt_se_imm,
        &Disass::fmt_sne_imm,
        &Disass::fmt_se_reg,
        &Disass::fmt_ld_imm,
        &Disass::fmt_add_imm,
        &Disass::fmt_alu,
        &Disass::fmt_sne_reg,
        &Disass::fmt_ld_i,
        &Disass::fmt_jp_v0,
        &Disass::fmt_rnd,
        &Disass::fmt_drw,
        &Disass::fmt_key,
        &Disass::fmt_misc,
    }
{
}

Disass::~Disass()
{
    file_.close();
}

int Disass::run()
{
    uint16_t instr;

    for (int i = RAM_BEGIN; i < count_; i++)
        file_.get();

    do
    {
        instr = (file_.get() << 8) | file_.get();
        if (instr == 0xFFFF)
            continue;
        std::string literal = handle_instruction(instr);
        std::cout << "0x" << std::hex << count_ << ":\t0x" << instr << std::dec
            << ":\t\t" << literal << std::endl;
        count_ += 2;
    } while (file_.good());
    return 0;
}

std::string Disass::handle_instruction(uint16_t instr)
{
    uint8_t nibble = (instr >> 12) & 0x0F;
    return (this->*dispatch_[nibble])(instr);
}

std::string Disass::fmt_sys(uint16_t instr)
{
    char buf[32];
    uint8_t lo = instr & 0x00FF;
    if (lo == 0xE0)
        return "CLS";
    else if (lo == 0xEE)
        return "RET";
    snprintf(buf, sizeof(buf), "SYS\t%#x", instr & 0x0FFF);
    return buf;
}

std::string Disass::fmt_jp(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "JP\t%#x", instr & 0x0FFF);
    return buf;
}

std::string Disass::fmt_call(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "CALL\t%#x", instr & 0x0FFF);
    return buf;
}

std::string Disass::fmt_se_imm(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "SE\tV%x, %#x", (instr >> 8) & 0x0F, instr & 0x00FF);
    return buf;
}

std::string Disass::fmt_sne_imm(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "SNE\tV%x, %#x", (instr >> 8) & 0x0F, instr & 0x00FF);
    return buf;
}

std::string Disass::fmt_se_reg(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "SE\tV%x, V%x", (instr >> 8) & 0x0F, (instr >> 4) & 0x0F);
    return buf;
}

std::string Disass::fmt_ld_imm(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "LD\tV%x, %#x", (instr >> 8) & 0x0F, instr & 0x00FF);
    return buf;
}

std::string Disass::fmt_add_imm(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "ADD\tV%x, %#x", (instr >> 8) & 0x0F, instr & 0x00FF);
    return buf;
}

std::string Disass::fmt_alu(uint16_t instr)
{
    char buf[32];
    uint8_t x = (instr >> 8) & 0x0F;
    uint8_t y = (instr >> 4) & 0x0F;
    switch (instr & 0x0F)
    {
      case 0x0:
       snprintf(buf, sizeof(buf), "LD\tV%x, V%x",   x, y);
       break;
      case 0x1:
       snprintf(buf, sizeof(buf), "OR\tV%x, V%x",   x, y);
       break;
      case 0x2:
       snprintf(buf, sizeof(buf), "AND\tV%x, V%x",  x, y);
       break;
      case 0x3:
       snprintf(buf, sizeof(buf), "XOR\tV%x, V%x",  x, y);
       break;
      case 0x4:
       snprintf(buf, sizeof(buf), "ADD\tV%x, V%x",  x, y);
       break;
      case 0x5:
       snprintf(buf, sizeof(buf), "SUB\tV%x, V%x",  x, y);
       break;
      case 0x6:
       snprintf(buf, sizeof(buf), "SHR\tV%x, V%x",  x, y);
       break;
      case 0x7:
       snprintf(buf, sizeof(buf), "SUBN\tV%x, V%x", x, y);
       break;
      case 0xE:
       snprintf(buf, sizeof(buf), "SHL\tV%x, V%x",  x, y);
       break;
      default:
       snprintf(buf, sizeof(buf), "???");
       break;
    }
    return buf;
}

std::string Disass::fmt_sne_reg(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "SNE\tV%x, V%x", (instr >> 8) & 0x0F, (instr >> 4) & 0x0F);
    return buf;
}

std::string Disass::fmt_ld_i(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "LD\tI, %#x", instr & 0x0FFF);
    return buf;
}

std::string Disass::fmt_jp_v0(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "JP\tV0, %#x", instr & 0x0FFF);
    return buf;
}

std::string Disass::fmt_rnd(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "RND\tV%x, %#x", (instr >> 8) & 0x0F, instr & 0x00FF);
    return buf;
}

std::string Disass::fmt_drw(uint16_t instr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "DRW\tV%x, V%x, %#x",
             (instr >> 8) & 0x0F, (instr >> 4) & 0x0F, instr & 0x0F);
    return buf;
}

std::string Disass::fmt_key(uint16_t instr)
{
    char buf[32];
    uint8_t x = (instr >> 8) & 0x0F;
    uint8_t lo = instr & 0x00FF;
    if (lo == 0x9E)
        snprintf(buf, sizeof(buf), "SKP\tV%x", x);
    else if (lo == 0xA1)
        snprintf(buf, sizeof(buf), "SKNP\tV%x", x);
    else
        snprintf(buf, sizeof(buf), "???");
    return buf;
}

std::string Disass::fmt_misc(uint16_t instr)
{
    char buf[32];
    uint8_t x = (instr >> 8) & 0x0F;
    switch (instr & 0x00FF)
    {
      case 0x07:
       snprintf(buf, sizeof(buf), "LD\tV%x, DT",  x);
       break;
      case 0x0A:
       snprintf(buf, sizeof(buf), "LD\tV%x, K",   x);
       break;
      case 0x15:
       snprintf(buf, sizeof(buf), "LD\tDT, V%x",  x);
       break;
      case 0x18:
       snprintf(buf, sizeof(buf), "LD\tST, V%x",  x);
       break;
      case 0x1E:
       snprintf(buf, sizeof(buf), "ADD\tI, V%x",  x);
       break;
      case 0x29:
       snprintf(buf, sizeof(buf), "LD\tF, V%x",   x);
       break;
      case 0x33:
       snprintf(buf, sizeof(buf), "LD\tB, V%x",   x);
       break;
      case 0x55:
       snprintf(buf, sizeof(buf), "LD\t[I], V%x", x);
       break;
      case 0x65:
       snprintf(buf, sizeof(buf), "LD\tV%x, [I]", x);
       break;
      default:
       snprintf(buf, sizeof(buf), "???");
       break;
    }
    return buf;
}
