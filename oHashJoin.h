#include "iOperation.h"
#include <fstream>
#include <iostream>

using namespace std;

#ifndef O_HASH_JOIN_H
#define O_HASH_JOIN_H

class oHashJoin: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        oHashJoin(Operation *, Operation *);  
        int tSize();  
        void print(int *, int, const char *);
    private:
        Operation * fileA;        
        Operation * fileB;        
        // int colSize;
        // int * tuple;
};

#endif