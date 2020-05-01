#include "iOperation.h"
#include <fstream>
#include <iostream>
#include <chrono> 

using namespace std;


#ifndef O_GENERIC_H
#define O_GENERIC_H

using namespace std::chrono; 

class oGenericOp: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        //Upstream op, fxn to be called, # columns in output, fxn args
        //fxnArgs - #args, #operators, col count, additional args
        oGenericOp(Operation *, int * (*)(Operation *, int *, void *), int, void *);  
        oGenericOp(Operation **, int, int * (*)(Operation *, int *, void *), int, void *);          
        
    private:
        int * outTuple;
        //Fxn sent in - Upstream op, output tuple, Optional args (# args, args)
        //Args sent in --> if not null, 1st item = # args, followed by args
        int * (*nextFxn)(Operation *, int *, void *);
        time_point<high_resolution_clock> oStart;
        time_point<high_resolution_clock> oEnd;
        duration<double, micro> oDuration;
};

#endif