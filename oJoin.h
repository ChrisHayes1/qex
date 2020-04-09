#include "iOperation.h"
#include <fstream>
#include <iostream>

using namespace std;

#ifndef O_JOIN_H
#define O_JOIN_H

class oJoin: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        //Upstream op, fxn to be called, # columns in output, fxn args
        //fxnArgs - #args, #operators, col count, additional args
        oJoin(Operation **, int * (*)(Operation *, int *, int *), int, int *);  
        int tSize();  
        void print(int *, int, const char *);        
        void setPrint(bool);
        bool getPrint();
        Operation * getUpsOp();
        Operation ** getUpsOps(); //returns upstream operation
    private:
        int * args;
        //int current;
        Operation ** ops;
        int * outTuple;
        int colCount;
        bool showPrintout;
        //Fxn sent in - Upstream op, output tuple, Optional args (# args, args)
        //Args sent in --> if not null, 1st item = # args, followed by args
        int * (*nextFxn)(Operation *, int *, int *);
};

#endif