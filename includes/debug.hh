#ifndef DEBUG_HH
# define DEBUG_HH

# include <unistd.h>
# include <cstdio>
# include "CPU.hh"
# include "RAM.hh"
# include "disass.hh"

class Debug
{
    public:
        Debug();
        Debug(CPU* cpu, RAM* ram, int on);
        ~Debug();

    public:
        void stop();
        void print_status();

    private:
        void wait();
        void print_instr();

    private:
        CPU* cpu_;
        RAM* ram_;
        int on_;
        int i_;
};

#endif /* !DEBUG_HH */
