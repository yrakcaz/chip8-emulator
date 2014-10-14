#include "../includes/emulator.hh"

Emulator::Emulator()
{
}

Emulator::Emulator(char* file)
    : state_(0)
    , ram_(new RAM(file))
    , display_(ram_)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    screen_ = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_ANYFORMAT);
    SDL_WM_SetCaption("Chip8 Emulator", NULL);
    display_.screen_set(screen_);
}

Emulator::~Emulator()
{
}

void Emulator::fetch_decode_execute()
{
    uint8_t instruction[2];
    instruction[0] = ram_->ram_get(cpu_.pc_get());
    instruction[1] = ram_->ram_get(cpu_.pc_get() + 1);
    printf("PC:%x\n", cpu_.pc_get());
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
            {
                printf("%x:%x:CLS\n", instruction[0], instruction[1]);
                display_.clear();
            }
            else if (instruction[1] == 0xEE)
            {
                printf("%x:%x:RET\n", instruction[0], instruction[1]);
                cpu_.ret();
            }
            break;
        case 0x1:
            cpu_.pc_set(instr & 0x0FFF);
            printf("%x:%x:JP\n", instruction[0], instruction[1]);
            break;
        case 0x2:
            cpu_.call(instr & 0x0FFF);
            printf("%x:%x:CALL\n", instruction[0], instruction[1]);
            break;
        case 0x3:
            printf("%x:%x:SE\n", instruction[0], instruction[1]);
            if (cpu_.registers_get(instruction[0] & 0x0F) == instruction[1])
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x4:
            printf("%x:%x:SNE\n", instruction[0], instruction[1]);
            if (cpu_.registers_get(instruction[0] & 0x0F) != instruction[1])
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x5:
            printf("%x:%x:SE\n", instruction[0], instruction[1]);
            if (cpu_.registers_get(instruction[0] & 0x0F) == cpu_.registers_get(instruction[1] >> 4))
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0x6:
            printf("%x:%x:LD\n", instruction[0], instruction[1]);
            cpu_.registers_set(instruction[0] & 0x0F, instruction[1]);
            break;
        case 0x7:
            printf("%x:%x:ADD\n", instruction[0], instruction[1]);
            cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[0] & 0x0F) + instruction[1]);
            break;
        case 0x8:
            if ((instruction[1] & 0x0F) == 0x0)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.registers_get(instruction[1] >> 4));
            }
            else if ((instruction[1] & 0x0F) == 0x1)
            {
                printf("%x:%x:OR\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, (instruction[0] & 0x0F) | ((instruction[1] >> 4) & 0x0F));
            }
            else if ((instruction[1] & 0x0F) == 0x2)
            {
                printf("%x:%x:AND\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, (instruction[0] & 0x0F) & ((instruction[1] >> 4) & 0x0F));
            }
            else if ((instruction[1] & 0x0F) == 0x3)
            {
                printf("%x:%x:XOR\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, (instruction[0] & 0x0F) ^ ((instruction[1] >> 4) & 0x0F));
            }
            else if ((instruction[1] & 0x0F) == 0x4)
            {
                printf("%x:%x:ADD\n", instruction[0], instruction[1]);
                cpu_.add(instr);
            }
            else if ((instruction[1] & 0x0F) == 0x5)
            {
                printf("%x:%x:SUB\n", instruction[0], instruction[1]);
                cpu_.sub(instr);
            }
            else if ((instruction[1] & 0x0F) == 0x6)
            {
                printf("%x:%x:SHR\n", instruction[0], instruction[1]);
                cpu_.shr(instr);
            }
            else if ((instruction[1] & 0x0F) == 0x7)
            {
                printf("%x:%x:SUBN\n", instruction[0], instruction[1]);
                cpu_.subn(instr);
            }
            else if ((instruction[1] & 0x0F) == 0xE)
            {
                printf("%x:%x:SHL\n", instruction[0], instruction[1]);
                cpu_.shl(instr);
            }
            break;
        case 0x9:
            printf("%x:%x:SNE\n", instruction[0], instruction[1]);
            if (cpu_.registers_get(instruction[0] & 0x0F) != cpu_.registers_get(instruction[1] >> 4))
                cpu_.pc_set(cpu_.pc_get() + 2);
            break;
        case 0xA:
            printf("%x:%x:LD\n", instruction[0], instruction[1]);
            cpu_.Ireg_set(instr & 0x0FFF);
            break;
        case 0xB:
            printf("%x:%x:JP\n", instruction[0], instruction[1]);
            cpu_.pc_set((instr & 0x0FFF) + cpu_.registers_get(0));
            break;
        case 0xC:
            printf("%x:%x:RND\n", instruction[0], instruction[1]);
            cpu_.randomize(instr);
            break;
        case 0xD:
            printf("%x:%x:DRW\n", instruction[0], instruction[1]);
            display_.draw_sprite(cpu_.registers_get(instruction[0] & 0x0F), cpu_.registers_get(instruction[1] >> 4), cpu_.Ireg_get(), instruction[1] & 0x0F, &cpu_);
            break;
        case 0xE:
            if (instruction[1] == 0x9E)
            {
                printf("%x:%x:SKP\n", instruction[0], instruction[1]);
                if (kb_.is_key_pressed(cpu_.registers_get(instruction[0] & 0x0F)))
                    cpu_.pc_set(cpu_.pc_get() + 2);
            }
            else if (instruction[1] == 0xA1)
            {
                printf("%x:%x:SKNP\n", instruction[0], instruction[1]);
                if (!kb_.is_key_pressed(cpu_.registers_get(instruction[0] & 0x0F)))
                    cpu_.pc_set(cpu_.pc_get() + 2);
            }
            break;
        case 0xF:
            if (instruction[1] == 0x07)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, cpu_.timer_get());
            }
            else if (instruction[1] == 0x0A)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.registers_set(instruction[0] & 0x0F, kb_.get_key_pressed());
            }
            else if (instruction[1] == 0x15)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.timer_set(cpu_.registers_get(instruction[0] & 0x0F));
            }
            else if (instruction[1] == 0x18)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.sound_set(cpu_.registers_get(instruction[0] & 0x0F));
            }
            else if (instruction[1] == 0x1E)
            {
                printf("%x:%x:ADD\n", instruction[0], instruction[1]);
                cpu_.Ireg_set(cpu_.Ireg_get() + cpu_.registers_get(instruction[0] & 0x0F));
            }
            else if (instruction[1] == 0x29)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.Ireg_set((instruction[0] & 0x0F) * 5);
            }
            else if (instruction[1] == 0x33)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.ldb(instr, ram_);
            }
            else if (instruction[1] == 0x55)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.ld55(instr, ram_);
            }
            else if (instruction[1] == 0x65)
            {
                printf("%x:%x:LD\n", instruction[0], instruction[1]);
                cpu_.ld65(instr, ram_);
            }
            break;
        default:
            printf("default case oO!\n");
            break;
    }
    printf("\n");
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
        fetch_decode_execute();
        SDL_Flip(screen_);
        while (SDL_GetTicks() - last < 16)
            continue;
        last = SDL_GetTicks();
    }
    SDL_Quit();
    return 0;
}
