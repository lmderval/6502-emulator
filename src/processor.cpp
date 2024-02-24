#include <emulator.hpp>

Processor::Processor(Memory *memory)
{
    a = 0x00;
    x = 0x00;
    y = 0x00;
    sp = 0xff;
    pc = 0x0600;
    flags = 0x20 | FLAG_B;
    this->memory = memory;
}

unsigned char Processor::getA()
{
    return a;
}

unsigned char Processor::getX()
{
    return x;
}

unsigned char Processor::getY()
{
    return y;
}

unsigned char Processor::getSP()
{
    return sp;
}

unsigned short int Processor::getPC()
{
    return pc;
}

unsigned char Processor::getFlags()
{
    return flags;
}

unsigned char Processor::next()
{
    unsigned char op = memory->get(pc);
    pc++;
    return execute(op);
}

unsigned char Processor::execute(unsigned char op)
{
    unsigned short int address;
    unsigned short int res;
    unsigned char oldA;
    unsigned char summand;
    unsigned char length;
    switch (op)
    {
    case 0x69: // ADC Immediate
        length = 0x02;
        oldA = a;
        summand = memory->get(pc);
        pc++;
        res = (unsigned short int)a + (unsigned short int)summand;
        a = res & 0xff;
        flags = ~(~flags | FLAG_N | FLAG_V | FLAG_Z | FLAG_C);
        if (a > 0x7f)
            flags |= FLAG_N;
        if (oldA < 0x80 && summand < 0x80 && a > 0x7f
            || oldA > 0x7f && summand > 0x7f && a < 0x80)
            flags |= FLAG_V;
        if (a == 0x00)
            flags |= FLAG_Z;
        if (res > 0xff)
            flags |= FLAG_C;
        break;

    case 0x90: // BCC
        length = 0x02;
        summand = ~memory->get(pc);
        if ((flags & FLAG_C) == FLAG_C)
            pc++;
        else
            pc = pc - (unsigned short int)summand;
        break;

    case 0xc9: // CMP Immediate
        length = 0x02;
        summand = memory->get(pc);
        pc++;
        flags = ~(~flags | FLAG_N | FLAG_Z | FLAG_C);
        if (a > summand + 0x7f)
            flags |= FLAG_N;
        if (a == summand)
            flags |= FLAG_Z;
        if (a >= summand)
            flags |= FLAG_C;
        break;

    case 0xa9: // LDA Immediate
        length = 0x02;
        a = memory->get(pc);
        pc++;
        flags = ~(~flags | FLAG_N | FLAG_Z);
        if (a > 0x7f)
            flags |= FLAG_N;
        if (a == 0x00)
            flags |= FLAG_Z;
        break;

    case 0x8d: // STA Absolute
        length = 0x03;
        address = memory->get(pc) | memory->get(pc + 0x0001) << 0x08;
        pc += 0x0002;
        memory->set(address, a);
        break;

    default: // NOP
        length = 1;
        break;
    }

    return length;
}

void Processor::load(Program *program)
{
    for (unsigned short int i = 0x0000;
         i < program->getLength() && i + 0x0600 < MEMORY_SIZE; i++)
    {
        memory->set(0x0600 + i, program->get(i));
    }
}
