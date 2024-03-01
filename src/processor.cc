#include <emulator.hh>

Processor::Processor(Memory &memory)
    : a_(0x00)
    , x_(0x00)
    , y_(0x00)
    , sp_(0xff)
    , pc_(0x0600)
    , flags_(0x20 | FLAG_B)
    , memory_(memory)
{}

unsigned char Processor::getA() const
{
    return a_;
}

unsigned char Processor::getX() const
{
    return x_;
}

unsigned char Processor::getY() const
{
    return y_;
}

unsigned char Processor::getSP() const
{
    return sp_;
}

unsigned short int Processor::getPC() const
{
    return pc_;
}

unsigned char Processor::getFlags() const
{
    return flags_;
}

unsigned char Processor::next()
{
    unsigned char op = memory_.get(pc_);
    pc_++;
    return execute(op);
}

unsigned char Processor::adc_imm(unsigned char imm)
{
    pc_++;
    unsigned char oldA = a_;
    unsigned short int res = static_cast<unsigned short int>(a_)
        + static_cast<unsigned short int>(imm);
    a_ = res & 0xff;
    flags_ = ~(~flags_ | FLAG_N | FLAG_V | FLAG_Z | FLAG_C);
    if (a_ > 0x7f)
        flags_ |= FLAG_N;
    if ((oldA < 0x80 && imm < 0x80 && a_ > 0x7f)
        || (oldA > 0x7f && imm > 0x7f && a_ < 0x80))
        flags_ |= FLAG_V;
    if (a_ == 0x00)
        flags_ |= FLAG_Z;
    if (res > 0xff)
        flags_ |= FLAG_C;
    return 0x02;
}

unsigned char Processor::bcc(unsigned char rel)
{
    if ((flags_ & FLAG_C) == FLAG_C)
        pc_++;
    else
        pc_ = pc_ - static_cast<unsigned char>(~rel);
    return 0x02;
}

unsigned char Processor::cmp_imm(unsigned char imm)
{
    pc_++;
    flags_ = ~(~flags_ | FLAG_N | FLAG_Z | FLAG_C);
    if (a_ > imm + 0x7f)
        flags_ |= FLAG_N;
    if (a_ == imm)
        flags_ |= FLAG_Z;
    if (a_ >= imm)
        flags_ |= FLAG_C;
    return 0x02;
}

unsigned char Processor::lda_abs(unsigned short int abs)
{
    pc_ += 0x0002;
    a_ = memory_.get(abs);
    flags_ = ~(~flags_ | FLAG_N | FLAG_Z);
    if (a_ > 0x7f)
        flags_ |= FLAG_N;
    if (a_ == 0x00)
        flags_ |= FLAG_Z;
    return 0x03;
}

unsigned char Processor::lda_imm(unsigned char imm)
{
    a_ = imm;
    pc_++;
    flags_ = ~(~flags_ | FLAG_N | FLAG_Z);
    if (a_ > 0x7f)
        flags_ |= FLAG_N;
    if (a_ == 0x00)
        flags_ |= FLAG_Z;
    return 0x02;
}

unsigned char Processor::nop()
{
    return 0x01;
}

unsigned char Processor::sta_abs(unsigned short int abs)
{
    pc_ += 0x0002;
    memory_.set(abs, a_);
    return 0x03;
}

unsigned char Processor::execute(unsigned char op)
{
    switch (op)
    {
    case 0x69:
        return adc_imm(memory_.get(pc_));

    case 0x90:
        return bcc(memory_.get(pc_));

    case 0xc9:
        return cmp_imm(memory_.get(pc_));

    case 0xa9:
        return lda_imm(memory_.get(pc_));

    case 0xad:
        return lda_abs(static_cast<unsigned short int>(memory_.get(pc_))
                       | static_cast<unsigned short int>(
                           memory_.get(pc_ + 0x0001) << 0x08));

    case 0x8d:
        return sta_abs(static_cast<unsigned short int>(memory_.get(pc_))
                       | static_cast<unsigned short int>(
                           memory_.get(pc_ + 0x0001) << 0x08));

    default:
        return nop();
    }
}

void Processor::load(const Program &program)
{
    for (unsigned short int i = 0x0000;
         i < program.getLength() && i + 0x0600 < MEMORY_SIZE; i++)
    {
        memory_.set(0x0600 + i, program.get(i));
    }
}
