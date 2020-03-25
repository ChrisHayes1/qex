#include "iOperation.h"
#include <fstream>
#include <iostream>

using namespace std;

#ifndef O_FSCAN_H
#define O_FSCAN_H

class oFScan: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        oFScan(string);  
        int tSize();  
        void print(int *, int, const char *);
        int getFileSize();
    private:
        Operation * op;        
        string mFileName;
        ifstream file;
        int colSize;
        int * tuple;
};


#endif