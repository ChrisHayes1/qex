#include "iOperation.h"

#ifndef O_GEN_DATA_FULL_H
#define O_GEN_DATA_FULL_H

class oGenDataFull: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oGenDataFull(int, int);      
        int tSize();  
        void print(int *);
    private:
        int rowCount;
        int colCount;
        int current;
        int ** tuples;
};


#endif