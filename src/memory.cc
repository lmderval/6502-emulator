#include <emulator.hh>
#include <iostream>

void Memory::dump(unsigned int begin, unsigned int end) const
{
    if (begin % 0x10 || (end + 1) % 0x10 || end > MEMORY_SIZE)
        return;

    std::cout << "------";
    for (unsigned char j = 0x00; j < 0x10; j++)
        std::cout << "\t0x" << std::hex << +j;

    std::cout << '\n';

    for (unsigned int i = begin; i < end; i += 0x10)
    {
        std::cout << "0x" << std::hex << +i;
        for (unsigned char j = 0; j < 0x10; j++)
            std::cout << "\t0x" << std::hex << +mem_[i + j];

        std::cout << '\n';
    }
}

void Memory::set(unsigned short int address, unsigned char value)
{
    if (address >= MEMORY_SIZE)
        return;

    mem_[address] = value;
}

unsigned char Memory::get(unsigned short int address) const
{
    if (address >= MEMORY_SIZE)
        return 0x00;

    return mem_[address];
}
