#include "../includes/RAM.hh"

RAM::RAM()
{
}

RAM::RAM(char* file)
{
    for (int i = 0; i < RAM_SIZE; i++)
        ram_[i] = 0;
    int pos = 0x200;
    std::string line;
    std::ifstream f(file);
    if (f.is_open())
    {
        while (getline(f, line))
        {
            for (size_t i = 0; i < line.size(); i++)
            {
                ram_[pos] = line[i];
                pos++;
            }
        }
        f.close();
    }
}

RAM::~RAM()
{
}

uint8_t RAM::ram_get(int i)
{
    return ram_[i];
}

void RAM::ram_set(int i, uint8_t val)
{
    ram_[i] = val;
}

void RAM::print()
{
    for (int i = 0; i < RAM_SIZE; i++)
    {
        printf("%x", (ram_[i] & 0xF0) >> 4);
        printf("%x ", ram_[i] & 0x0F);
        if (i % 32 == 0 && i)
        {
            std::cout << std::endl;
            continue;
        }
    }
}
