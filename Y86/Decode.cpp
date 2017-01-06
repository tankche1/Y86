# include "pipe.h"
# include "instruction.h"


int pipe::Decode::d_srcA(){
    if(icode&(IRRMOVL|IRMMOVL|IOPL|IPUSHL)) return rA;
    if(icode&(IPOPL|IRET)) return RESP;
    return RNONE;
}

int pipe::Decode::d_srcB(){
    if(icode&(IOPL|IRMMOVL|IMRMOVL)) return rB;
    if(icode&(IPUSHL|IPOPL|ICALL|IRET)) return RESP;
    return RNONE;
}

int pipe::Decode::d_dstE(){
    if(icode&(IRRMOVL|IIRMOVL|IOPL)) return rB;
    if(icode&(IPUSHL|IPOPL|ICALL|IRET)) return RESP;
    return RNONE;
}

int pipe::Decode::d_dstM(){
    if(icode&(IMRMOVL|IPOPL)) return rA;
    return RNONE;
}

int pipe::Decode::d_rvalA(registerfile &Reg){
    return Reg.read(d_srcA());
}

int pipe::Decode::d_rvalB(registerfile &Reg){
    return Reg.read(d_srcB());
}

int pipe::Decode::d_valA(Execute &e,_Memory &m,WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem){
    if(icode&(ICALL|IJXX)) return valP;
    if(d_srcA()==e.e_dstE(cc)) return e.e_valE();
    if(d_srcA()==m.dstM) return m.m_valM(Mem);
    if(d_srcA()==m.dstE) return m.valE;
    if(d_srcA()==w.dstM) return w.valM;
    if(d_srcA()==w.dstE) return w.valE;
    return d_rvalA(Reg);
}


int pipe::Decode::d_valB(Execute &e,_Memory &m,WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem){
    if(d_srcB()==e.e_dstE(cc)) return e.e_valE();
    if(d_srcB()==m.dstM) return m.m_valM(Mem);
    if(d_srcB()==m.dstE) return m.valE;
    if(d_srcB()==w.dstM) return w.valM;
    if(d_srcB()==w.dstE) return w.valE;
    return d_rvalB(Reg);
}

pipe::Execute pipe::Decode::decode(pipe::Execute &e,pipe::_Memory &m,pipe::WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem){
    return Execute(stat,icode,ifun,valC,d_valA(e,m,w,Reg,cc,Mem),d_valB(e,m,w,Reg,cc,Mem),d_dstE(),d_dstM(),d_srcA(),d_srcB());
}
