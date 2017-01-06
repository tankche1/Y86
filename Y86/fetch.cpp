# include "pipe.h"
# include "instruction.h"
# include<stdio.h>

unsigned int pipe::Fetch::f_pc(pipe::_Memory &m,pipe::WriteBack &w){
    if(m.icode==IJXX&&!m.Cnd) return m.valA;
    if(w.icode==IRET) return w.valM;
    return predPC;
}

int pipe::Fetch::f_icode(int icode,memory &Mem){
    if(Mem.error) return INOP;
    return icode;
}

int pipe::Fetch::f_ifun(int ifun,memory &Mem){
    if(Mem.error) return RNONE;
    return ifun;
}

bool pipe::Fetch::Instr_valid(int icode){
    return icode&(INOP|IHALT|IRRMOVL|IIRMOVL|IRMMOVL|IMRMOVL|IOPL|IJXX|ICALL|IRET|IPUSHL|IPOPL);
}

bool pipe::Fetch::Need_regids(int icode){
    return icode&(IRRMOVL|IOPL|IPUSHL|IPOPL|IIRMOVL|IRMMOVL|IMRMOVL);
}

bool pipe::Fetch::Need_valC(int icode){
    return icode&(IIRMOVL|IRMMOVL|IMRMOVL|IJXX|ICALL);
}

int pipe::Fetch::f_stat(int icode,memory &Mem){
    if(Mem.error) return SADR;
    if(!Instr_valid(icode)) return SINS;
    if(icode==IHALT) return SHLT;
    return SAOK;
}

unsigned int pipe::Fetch::f_predPC(int icode,unsigned int valC,unsigned int valP){
	if(icode&(IJXX|ICALL))	return valC;
	return valP;
}

std::pair<pipe::Fetch,pipe::Decode> pipe::Fetch::fetch(pipe::_Memory &m,pipe::WriteBack &w,memory &Mem){
    unsigned int pc=f_pc(m,w);
    int tmp=Mem.read(pc,1);
    //printf("pc:%u tmp:%d\n",pc,tmp);
    int ifun=tmp&(0xF);tmp>>=4;int icode=tmp&0xF;icode=1<<icode;
    pc++;

    icode=f_icode(icode,Mem);ifun=f_ifun(ifun,Mem);
    //printf("!!!!icode:%d!!!\n",icode);
    //bool instr_valid=f_instr_valid(icode);
    bool needRigids=Need_regids(icode);
    bool needValC=Need_valC(icode);

    //printf("!!!needRigids:%d!!! icode:%d\n",needRigids,icode);

    int rB=RNONE,rA=RNONE;
    if(needRigids){
        tmp=Mem.read(pc,1);
        rB=tmp&0xF;tmp>>=4;
        rA=tmp&0xF;
        pc++;
    }

    int valC=0;
    if(needValC){
        tmp=Mem.read(pc,4);
        valC=tmp;
        pc+=4;
    }

    int stat=f_stat(icode,Mem);
    return std::pair<pipe::Fetch,pipe::Decode>(Fetch(f_predPC(icode,valC,pc)),Decode(stat,icode,ifun,rA,rB,valC,pc));
}


