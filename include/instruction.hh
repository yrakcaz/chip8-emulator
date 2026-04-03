#ifndef INSTRUCTION_HH
# define INSTRUCTION_HH

# include <cstdint>
# include <string>
# include <unordered_map>

class Instruction
{
    public:
        Instruction();
        Instruction(const std::string& line);
        ~Instruction();

    public:
        uint16_t get_opcode();

    private:
        void cut_line(const std::string& line);
        uint16_t get_hex(char c);
        uint16_t handle_xnnn(uint16_t x, int i);
        uint16_t handle_xy(uint16_t around, char x, char y);
        uint16_t handle_xkk(uint16_t around, char x, uint16_t kk);
        uint16_t handle_x(uint16_t around, char x);

        uint16_t asm_cls();
        uint16_t asm_ret();
        uint16_t asm_sys();
        uint16_t asm_jp();
        uint16_t asm_call();
        uint16_t asm_se();
        uint16_t asm_sne();
        uint16_t asm_ld();
        uint16_t asm_add();
        uint16_t asm_or();
        uint16_t asm_and();
        uint16_t asm_xor();
        uint16_t asm_sub();
        uint16_t asm_shr();
        uint16_t asm_subn();
        uint16_t asm_shl();
        uint16_t asm_rnd();
        uint16_t asm_drw();
        uint16_t asm_skp();
        uint16_t asm_sknp();

    private:
        using OpcodeHandler = uint16_t (Instruction::*)();

        std::string* argv_;
        int argc_;
        static const std::unordered_map<std::string, OpcodeHandler> dispatch_;
};

#endif /* !INSTRUCTION_HH */
