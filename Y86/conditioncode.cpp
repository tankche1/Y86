# include "conditioncode.h"

conditioncode::conditioncode(void) {cc=0;}

conditioncode::~conditioncode(void) {}

conditioncode::conditioncode(int ZF,int SF,int OF) {cc=(ZF<<2)|(SF<<1)|OF;}

void conditioncode::update(int v) {cc=v;}

void conditioncode::update(int ZF,int SF,int OF) {cc=(ZF<<2)|(SF<<1)|OF;}

bool conditioncode::isTrue(int fun){
    bool zf=(cc>>2)&1,sf=(cc>>1)&1,of=cc&1;
	if(fun==CCJE)	return zf;
	if(fun==CCJNE)	return !zf;
	if(fun==CCJL)	return sf^of;
	if(fun==CCJLE)	return (sf^of)|zf;
	if(fun==CCJG)	return (!(sf^of))&(!zf);
	if(fun==CCJGE)	return !(sf^of);
	return true;
}

int conditioncode::isZF(){
	return (cc>>2)&1;
}

int conditioncode::isSF(){
	return (cc>>1)&1;
}

int conditioncode::isOF(){
	return cc&1;
}


