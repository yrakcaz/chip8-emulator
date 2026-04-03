#include <emulator.hh>

Emulator::Emulator()
{
}

Emulator::Emulator(const char* file, int dbg)
    : state_(0)
    , ram_(new RAM(file))
    , display_(ram_)
    , dbg_(&cpu_, ram_, dbg)
    , dispatch_{
        &Emulator::exec_sys,
        &Emulator::exec_jp,
        &Emulator::exec_call,
        &Emulator::exec_se_imm,
        &Emulator::exec_sne_imm,
        &Emulator::exec_se_reg,
        &Emulator::exec_ld_imm,
        &Emulator::exec_add_imm,
        &Emulator::exec_alu,
        &Emulator::exec_sne_reg,
        &Emulator::exec_ld_i,
        &Emulator::exec_jp_v0,
        &Emulator::exec_rnd,
        &Emulator::exec_drw,
        &Emulator::exec_key,
        &Emulator::exec_misc,
    }
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    screen_ = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_ANYFORMAT);
    SDL_WM_SetCaption("Chip8 Emulator", NULL);
    display_.screen_set(screen_);
}

Emulator::~Emulator()
{
    delete ram_;
}

void Emulator::fetch_decode_execute()
{
    uint8_t instruction[2];
    instruction[0] = ram_->ram_get(cpu_.pc_get());
    instruction[1] = ram_->ram_get(cpu_.pc_get() + 1);
    cpu_.pc_set(cpu_.pc_get() + 2);
    if (instruction[0] == 0 && instruction[1] == 0)
    {
        state_ = 1;
        return;
    }
    (this->*dispatch_[instruction[0] >> 4])(instruction);
}

void Emulator::exec_sys(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    if (instr[1] == 0xE0)
        display_.clear();
    else if (instr[1] == 0xEE)
        cpu_.ret();
    else
        fprintf(stderr, "Error: %#x: unknown opcode!", full);
}

void Emulator::exec_jp(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    cpu_.pc_set(full & 0x0FFF);
}

void Emulator::exec_call(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    cpu_.call(full & 0x0FFF);
}

void Emulator::exec_se_imm(const uint8_t* instr)
{
    if (cpu_.registers_get(instr[0] & 0x0F) == instr[1])
        cpu_.pc_set(cpu_.pc_get() + 2);
}

void Emulator::exec_sne_imm(const uint8_t* instr)
{
    if (cpu_.registers_get(instr[0] & 0x0F) != instr[1])
        cpu_.pc_set(cpu_.pc_get() + 2);
}

void Emulator::exec_se_reg(const uint8_t* instr)
{
    if (cpu_.registers_get(instr[0] & 0x0F) == cpu_.registers_get(instr[1] >> 4))
        cpu_.pc_set(cpu_.pc_get() + 2);
}

void Emulator::exec_ld_imm(const uint8_t* instr)
{
    cpu_.registers_set(instr[0] & 0x0F, instr[1]);
}

void Emulator::exec_add_imm(const uint8_t* instr)
{
    cpu_.registers_set(instr[0] & 0x0F, cpu_.registers_get(instr[0] & 0x0F) + instr[1]);
}

void Emulator::exec_alu(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    uint8_t x = instr[0] & 0x0F;
    uint8_t y = (instr[1] >> 4) & 0x0F;
    switch (instr[1] & 0x0F)
    {
      case 0x0:
       cpu_.registers_set(x, cpu_.registers_get(y));
       break;
      case 0x1:
       cpu_.registers_set(x, cpu_.registers_get(x) | cpu_.registers_get(y));
       break;
      case 0x2:
       cpu_.registers_set(x, cpu_.registers_get(x) & cpu_.registers_get(y));
       break;
      case 0x3:
       cpu_.registers_set(x, cpu_.registers_get(x) ^ cpu_.registers_get(y));
       break;
      case 0x4:
       cpu_.add(full);
       break;
      case 0x5:
       cpu_.sub(full);
       break;
      case 0x6:
       cpu_.shr(full);
       break;
      case 0x7:
       cpu_.subn(full);
       break;
      case 0xE:
       cpu_.shl(full);
       break;
      default:
       fprintf(stderr, "Error: %#x: unknown opcode!", full);
       break;
    }
}

void Emulator::exec_sne_reg(const uint8_t* instr)
{
    if (cpu_.registers_get(instr[0] & 0x0F) != cpu_.registers_get(instr[1] >> 4))
        cpu_.pc_set(cpu_.pc_get() + 2);
}

void Emulator::exec_ld_i(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    cpu_.ireg_set(full & 0x0FFF);
}

void Emulator::exec_jp_v0(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    cpu_.pc_set((full & 0x0FFF) + cpu_.registers_get(0));
}

void Emulator::exec_rnd(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    cpu_.randomize(full);
}

void Emulator::exec_drw(const uint8_t* instr)
{
    display_.draw_sprite(cpu_.registers_get(instr[0] & 0x0F), cpu_.registers_get(instr[1] >> 4),
                         cpu_.ireg_get(), instr[1] & 0x0F, &cpu_);
}

void Emulator::exec_key(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    if (instr[1] == 0x9E)
    {
        if (kb_.is_key_pressed(cpu_.registers_get(instr[0] & 0x0F)))
            cpu_.pc_set(cpu_.pc_get() + 2);
    }
    else if (instr[1] == 0xA1)
    {
        if (!kb_.is_key_pressed(cpu_.registers_get(instr[0] & 0x0F)))
            cpu_.pc_set(cpu_.pc_get() + 2);
    }
    else
        fprintf(stderr, "Error: %#x: unknown opcode!", full);
}

void Emulator::exec_misc(const uint8_t* instr)
{
    uint16_t full = (instr[0] << 8) | instr[1];
    uint8_t x = instr[0] & 0x0F;
    switch (instr[1])
    {
      case 0x07:
       cpu_.registers_set(x, cpu_.timer_get());
       break;
      case 0x0A:
       cpu_.registers_set(x, kb_.get_key_pressed());
       break;
      case 0x15:
       cpu_.timer_set(cpu_.registers_get(x));
       break;
      case 0x18:
       cpu_.sound_set(cpu_.registers_get(x));
       break;
      case 0x1E:
       cpu_.addi(full);
       break;
      case 0x29:
       cpu_.ireg_set(cpu_.registers_get(x) * 5);
       break;
      case 0x33:
       cpu_.ldb(full, ram_);
       break;
      case 0x55:
       cpu_.ld55(full, ram_);
       break;
      case 0x65:
       cpu_.ld65(full, ram_);
       break;
      default:
       fprintf(stderr, "Error: %#x: unknown opcode!", full);
       break;
    }
}

int Emulator::run()
{
    uint32_t last = SDL_GetTicks();
    while (!state_)
    {
        if (kb_.is_key_pressed(SDLK_ESCAPE))
            break;
        if (cpu_.timer_get() > 0)
            cpu_.timer_set(cpu_.timer_get() - 1);
        if (cpu_.sound_get() > 0)
        {
            std::cout << '\a' << std::endl;
            cpu_.sound_set(cpu_.sound_get() - 1);
        }
        dbg_.stop();
        fetch_decode_execute();
        dbg_.print_status();
        SDL_Flip(screen_);
        while (SDL_GetTicks() - last < 16)
            continue;
        last = SDL_GetTicks();
    }
    SDL_Quit();
    return 0;
}
