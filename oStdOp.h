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
        //Upstream op, fxn to be called, # columns in output
        oStdOp(Operation *, int * (*)(Operation *, int *, int *), int *);  
        int tSize();  
        void print(int *, int, const char *);        
    private:
        int * args;
        //int current;
        Operation * op;
        int * outTuple;
        int colCount;
        //Fxn sent in - Upstream op, output tuple, Optional args (# args, args)
        //Args sent in --> if not null, 1st item = # args, 2nd = col count of 
        //output tuple. If 2nd col should be upstream tuple size send in as -1
        int * (*nextFxn)(Operation *, int *, int *);
};


#endif