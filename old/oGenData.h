#include "iOperation.h"

#ifndef O_GEN_DATA_H
#define O_GEN_DATA_H

class oGenData: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oGenData(int, int);      
        int tSize();  
        void print(int *);
    private:
        int rowCount;
        int colCount;
        int current;
        int * tuple;
};


#endif