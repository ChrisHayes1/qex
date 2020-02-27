#include "iOperation.h"

#ifndef O_MANY_TO_MANY_H
#define O_MANY_TO_MANY_H

class oManyToMany: public Operation {
    public:
        //interface
        void open();
        int * next();
        void close();

        //Class specific
        oManyToMany(Operation *, int * (*)(int *, Operation *));  
        int tSize();  
        void print(int *);
    private:
        int colCount;
        int current;
        Operation * opp;
        int * tuple;
        int * (*mNextFxn)(int *, Operation *);
};


#endif