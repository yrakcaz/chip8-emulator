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
        uint16_t handle_xnnn(uint16_t x, int i);
        uint16_t handle_xy(uint16_t around, char x, char y);
        uint16_t handle_xkk(uint16_t around, char x, uint16_t kk);
        uint16_t handle_x(uint16_t around, char x);
        uint16_t handle_jp();
        uint16_t handle_se();
        uint16_t handle_sne();
        uint16_t handle_ld();
        uint16_t handle_add();
        uint16_t handle_drw();

    private:
        std::string* argv_;
        int argc_;
};

#endif /* !INSTRUCTION_HH */
