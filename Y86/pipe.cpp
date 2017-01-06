# include "pipe.h"
# include<stdio.h>
# include "instruction.h"

pipe::pipe(void) {printf("pipe build!\n");}

pipe::~pipe(void) {printf("pipe deleted!\n");}

bool pipe::F_bubble(){return 0;}

bool pipe::F_stall(){
    if(((e.icode&(IMRMOVL|IPOPL))&&(e.dstM==d.d_srcA()||e.dstM==d.d_srcB()))||(d.icode==IRET||e.icode==IRET||m.icode==IRET)) return true;
    return false;
}

bool pipe::D_stall(){
    if((e.icode&(IMRMOVL|IPOPL))&&(e.dstM==d.d_srcA()||e.dstM==d.d_srcB())) return true;
    return false;
}

bool pipe::D_bubble(){
    if((e.icode==IJXX&&!e.e_Cnd(cc))||(!((e.icode&(IMRMOVL|IPOPL))&&e.dstM&(d.d_srcA()|d.d_srcB()))&&(d.icode==IRET||e.icode==IRET||m.icode==IRET)))
        return true;
    return false;
}

bool pipe::E_stall() {return 0;}

bool pipe::E_bubble(){
    if((e.icode==IJXX&&!e.e_Cnd(cc))||((e.icode==IMRMOVL||e.icode==IPOPL)&&(e.dstM==d.d_srcA()||e.dstM==d.d_srcB()))) return true;
    return false;
}

bool pipe::M_stall() {return 0;}

bool pipe::M_bubble(){
    if((m.m_stat(Mem)!=SAOK)||(w.stat!=SAOK)) return true;
    return false;
}

bool pipe::W_stall(){
     if(w.stat!=SAOK) return true;
     return false;
}

bool pipe::W_bubble() {return 0;}

int mem_addr();
        bool mem_read();
        bool mem_write();
        int m_stat(memory &Mem);
        int m_valM(memory &Mem);

void pipe::print(){
    printf("Fetch:\npredPC:%u pc:%u \n",f.predPC,f.f_pc(m,w));
    printf("Decode:\nstat:%d icode:%d ifun:%d rA:%d rB:%d valC:%d valP:%d\n"
           ,d.stat,d.icode,d.ifun,d.rA,d.rB,d.valC,d.valP);
    //printf("srcA:%d srcB:%d dstE:%d dstM:%d rvalA:%d rvalB:%d valA:%d valB:%d\n\n",
     //      d.d_srcA(),d.d_srcB(),d.d_dstE(),d.d_dstM(),d.d_rvalA(Reg),d.d_rvalB(Reg),d.d_valA(e,m,w,Reg,cc,Mem),d.d_valB(e,m,w,Reg,cc,Mem));
    printf("Execute:\nstat:%d icode:%d ifun:%d valC:%d valA:%d valB:%d dstE:%d dstM:%d srcA:%d srcB:%d \n",
           e.stat,e.icode,e.ifun,e.valC,e.valA,e.valB,e.dstE,e.dstM,e.srcA,e.srcB);
    //printf("aluA:%d aluB:%d alufun:%d e_valA:%d e_dstE:%d e_Cnd:%d \n\n",e.aluA(),e.aluB(),e.alufun(),e.e_valA(),e.e_dstE(cc),e.e_Cnd(cc));
    printf("Memory:stat:%d icode:%d Cnd:%d valE:%d valA:%d dstE:%d dstM:%d \n",m.stat,m.icode,m.Cnd,m.valE,m.valA,m.dstE,m.dstM);
    //printf("");
    printf("WriteBack:stat:%d icode:%d valE:%d valM:%d dstE:%d dstM:%d \n",w.stat,w.icode,w.valE,w.valM,w.dstE,w.dstM);

    puts("");
    puts("");
}

bool pipe::Go(){
    Fetch nf;
    Decode nd;
    Execute ne;
    _Memory nm;
    WriteBack nw;
    std::pair<pipe::Fetch,pipe::Decode> tmp;

    if(F_stall()) nf=f;
    else if(F_bubble());
    else{

        tmp=f.fetch(m,w,Mem);
        nf=tmp.first;
    }

    if(D_stall()) nd=d;
    else if(D_bubble()){
        nd=Decode();
    }
    else nd=tmp.second;

    if(E_stall()) ne=e;
    else if(E_bubble()){
        ne=Execute();
    }
    else{
        ne=d.decode(e,m,w,Reg,cc,Mem);
    }

    if(M_stall()) nm=m;
    else if(M_bubble()){
        nm=_Memory();
    }
    else
        nm=e.execute(m,w,cc,Mem);

    if(W_stall()) nw=w;
    else if(W_bubble()) nw=WriteBack();
    else nw=m._memory(Mem);

    w.writeback(Reg,Stat);

    printf("f.pc:%d ",f.f_pc(m,w));printf("nf.pc:%d \n",nf.f_pc(nm,nw));

    print();

    if(Stat!=SAOK){
        if(Stat==2) puts("SADR!");
        else if(Stat==4) puts("SINS");
        else puts("SHLT");
        return false;
    }

    f=nf;
    d=nd;
    e=ne;
    m=nm;
    w=nw;

    return true;
}
