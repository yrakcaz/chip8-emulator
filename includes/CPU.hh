#ifndef CPU_HH
# define CPU_HH

# define TAB_SIZE 16

# include <cstdint>

class CPU
{
    public:
        CPU();
        ~CPU();

        uint8_t registers_get(int i);
        void registers_set(int i, uint8_t val);

        uint16_t Ireg_get();
        void Ireg_set(uint16_t val);

        uint8_t timer_get();
        void timer_set(uint8_t val);

        uint8_t sound_get();
        void sound_set(uint8_t val);

        uint16_t pc_get();
        void pc_set(uint16_t val);

        uint8_t sp_get();
        void sp_set(uint8_t val);

        uint16_t stack_get();
        void stack_set(uint16_t val);

        void ret();
        void call(uint16_t addr);

    private:
        uint8_t registers_[TAB_SIZE];
        uint16_t Ireg_;
        uint8_t timer_;
        uint8_t sound_;
        uint16_t pc_;
        uint8_t sp_;
        uint16_t stack_[TAB_SIZE];
};

#endif /* !CPU_HH */
