# pragma once
# include<stdio.h>
# include<fstream>
# include   "pipe.h"


void readWrite(pipe &pipe){
    unsigned int addr,addr2;
    char s[1010];
    long long val,tmp;
    //freopen("xx.yo","r",stdin);
    FILE* fin;
    fin=fopen("xx.yo","r");
    if(fin==NULL) puts("Cannot open!!!");
    while(fgets(s,1000,fin)){
        puts(s);
        sscanf(s,"%x: %I64x",&addr,&val);
        //printf("addr:%u val:%I64d addr:%x val:%I64x\n",addr,val,addr,val);
        int len=0;tmp=val;
        while(tmp) {len++,tmp>>=8;}
        tmp=val;
        if(len==0) len++;
        //printf("len:%d %u\n",len,addr+len-1);
        for(addr2=addr+len-1;;addr2--){
            //printf("addr2:%ud addr:%ud\n",addr2,addr);
            pipe.Mem.writeByte(addr2,tmp&0xFF);tmp>>=8;
            if(addr2==addr) break;
        }
    }

    puts("Read Finished!");
    fclose(fin);
}
