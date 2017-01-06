# include "pipe.h"
# include "instruction.h"

int pipe::_Memory::mem_addr(){
    if(icode&(IRMMOVL|IPUSHL|ICALL|IMRMOVL)) return valE;
    if(icode&(IPOPL|IRET)) return valA;
}

bool pipe::_Memory::mem_read(){
    return icode&(IMRMOVL|IPOPL|IRET);
}

bool pipe::_Memory::mem_write(){
    return icode&(IRMMOVL|IPUSHL|ICALL);
}

int pipe::_Memory::m_stat(memory &Mem){
    if(Mem.error) return SADR;
    return stat;
}

int pipe::_Memory::m_valM(memory &Mem){
    if(mem_read()) return Mem.read(mem_addr(),4);
    if(mem_write()) return Mem.write(mem_addr(),valA);
    return 0;
}

pipe::WriteBack pipe::_Memory::_memory(memory &Mem){
    int tmp=m_valM(Mem);
    return WriteBack(m_stat(Mem),icode,valE,tmp,dstE,dstM);
}
