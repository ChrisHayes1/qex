#include "iOperation.h"

#ifndef O_MANY_TO_N_H
#define O_MANY_TO_N_H

class oStdOp: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        oStdOp(Operation *, int * (*)(Operation *, int *), int);  
        int tSize();  
        void print(int *, int, const char *);        
    private:
        int colCount;
        //int current;
        Operation * op;
        int * mOut;
        int * (*mNextFxn)(Operation *, int *);
};


#endif