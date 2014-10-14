#include "../includes/emulator.hh"
#include "../includes/disass.hh"
#include <ctime>

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc == 1 || argc > 2)
    {
        if (argc == 3 && (!(std::string(argv[1])).compare("-d") ||
                !(std::string(argv[1])).compare("--disass")))
        {
            Disass disass(argv[2], 0x219);
            disass.run();
        }
        else
            std::cout << "Invalid arguments: use chip8-emulator -h or --help." << std::endl;
    }
    else
    {
        if (!(std::string(argv[1])).compare("-h") ||
            !(std::string(argv[1])).compare("--help"))
            std::cout << "Usage : chip8-emulator [ROM]" << std::endl;
        else
        {
            Emulator em(argv[1]);
            return em.run();
        }
        return 0;
    }
    return 1;
}
