#ifndef INSTRUCTION_HH
# define INSTRUCTION_HH

# include <string>

class Instruction
{
    public:
        Instruction();
        Instruction(std::string line);
        ~Instruction();

    public:
        uint16_t get_opcode();

    private:
        void cut_line(std::string line);
        uint16_t get_hex(char c);
        uint16_t treat_xnnn(uint16_t x, int i);
        uint16_t treat_xy(uint16_t around, char x, char y);
        uint16_t treat_xkk(uint16_t around, char x, uint16_t kk);
        uint16_t treat_x(uint16_t around, char x);
        uint16_t treat_jp();
        uint16_t treat_se();
        uint16_t treat_sne();
        uint16_t treat_ld();

    private:
        std::string* argv_;
        int argc_;
};

#endif /* !INSTRUCTION_HH */
