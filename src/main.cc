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
            Disass disass(argv[2], 0x200);
            disass.run();
        }
        else if (argc == 3 && (!(std::string(argv[1])).compare("-dbg") ||
                !(std::string(argv[1])).compare("--debug")))
        {
            Emulator em(argv[2], 1);
            em.run();
        }
        else
            std::cout << "Invalid arguments: use chip8-emulator -h or --help." << std::endl;
    }
    else
    {
        if (!(std::string(argv[1])).compare("-h") ||
            !(std::string(argv[1])).compare("--help"))
        {
            std::cout << "Usage : chip8-emulator [options] [file]" << std::endl;
            std::cout << "\tOptions :\t-d, --disass\tDisassemble ROM" << std::endl;
            std::cout << "\t\t\t-dbg, --debug\tExecution step by step" << std::endl;
        }
        else
        {
            Emulator em(argv[1], 0);
            return em.run();
        }
        return 0;
    }
    return 1;
}
