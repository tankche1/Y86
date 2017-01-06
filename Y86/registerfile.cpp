#pragma once
# include "instruction.h"
# include "registerfile.h"
# include<string.h>

registerfile::registerfile(void){
    memset(reg,0,sizeof(reg));
}

registerfile::~registerfile(void)
{
}

int registerfile::read(int addr){
    if(addr<0||addr>=8){
        if(addr==RNONE) error=0;
        else error=1;
        return 0;
    }
    return reg[addr];
}

int registerfile::write(int addr,int val){
    if(addr<0||addr>=8){
        if(addr==RNONE) error=0;
        else error=1;
        return 0;
    }
    reg[addr]=val;
    return 1;
}
