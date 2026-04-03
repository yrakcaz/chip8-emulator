#include <ctime>

#include <disass.hh>
#include <emulator.hh>
#include <interp.hh>

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc == 1 || argc > 2)
    {
        if (argc == 3 && (!(std::string(argv[1])).compare("-d") ||
                !(std::string(argv[1])).compare("--disass")))
        {
            Disass disass(argv[2], 0x200);
            return disass.run();
        }
        else if (argc == 3 && (!(std::string(argv[1])).compare("-g") ||
                !(std::string(argv[1])).compare("--debug")))
        {
            Emulator em(argv[2], 1);
            return em.run();
        }
        else if (argc == 3 && (!(std::string(argv[1])).compare("-i") ||
                !(std::string(argv[1])).compare("--interpret")))
        {
            Interp interpreter(argv[2]);
            return interpreter.interpret();
        }
        else
        {
            std::cout << "Invalid arguments: use chip8-emulator -h or --help." << std::endl;
            return 1;
        }
    }
    else
    {
        if (!(std::string(argv[1])).compare("-h") ||
            !(std::string(argv[1])).compare("--help"))
        {
            std::cout << "Usage : chip8-emulator [options] [file]" << std::endl;
            std::cout << "\tOptions :\t-d, --disass\tDisassemble ROM" << std::endl;
            std::cout << "\t\t\t-g, --debug\tExecution step by step" << std::endl;
            std::cout << "\t\t\t-i, --interpret\tInterpret assembly file" << std::endl;
            std::cout << "\t\t\t-h, --help\tShow this help" << std::endl;
            return 0;
        }
        else
        {
            Emulator em(argv[1], 0);
            return em.run();
        }
    }
}
