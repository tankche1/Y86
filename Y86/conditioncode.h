#pragma once
# include "instruction.h"

class conditioncode{
private:
    int cc;
public:
    conditioncode(void);
    conditioncode(int ZF,int SF,int OF);
    ~conditioncode(void);
    void update(int v);
    void update(int ZF,int SF,int OF);
    bool isTrue(int ifun);
    int isZF();
    int isSF();
    int isOF();
};

