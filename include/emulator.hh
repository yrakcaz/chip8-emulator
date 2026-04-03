#ifndef EMULATOR_HH
# define EMULATOR_HH

# include <iostream>

# include <cpu.hh>
# include <debug.hh>
# include <display.hh>
# include <keyboard.hh>

class Emulator
{
    public:
        Emulator();
        Emulator(const char* file, int dbg);
        ~Emulator();

        int run();

    private:
        void fetch_decode_execute();

        void exec_sys(const uint8_t* instr);
        void exec_jp(const uint8_t* instr);
        void exec_call(const uint8_t* instr);
        void exec_se_imm(const uint8_t* instr);
        void exec_sne_imm(const uint8_t* instr);
        void exec_se_reg(const uint8_t* instr);
        void exec_ld_imm(const uint8_t* instr);
        void exec_add_imm(const uint8_t* instr);
        void exec_alu(const uint8_t* instr);
        void exec_sne_reg(const uint8_t* instr);
        void exec_ld_i(const uint8_t* instr);
        void exec_jp_v0(const uint8_t* instr);
        void exec_rnd(const uint8_t* instr);
        void exec_drw(const uint8_t* instr);
        void exec_key(const uint8_t* instr);
        void exec_misc(const uint8_t* instr);

    private:
        using Handler = void (Emulator::*)(const uint8_t*);

        int state_;
        SDL_Surface* screen_;
        Keyboard kb_;
        CPU cpu_;
        RAM* ram_;
        Display display_;
        Debug dbg_;
        Handler dispatch_[16];
};

#endif /* !EMULATOR_HH */
