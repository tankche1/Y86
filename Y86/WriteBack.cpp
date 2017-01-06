# include "pipe.h"
# include "instruction.h"
# include<stdio.h>

void pipe::WriteBack::writeback(registerfile &Reg,int &Stat){
    Reg.write(dstE,valE);
    Reg.write(dstM,valM);
    if(Reg.error) Stat=SADR;
    else Stat=stat;

    if(icode==IHALT) {Stat=SHLT;}
}
