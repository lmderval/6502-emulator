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

unsigned char Processor::adc_imm(unsigned char imm)
{
    pc++;
    unsigned char oldA = a;
    unsigned short int res = (unsigned short int)a + (unsigned short int)imm;
    a = res & 0xff;
    flags = ~(~flags | FLAG_N | FLAG_V | FLAG_Z | FLAG_C);
    if (a > 0x7f)
        flags |= FLAG_N;
    if ((oldA < 0x80 && imm < 0x80 && a > 0x7f)
        || (oldA > 0x7f && imm > 0x7f && a < 0x80))
        flags |= FLAG_V;
    if (a == 0x00)
        flags |= FLAG_Z;
    if (res > 0xff)
        flags |= FLAG_C;
    return 0x02;
}

unsigned char Processor::bcc(unsigned char rel)
{
    if ((flags & FLAG_C) == FLAG_C)
        pc++;
    else
        pc = pc - (unsigned char)(~rel);
    return 0x02;
}

unsigned char Processor::cmp_imm(unsigned char imm)
{
    pc++;
    flags = ~(~flags | FLAG_N | FLAG_Z | FLAG_C);
    if (a > imm + 0x7f)
        flags |= FLAG_N;
    if (a == imm)
        flags |= FLAG_Z;
    if (a >= imm)
        flags |= FLAG_C;
    return 0x02;
}

unsigned char Processor::lda_abs(unsigned short int abs)
{
    pc += 0x0002;
    a = memory->get(abs);
    flags = ~(~flags | FLAG_N | FLAG_Z);
    if (a > 0x7f)
        flags |= FLAG_N;
    if (a == 0x00)
        flags |= FLAG_Z;
    return 0x03;
}

unsigned char Processor::lda_imm(unsigned char imm)
{
    a = imm;
    pc++;
    flags = ~(~flags | FLAG_N | FLAG_Z);
    if (a > 0x7f)
        flags |= FLAG_N;
    if (a == 0x00)
        flags |= FLAG_Z;
    return 0x02;
}

unsigned char Processor::nop()
{
    return 0x01;
}

unsigned char Processor::sta_abs(unsigned short int abs)
{
    pc += 0x0002;
    memory->set(abs, a);
    return 0x03;
}

unsigned char Processor::execute(unsigned char op)
{
    switch (op)
    {
    case 0x69:
        return adc_imm(memory->get(pc));

    case 0x90:
        return bcc(memory->get(pc));

    case 0xc9:
        return cmp_imm(memory->get(pc));

    case 0xa9:
        return lda_imm(memory->get(pc));

    case 0xad:
        return lda_abs((unsigned short int)memory->get(pc)
                       | (unsigned short int)memory->get(pc + 0x0001) << 0x08);

    case 0x8d:
        return sta_abs((unsigned short int)memory->get(pc)
                       | (unsigned short int)memory->get(pc + 0x0001) << 0x08);

    default:
        return nop();
    }
}

void Processor::load(Program *program)
{
    for (unsigned short int i = 0x0000;
         i < program->getLength() && i + 0x0600 < MEMORY_SIZE; i++)
    {
        memory->set(0x0600 + i, program->get(i));
    }
}
