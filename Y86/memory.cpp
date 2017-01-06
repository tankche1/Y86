# include "memory.h"

memory::memory(void) {error=0;}

memory::~memory(void) {}
int memory::readByte(unsigned int addr){
    if(!memo.count(addr)) return 0;
    return memo[addr];
}

int memory::read(unsigned int addr,int len){
    if((int)addr<0||addr+len-1<addr) {error=1;return 0;}
    if(len==1) return readByte(addr);
    //little-endian
    else if(len==4) return (readByte(addr+3)<<24)|(readByte(addr+2)<<16)|(readByte(addr+1)<<8)|readByte(addr);
}

int memory::writeByte(unsigned int addr,int val){
    memo[addr]=val;
    return 1;
}

int memory::write(unsigned int addr,int val){
    if((int)addr<0||addr+4-1<addr) {error=1;return 0;}
    writeByte(addr,val&0xFF);val>>=8;
    writeByte(addr,val&0xFF);val>>=8;
    writeByte(addr,val&0xFF);val>>=8;
    writeByte(addr,val&0xFF);val>>=8;

}
