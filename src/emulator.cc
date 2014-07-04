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
    uint16_t instr = instruction[0];
    instr = (instr << 4) | instruction[1];
    cpu_.pc_set(cpu_.pc_get() + 2);
    switch (instruction[0] >> 4)
    {
        case 0x0:
            if (instruction[1] == 0xE0)
                display_.clear();
            else if (instruction[1] == 0xEE)
                cpu_.ret();
            break;
        case 0x1:
            cpu_.pc_set(instr);
            break;
        case 0x2:
            cpu_.call(instr & 0x0FFF);
            break;
        case 0x3:
            //se(vx, byte)
            break;
        case 0x4:
            //sne(vx, byte)
            break;
        case 0x5:
            //se(vx, vy)
            break;
        case 0x6:
            //ld(vx, byte)
            break;
        case 0x7:
            //add (vx, byte)
            break;
        case 0x8:
            if ((instruction[1] & 0x0F) == 0x0)
                //ld(vx, vy)
            else if ((instruction[1] & 0x0F) == 0x1)
                //or
            else if ((instruction[1] & 0x0F) == 0x2)
                //and
            else if ((instruction[1] & 0x0F) == 0x3)
                //xor
            else if ((instruction[1] & 0x0F) == 0x4)
                //add(vx, vy)
            else if ((instruction[1] & 0x0F) == 0x5)
                //sub
            else if ((instruction[1] & 0x0F) == 0x6)
                //shr
            else if ((instruction[1] & 0x0F) == 0x7)
                //subn
            else if ((instruction[1] & 0x0F) == 0xE)
                //shl
            break;
        case 0x9:
            //sne(vx, vy)
            break;
        case 0xA:
            //ld(I, addr)
            break;
        case 0xB:
            //jp(v0, addr)
            break;
        case 0xC:
            //rnd
            break;
        case 0xD:
            //drw
            break;
        case 0xE:
            if (instruction[1] == 0x9E)
                //skp
            else if (instruction[1] == 0xA1)
                //sknp
            break;
        case 0xF:
            if (instruction[1] == 0x07)
                //ld(vx, dt)
            else if (instruction[1] == 0x0A)
                //ld(vx, k)
            else if (instruction[1] == 0x15)
                //ld(dt, vx)
            else if (instruction[1] == 0x18)
                //ld(st, vx)
            else if (instruction[1] == 0x1E)
                //add(I, vx)
            else if (instruction[1] == 0x29)
                //ld(f, vx)
            else if (instruction[1] == 0x33)
                //ld(b, vx)
            else if (instruction[1] == 0x55)
                //ld([i], vx)
            else if (instruction[1] == 0x65)
                //ld(vx, [i])
            break;
        std::cout << "There is an error into your rom, it could not works!" << std::endl;
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

        // Fetch, Decode, Execute here!
        
        SDL_Flip(screen_);
        while (SDL_GetTicks() - last < 16)
            continue;
        last = SDL_GetTicks();
    }
    SDL_Quit();
    return 0;
}
