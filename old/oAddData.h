#include "iOperation.h"

#ifndef O_ADD_DATA_H
#define O_ADD_DATA_H

class oAddData: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oAddData(Operation *);  
        int tSize();  
        void print(int *);
    private:
        Operation * opp;
        int * tuple;
};


#endif