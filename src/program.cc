#include <emulator.hh>

Program::Program(const std::vector<unsigned char> data)
    : data_(data)
{}

unsigned char Program::get(unsigned short int address) const
{
    if (address >= data_.size())
        return 0x00;

    return data_[address];
}

unsigned short int Program::getLength() const
{
    return data_.size();
}
