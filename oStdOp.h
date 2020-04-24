#include "iOperation.h"
#include <chrono> 


#ifndef O_MANY_TO_N_H
#define O_MANY_TO_N_H

using namespace std::chrono; 

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
        Operation ** getUpsOps();
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
        time_point<high_resolution_clock> oStart;
        time_point<high_resolution_clock> oEnd;
        duration<double, std::micro> oDuration;
};


#endif