#include "iOperation.h"
#include <fstream>

using namespace std;

#ifndef O_OSPOOL_H
#define O_SPOOL_H

class spool: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        spool(char *);  
        int tSize();  
        void print(int *);
    private:
        Operation * op;        
        char * mFileName;
        ifstream file;
        int colSize;
        int * tuple;
};


#endif