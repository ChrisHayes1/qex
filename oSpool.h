#include "iOperation.h"
#include <fstream>
#include <iostream>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

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
        //Set col we want to pull on itteration, bring back to start if not
        //initial pass
        void rewind();  
        
    private:
        int * tuple;

        time_point<high_resolution_clock> oStart;
        time_point<high_resolution_clock> oEnd;
        duration<double, micro> oDuration;

        node * head;
        node * end;
        node * current;
        bool rewound;
        bool isOpened;
        // linked list
        void init_list();
        void enQ(int *);
        void close_list();
        void printsList();
        
};
#endif