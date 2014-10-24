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
        uint16_t treat_xnnn(uint16_t x, int i);

    private:
        std::string* argv_;
        int argc_;
};

#endif /* !INSTRUCTION_HH */
