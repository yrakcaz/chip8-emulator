#include "../includes/debug.hh"

Debug::Debug()
{
}

Debug::Debug(CPU* cpu, RAM* ram, int on)
    : cpu_(cpu)
    , ram_(ram)
    , on_(on)
    , i_(0)
{
}

Debug::~Debug()
{
}

void Debug::stop()
{
    if (on_)
    {
        wait();
        print_instr();
        i_++;
    }
}

void Debug::wait()
{
    char c;
    while (read(STDIN_FILENO, &c, 1) == 0) {}
}

void Debug::print_instr()
{
    Disass disass;
    uint8_t instruction[2];
    instruction[0] = ram_->ram_get(cpu_->pc_get());
    instruction[1] = ram_->ram_get(cpu_->pc_get() + 1);
    uint16_t instr = (instruction[0] << 8) | instruction[1];
    std::cout << i_ << " :\t0x" << std::hex << cpu_->pc_get() << std::dec;
    std::cout << "\t" << disass.treat_instruction(instr) << std::endl << std::endl;
}

void Debug::print_status()
{
    if (on_)
    {
        std::cout << " V0 | V1 | V2 | V3 | V4 | V5 | V6 | V7 | V8 | V9 | VA | VB | VC | VD | VE | VF " << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        int i;
        for (i = 0x0; i < 0xF; i++)
            printf("0x%2x|", cpu_->registers_get(i));
        printf("0x%2x\n\n", cpu_->registers_get(i));

        std::cout << " S0 | S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 | S9 | SA | SB | SC | SD | SE | SF " << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        for (i = 0x0; i < 0xF; i++)
            printf("%4x|", cpu_->stack_get(i));
        printf("%4x\n\n", cpu_->stack_get(i));

        printf("PC : 0x%x\n", cpu_->pc_get());
        printf("SP : 0x%x\n", cpu_->sp_get());
        printf("I : 0x%x\n", cpu_->Ireg_get());
        printf("Timer : 0x%x\n", cpu_->timer_get());
        printf("Sound : 0x%x\n", cpu_->sound_get());

        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }
}
