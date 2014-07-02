#include "../includes/CPU.hh"

CPU::CPU()
  : Ireg_(0)
  , timer_(0)
  , sound_(0)
  , pc_(0x200)
  , sp_(0)
{
    for (int i = 0; i < TAB_SIZE; i++)
    {
        registers_[i] = 0;
        stack_[i] = 0;
    }
}

CPU::~CPU()
{
}

uint8_t CPU::registers_get(int i)
{
    return registers_[i];
}

void CPU::registers_set(int i, uint8_t val)
{
    registers_[i] = val;
}

uint16_t CPU::Ireg_get()
{
    return Ireg_;
}

void CPU::Ireg_set(uint16_t val)
{
    Ireg_ = val;
}

uint8_t CPU::timer_get()
{
    return timer_;
}

void CPU::timer_set(uint8_t val)
{
    timer_ = val;
}

uint8_t CPU::sound_get()
{
    return sound_;
}

void CPU::sound_set(uint8_t val)
{
    sound_ = val;
}

uint16_t CPU::pc_get()
{
    return pc_;
}

void CPU::pc_set(uint16_t val)
{
    pc_ = val;
}

uint8_t CPU::sp_get()
{
    return sp_;
}

void CPU::sp_set(uint8_t val)
{
    sp_ = val;
}

uint16_t CPU::stack_get()
{
    return stack_[sp_];
}

void CPU::stack_set(uint16_t val)
{
    stack_[sp_] = val;
}