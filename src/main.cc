#include <bitset>
#include <emulator.hh>
#include <iostream>

#define LEN(ARR, TYPE) (sizeof(ARR) / sizeof(TYPE))

int main()
{
    Memory memory;
    Processor proc(memory);
    // clang-format off
    const unsigned char data[] = {
        0xa9, 0xff,             // lda #$ff
        0x8d, 0x00, 0x80,       // sta $8000
        0xa9, 0x00,             // lda #$00
        0x69, 0x01,             // adc #$01
        0x8d, 0x01, 0x80,       // sta $8001
        0xc9, 0x08,             // cmp #$08
        0x90, 0xf7,             // bcc $f7
        0xad, 0x00, 0x80        // lda $8000
    };
    // clang-format on

    size_t length = LEN(data, unsigned char);
    std::vector<unsigned char> data_vec;
    for (size_t i = 0; i < length; i++)
        data_vec.push_back(data[i]);

    Program prog(data_vec);
    proc.load(prog);

    std::cout << "Program:\n";
    memory.dump(0x0600, 0x0600 + (prog.getLength() / 0x10) * 0x10 + 0x000f);

    std::cout << "Execution:\n";
    while (proc.getPC() < prog.getLength() + 0x0600)
    {
        unsigned short int oldPC = proc.getPC();
        unsigned char length = proc.next();
        for (unsigned short int i = 0x0000; i < 0x0004; i++)
        {
            if (i < length)
                std::cout << std::hex << "0x" << +memory.get(oldPC + i) << "\t";
            else
                std::cout << "\t";
        }
        std::cout << std::hex << "->\tA:0x" << +proc.getA() << "\tX:0x"
                  << +proc.getX() << "\tY:0x" << +proc.getY() << "\tPC:0x"
                  << proc.getPC() << "\tNV-BDIZC:0b"
                  << std::bitset<8>(proc.getFlags()) << '\n';
    }

    std::cout << "Memory:\n";
    memory.dump(0x8000, 0x800f);

    return 0;
}
