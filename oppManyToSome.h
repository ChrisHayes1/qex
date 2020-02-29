#include "iOperation.h"

#ifndef O_MANY_TO_SOME_H
#define O_MANY_TO_SOME_H

class oppManyToSome: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oppManyToSome(Operation *, int * (*)(int *, Operation *, int *));  
        int tSize();  
        void print(int *);
    private:
        //int colCount;
        //int current;
        Operation * opp;
        int * tuple;
        int * (*mNextFxn)(int *, Operation *, int *);
};


#endif