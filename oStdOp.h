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
        //Upstream op, fxn to be called, # columns in output, fxn args
        oStdOp(Operation *, int * (*)(Operation *, int *, int *), int, int *);  
        int tSize();  
        void print(int *, int, const char *);        
        void setPrint(bool);
        bool getPrint();
        Operation * getUpsOp(); //returns upstream operation
    private:
        int * args;
        //int current;
        Operation * op;
        int * outTuple;
        int colCount;
        bool showPrintout;
        //Fxn sent in - Upstream op, output tuple, Optional args (# args, args)
        //Args sent in --> if not null, 1st item = # args, followed by args
        int * (*nextFxn)(Operation *, int *, int *);
};


#endif