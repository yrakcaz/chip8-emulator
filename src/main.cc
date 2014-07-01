#include <iostream>

int main(int argc, char **argv)
{
    if (argc == 1 || argc > 2)
        std::cout << "Invalid arguments: use chip8-emulator -h or --help." << std::endl;
    else
    {
        if (!(std::string(argv[1])).compare("-h") ||
            !(std::string(argv[1])).compare("--help"))
            std::cout << "Usage : chip8-emulator [ROM]" << std::endl;
        //else
        //    Emulator em(argv[1]);
        //    em.run();
        return 0;
    }
    return 1;
}
