#pragma once

#include <vector>

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
    void dump(unsigned int begin = 0x0000,
              unsigned int end = MEMORY_SIZE) const;
    void set(unsigned short int address, unsigned char value);
    unsigned char get(unsigned short int address) const;

private:
    unsigned char mem_[MEMORY_SIZE] = { 0x00 };
};

class Program
{
public:
    Program(const std::vector<unsigned char> data);
    unsigned short int getLength() const;
    unsigned char get(unsigned short int address) const;

private:
    const std::vector<unsigned char> data_;
};

class Processor // TODO: Add stack from 0x0100 to 0x01ff
{
public:
    Processor(Memory &memory);
    unsigned char next();
    void load(const Program &program);
    unsigned char getA() const;
    unsigned char getX() const;
    unsigned char getY() const;
    unsigned char getSP() const;
    unsigned short int getPC() const;
    unsigned char getFlags() const;

private:
    unsigned char clc(); // 0x18
    unsigned char adc_imm(unsigned char imm); // 0x69
    unsigned char bcc(unsigned char rel); // 0x90
    unsigned char cmp_imm(unsigned char imm); // 0xc9
    unsigned char lda_imm(unsigned char imm); // 0xa9
    unsigned char lda_abs(unsigned short int abs); // 0xad
    unsigned char nop(); // 0xea
    unsigned char sta_abs(unsigned short int abs); // 0x8d
    unsigned char execute(unsigned char op);

    unsigned char a_;
    unsigned char x_;
    unsigned char y_;
    unsigned char sp_;
    unsigned short int pc_;
    unsigned char flags_;
    Memory &memory_;
};
