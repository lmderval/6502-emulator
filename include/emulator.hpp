#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <stdlib.h>

#define MEMORY_SIZE 0xffff

#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_V 0x40
#define FLAG_N 0x80

class Memory
{
public:
    Memory();
    void set(unsigned short int address, unsigned char value);
    unsigned char get(unsigned short int address);

private:
    unsigned char mem[MEMORY_SIZE];
};

class Program
{
public:
    Program(const unsigned char data[], unsigned short int length);
    unsigned short int getLength();
    unsigned char get(unsigned short int address);

private:
    unsigned short int length;
    unsigned char *data;
};

class Processor // TODO: Add stack from 0x0100 to 0x01ff
{
public:
    Processor(Memory *memory);
    unsigned char next();
    void load(Program *program);
    unsigned char getA();
    unsigned char getX();
    unsigned char getY();
    unsigned char getSP();
    unsigned short int getPC();
    unsigned char getFlags();

private:
    unsigned char execute(unsigned char op);
    unsigned char a;
    unsigned char x;
    unsigned char y;
    unsigned char sp;
    unsigned short int pc;
    unsigned char flags;
    Memory *memory;
};

#endif // defined
