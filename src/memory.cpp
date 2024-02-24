#include <emulator.hpp>

Memory::Memory()
{
    for (unsigned short int i = 0x0000; i < MEMORY_SIZE; i++)
    {
        mem[i] = 0x00;
    }
}

void Memory::set(unsigned short int address, unsigned char value)
{
    if (address >= MEMORY_SIZE)
        return;
    mem[address] = value;
}

unsigned char Memory::get(unsigned short int address)
{
    if (address >= MEMORY_SIZE)
        return 0x00;
    return mem[address];
}
