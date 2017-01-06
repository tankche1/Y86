# include "pipe.h"
# include "instruction.h"

int pipe::Execute::aluA(){
    if(icode&(IRRMOVL|IOPL)) return valA;
    if(icode&(IIRMOVL|IRMMOVL|IMRMOVL)) return valC;
    if(icode&(ICALL|IPUSHL)) return -4;
    if(icode&(IRET|IPOPL)) return 4;
}

int pipe::Execute::aluB(){
    if(icode&(IRMMOVL|IMRMOVL|IOPL|ICALL|IPUSHL|IRET|IPOPL)) return valB;
    if(icode&(IRRMOVL|IIRMOVL)) return 0;
}

int pipe::Execute::alufun(){
    if(icode&(IOPL|ifun)) return ifun;
    return ALUADD;
}

bool pipe::Execute::set_cc(_Memory &m,WriteBack &w,memory &Mem){
    return icode==IOPL&&!(m.m_stat(Mem)&(SADR|SINS|SHLT))&&!(w.stat&(SADR|SINS|SHLT));
}

int pipe::Execute::e_valA(){
    return valA;
}

int pipe::Execute::e_Cnd(conditioncode &cc){
    return cc.isTrue(ifun);
}

int pipe::Execute::e_dstE(conditioncode &cc){
    if((icode&IRRMOVL)&&(!e_Cnd(cc))) return RNONE;
    return dstE;
}

int pipe::Execute::e_valE(){
    int fun=alufun();
    int a=aluA(),b=aluB();
    int e=0;
    if(fun==ALUADD)	e=a+b;
	if(fun==ALUSUB)	e=b-a;
	if(fun==ALUAND) e=a&b;
	if(fun==ALUXOR)	e=a^b;
	return e;
}

void pipe::Execute::do_set_cc(conditioncode &cc){
    int fun=alufun();
    int a=aluA(),b=aluB();
    int e=0;
    if(fun==ALUADD)	{e=a+b;cc.update(e==0,e<0,(a>0&&b>0&&e<0)||(a<0&&b<0&&e>0));}
	if(fun==ALUSUB)	{e=b-a;cc.update(e==0,e<0,(a>0&&b<0&&e>0)||(a<0&&b>0&&e<0));}
	if(fun==ALUAND) {e=a&b;cc.update(e==0,e<0,0);}
	if(fun==ALUXOR)	{e=a^b;cc.update(e==0,e<0,0);}
}


pipe::_Memory pipe::Execute::execute(_Memory &m,WriteBack &w,conditioncode &cc,memory &Mem){
    if(set_cc(m,w,Mem)) do_set_cc(cc);
    return _Memory(stat,icode,e_Cnd(cc),e_valE(),valA,e_dstE(cc),dstM);
}

