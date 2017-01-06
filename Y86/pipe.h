#pragma once
# include "memory.h"
# include "registerfile.h"
# include "conditioncode.h"
# include "instruction.h"

class pipe
{
public:
    pipe(void);
    ~pipe(void);
    memory Mem;
    registerfile Reg;
    conditioncode cc;
    int Stat;

    typedef struct WriteBack{
        int stat,icode,valE,valM,dstE,dstM;
        WriteBack(int stat=SAOK,int icode=INOP,int valE=0,int valM=0,int dstE=RNONE,int dstM=RNONE):
            stat(stat),icode(icode),valE(valE),valM(valM),dstE(dstE),dstM(dstM) {}
        void writeback(registerfile &Reg,int &Stat);
    }WriteBack;

    typedef struct _Memory{
        int stat,icode,Cnd,valE,valA,dstE,dstM;
        _Memory(int stat=SAOK,int icode=INOP,int Cnd=true,int valE=0,int valA=0,int dstE=RNONE,int dstM=RNONE):
            stat(stat),icode(icode),Cnd(Cnd),valE(valE),valA(valA),dstE(dstE),dstM(dstM) {}
        int mem_addr();
        bool mem_read();
        bool mem_write();
        int m_stat(memory &Mem);
        int m_valM(memory &Mem);
        WriteBack _memory(memory &Mem);

    }_Memory;

    typedef struct Execute{
        int stat,icode,ifun,valC,valA,valB,dstE,dstM,srcA,srcB;
        Execute(int stat=SAOK,int icode=INOP,int ifun=FNONE,int valC=0,int valA=0,int valB=0,int dstE=RNONE,int dstM=RNONE,int srcA=FNONE,int srcB=FNONE):
            stat(stat),icode(icode),ifun(ifun),valC(valC),valA(valA),valB(valB),dstE(dstE),dstM(dstM),srcA(srcA),srcB(srcB) {}
        int aluA() ;
        int aluB() ;
        int alufun() ;
        bool set_cc(_Memory &m,WriteBack &w,memory &Mem) ;
        int e_valA();
        int e_dstE(conditioncode &cc);
        int e_Cnd(conditioncode &cc);
        int e_valE() ;
        void do_set_cc(conditioncode &cc);
        _Memory execute(_Memory &m,WriteBack &w,conditioncode &cc,memory &Mem);

    }Execute;

    typedef struct Decode{
        int stat,icode,ifun,rA,rB,valC,valP;
        Decode(int stat=SAOK,int icode=INOP,int ifun=FNONE,int rA=RNONE,int rB=RNONE,int valC=0,int valP=0):
            stat(stat),icode(icode),ifun(ifun),rA(rA),rB(rB),valC(valC),valP(valP){}
        Execute decode(Execute &e,_Memory &m,WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem);
        int d_srcA();
        int d_srcB();
        int d_dstE();
        int d_dstM();
        int d_rvalA(registerfile &Reg);
        int d_rvalB(registerfile &Reg);
        int d_valA(Execute &e,_Memory &m,WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem);
        int d_valB(Execute &e,_Memory &m,WriteBack &w,registerfile &Reg,conditioncode &cc,memory &Mem);

    }Decode;


    typedef struct Fetch{
        unsigned int predPC;
        Fetch(unsigned int predPC=0):predPC(predPC) {}
        std::pair<Fetch,Decode> fetch(_Memory &m,WriteBack &w,memory &Mem) ;
        unsigned int f_pc(_Memory &m,WriteBack &w) ;
        int f_ifun(int ifun,memory &Mem);
        int f_icode(int icode,memory &Mem);
        bool Instr_valid(int icode);
        bool Need_regids(int icode);
        bool Need_valC(int icode);
        int f_stat(int icode,memory &Mem);
        unsigned int f_predPC(int icode,unsigned int valC,unsigned int valP);

        bool f_instr_valid();

    }Fetch;

    Fetch f;
    Decode d;
    Execute e;
    _Memory m;
    WriteBack w;

    bool Go();
    bool F_bubble();
    bool F_stall();
    bool D_stall();
    bool D_bubble();
    bool E_stall() ;
    bool E_bubble();
    bool M_stall();
    bool M_bubble();
    bool W_stall();
    bool W_bubble();
    void print();

};
