#pragma once
# include "instruction.h"

class registerfile
{
private:
    int reg[8];
public:
    int error;
    registerfile(void);
    ~registerfile(void);
    int read(int addr);
    int write(int addr,int val) ;
};
