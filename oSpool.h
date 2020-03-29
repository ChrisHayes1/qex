#include "iOperation.h"
#include <fstream>
#include <iostream>

using namespace std;

#ifndef O_SPOOL_H
#define O_SPOOL_H

struct node{    
    int * tuple;
    node * next;
};

class oSpool: public Operation {
    public:
        //interface
        int open();
        int * next();
        void close();

        //Class specific
        oSpool(Operation *);  //Need a pointer at init
        int tSize();  
        void print(int *, int, const char *);
        //Set col we want to pull on itteration, bring back to start if not
        //initial pass
        void rewind(int colNum);  
        
    private:
        Operation * op;        
        int colSize;
        int * tuple;
        node * head;
        node * end;
        void init_list();
        void enQ(int *);
        void close_list();
        void printsList();
};



#endif