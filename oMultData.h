#include "iOperation.h"

#ifndef O_MULT_DATA_H
#define O_MULT_DATA_H

class oMultData: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oMultData(Operation *);  
        int tSize();  
        void print(int *);
    private:
        int colCount;
        int current;
        Operation * opp;
        int * tuple;
};


#endif