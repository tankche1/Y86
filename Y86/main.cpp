# include <iostream>
# include <stdio.h>
# include<stdlib.h>
# include "pipe.h"
# include "read_file.h"
//# include "read_file.cpp"

using namespace std;

pipe P;


int main()
{

    readWrite(P);
    int x;
    while(1){
        if(!P.Go()) break;
    }
    return 0;
}
