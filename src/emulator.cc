#include "../includes/emulator.hh"

Emulator::Emulator()
{
}

Emulator::Emulator(char* file, int dbg)
    : state_(0)
    , ram_(new RAM(file))
    , display_(ram_)
    , dbg_(&cpu_, ram_, dbg)
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
    uint16_t instr = (instruction[0] << 8) | instruction[1];
    switch (instruction[0] >> 4)
    {
        case 0x0:
            if (instruction[1] == 0xE0)
                display_.clear();
            else if (instruction[1] == 0xEE)
                cpu_.ret();
            else
                fprintf(stderr, "Error: %#x: unknown opcode!", instr);
            break;
        case 0x1:
            cpu_.pc_set(instr & 0x0FFF);
            break;
        case 0x2:
            cpu_.call(instr & 0x0FFF);
            break;
        case 0x3:
            if (cpu_.registers_get(instruction[0] & 0x0F) == instruction[1])
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x4:
            if (cpu_.registers_get(instruction[0] & 0x0F) != instruction[1])
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x5:
            if (cpu_.registers_get(instruction[0] & 0x0F) == cpu_.registers_get(instruction[1] >> 4))
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x6:
            cpu_.registers_set(instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x7:
            cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[0] & 0x0F) + instruction[1]);
            break;
        case 0x8:
            if ((instruction[1] & 0x0F) == 0x0)
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[1] >> 4));
            else if ((instruction[1] & 0x0F) == 0x1)
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[0] & 0x0F) | cpu_.registers_get((instruction[1] >> 4) & 0x0F));
            else if ((instruction[1] & 0x0F) == 0x2)
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[0] & 0x0F) & cpu_.registers_get((instruction[1] >> 4) & 0x0F));
            else if ((instruction[1] & 0x0F) == 0x3)
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[0] & 0x0F) ^ cpu_.registers_get((instruction[1] >> 4) & 0x0F));
            else if ((instruction[1] & 0x0F) == 0x4)
                cpu_.add(instr);
            else if ((instruction[1] & 0x0F) == 0x5)
                cpu_.sub(instr);
            else if ((instruction[1] & 0x0F) == 0x6)
                cpu_.shr(instr);
            else if ((instruction[1] & 0x0F) == 0x7)
                cpu_.subn(instr);
            else if ((instruction[1] & 0x0F) == 0xE)
                cpu_.shl(instr);
            else
                fprintf(stderr, "Error: %#x: unknown opcode!", instr);
            break;
        case 0x9:
            if (cpu_.registers_get(instruction[0] & 0x0F) != cpu_.registers_get(instruction[1] >> 4))
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0xA:
            cpu_.Ireg_set(instr & 0x0FFF);
            break;
        case 0xB:
            cpu_.pc_set((instr & 0x0FFF) + cpu_.registers_get(0));
            break;
        case 0xC:
            cpu_.randomize(instr);
            break;
        case 0xD:
            display_.draw_sprite(cpu_.registers_get(instruction[0] & 0x0F), cpu_.registers_get(instruction[1] >> 4), cpu_.Ireg_get(), instruction[1] & 0x0F, &cpu_);
            break;
        case 0xE:
            if (instruction[1] == 0x9E)
            {
                if (kb_.is_key_pressed(cpu_.registers_get(instruction[0] & 0x0F)))
                    cpu_.pc_set(cpu_.pc_get() + 2);
            }
            else if (instruction[1] == 0xA1)
            {
                if (!kb_.is_key_pressed(cpu_.registers_get(instruction[0] & 0x0F)))
                    cpu_.pc_set(cpu_.pc_get() + 2);
            }
            else
                fprintf(stderr, "Error: %#x: unknown opcode!", instr);
            break;
        case 0xF:
            if (instruction[1] == 0x07)
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.timer_get());
            else if (instruction[1] == 0x0A)
                cpu_.registers_set(instruction[0] & 0x0F, kb_.get_key_pressed());
            else if (instruction[1] == 0x15)
                cpu_.timer_set(cpu_.registers_get(instruction[0] & 0x0F));
            else if (instruction[1] == 0x18)
                cpu_.sound_set(cpu_.registers_get(instruction[0] & 0x0F));
            else if (instruction[1] == 0x1E)
                cpu_.addi(instr);
            else if (instruction[1] == 0x29)
                cpu_.Ireg_set(cpu_.registers_get(instruction[0] & 0x0F) * 5);
            else if (instruction[1] == 0x33)
                cpu_.ldb(instr, ram_);
            else if (instruction[1] == 0x55)
                cpu_.ld55(instr, ram_);
            else if (instruction[1] == 0x65)
                cpu_.ld65(instr, ram_);
            else
                fprintf(stderr, "Error: %#x: unknown opcode!", instr);
            break;
        default:
            fprintf(stderr, "Error: %#x: unknown opcode!", instr);
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
