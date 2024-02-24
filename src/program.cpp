#include <emulator.hpp>

Program::Program(const unsigned char data[], unsigned short int length)
{
    this->length = length;
    this->data = (unsigned char *) malloc(length * sizeof(unsigned char));
    for (unsigned short int i = 0x0000; i < length; i ++)
    {
        this->data[i] = data[i];
    }
}

unsigned char Program::get(unsigned short int address)
{
    if (address >= length) return 0x00;
    return data[address];
}

unsigned short int Program::getLength()
{
    return length;
}
