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

void CPU::ret()
{
    pc_ = stack_[sp_--];
}

void CPU::call(uint16_t addr)
{
    stack_[sp_++] = pc_;
    pc_ = addr;
}

void CPU::randomize(uint16_t bytes)
{
    int r = random() % 256;
    r = r & (bytes & 0x00FF);
    registers_[(bytes >> 8) & 0x000F] = r;
}

void CPU::add(uint16_t bytes)
{
    int b = registers_[(bytes >> 8) & 0x000F] + registers_[(bytes >> 4) & 0x000F];
    b > 255 ? registers_[0xF] = 1 : registers_[0xF] = 0;
    registers_[(bytes >> 8) & 0x000F] = b;
}

void CPU::sub(uint16_t bytes)
{
    if (registers_[(bytes >> 8) & 0x000F] > registers_[(bytes >> 4) & 0x000F])
        registers_[0xF] = 1;
    else
        registers_[0xF] = 0;
    registers_[(bytes >> 8) & 0x000F] = registers_[(bytes >> 8) & 0x000F] - registers_[(bytes >> 4) & 0x000F];
}

void CPU::shr(uint16_t bytes)
{
    registers_[(bytes >> 8) & 0x000F] % 2 == 1 ? registers_[0xF] = 1 : registers_[0xF] = 0;
    registers_[(bytes >> 8) & 0x000F] = registers_[(bytes >> 8) & 0x000F] / 2;
}

void CPU::subn(uint16_t bytes)
{
    if (registers_[(bytes >> 8) & 0x000F] < registers_[(bytes >> 4) & 0x000F])
        registers_[0xF] = 1;
    else
        registers_[0xF] = 0;
    registers_[(bytes >> 4) & 0x000F] = registers_[(bytes >> 4) & 0x000F] - registers_[(bytes >> 8) & 0x000F];
}

void CPU::shl(uint16_t bytes)
{
    registers_[(bytes >> 8) & 0x000F] % 2 == 1 ? registers_[0xF] = 1 : registers_[0xF] = 0;
    registers_[(bytes >> 8) & 0x000F] = registers_[(bytes >> 8) & 0x000F] * 2;
}

void CPU::ldb(uint16_t bytes, RAM* ram)
{
    int v = registers_[(bytes >> 8) & 0x000F];
    ram->ram_set(Ireg_, (v / 100) % 100);
    ram->ram_set(Ireg_ + 1, (v / 10) % 10);
    int n = ((v / 100) % 100) + ((v / 10) % 10);
    v -= n;
    ram->ram_set(Ireg_ + 2, v);
}

void CPU::ld55(uint16_t bytes, RAM* ram)
{
    int v = (bytes >> 8) & 0x000F;
    for (int i = 0; i < v; i++)
        ram->ram_set(Ireg_ + i, registers_[i]);
}

void CPU::ld65(uint16_t bytes, RAM* ram)
{
    int v = (bytes >> 8) & 0x000F;
    for (int i = 0; i < v; i++)
        registers_[i] = ram->ram_get(Ireg_ + i);
}
