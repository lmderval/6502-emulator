#include <bitset>
#include <emulator.hpp>
#include <iostream>

int main(void)
{
    Memory memory;
    Processor proc(&memory);
    const unsigned char data[] = { 0xa9, 0x00, 0x69, 0x01, 0x8d, 0x00,
                                   0x80, 0xc9, 0x08, 0x90, 0xf7 };
    Program prog(data, 0x000b);
    proc.load(&prog);

    std::cout << "Program:" << std::endl;
    for (unsigned short int i = 0;
         i < prog.getLength() && i + 0x0600 < MEMORY_SIZE; i++)
    {
        std::cout << std::hex << "0x" << (int)memory.get(i + 0x0600) << "\t";
        if (i % 0x10 == 0x0f)
            std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Execution:" << std::endl;
    while (proc.getPC() < prog.getLength() + 0x0600)
    {
        unsigned short int oldPC = proc.getPC();
        unsigned char length = proc.next();
        for (unsigned short int i = 0; i < 4; i++)
        {
            if (i < length)
                std::cout << std::hex << "0x" << (int)memory.get(oldPC + i)
                          << "\t";
            else
                std::cout << "\t";
        }
        std::cout << std::hex << "->\tA:0x" << (int)proc.getA() << "\tX:0x"
                  << (int)proc.getX() << "\tY:0x" << (int)proc.getY()
                  << "\tPC:0x" << (int)proc.getPC() << "\tNV-BDIZC:0b"
                  << std::bitset<8>(proc.getFlags()) << std::endl;
    }

    prog.freeData();

    return 0;
}
