#ifndef DISASS_HH
# define DISASS_HH

# include <fstream>
# include <iostream>
# include <string>

# include <ram.hh>

class Disass
{
    public:
        Disass();
        Disass(const char* file, int count);
        ~Disass();

    public:
        int run();
        std::string handle_instruction(uint16_t instr);

    private:
        std::string fmt_sys(uint16_t instr);
        std::string fmt_jp(uint16_t instr);
        std::string fmt_call(uint16_t instr);
        std::string fmt_se_imm(uint16_t instr);
        std::string fmt_sne_imm(uint16_t instr);
        std::string fmt_se_reg(uint16_t instr);
        std::string fmt_ld_imm(uint16_t instr);
        std::string fmt_add_imm(uint16_t instr);
        std::string fmt_alu(uint16_t instr);
        std::string fmt_sne_reg(uint16_t instr);
        std::string fmt_ld_i(uint16_t instr);
        std::string fmt_jp_v0(uint16_t instr);
        std::string fmt_rnd(uint16_t instr);
        std::string fmt_drw(uint16_t instr);
        std::string fmt_key(uint16_t instr);
        std::string fmt_misc(uint16_t instr);

    private:
        using Formatter = std::string (Disass::*)(uint16_t);

        std::ifstream file_;
        int count_;
        Formatter dispatch_[16];
};

#endif /* !DISASS_HH */
