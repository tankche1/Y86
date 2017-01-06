#pragma once
# include "instruction.h"
# include <map>
class memory
{
private:
    std::map<int,int> memo;

public:
    int error;
    memory(void);
    ~memory(void);
    int readByte(unsigned int addr);
    int read(unsigned int addr,int len=1);
    int writeByte(unsigned int addr,int val);
    int write(unsigned int addr,int val);
};
