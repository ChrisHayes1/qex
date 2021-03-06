#include "iOperation.h"
#include <fstream>
#include <iostream>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

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
        int getFileSize();
    private:
        string mFileName;
        ifstream file;
        int * tuple;
        time_point<high_resolution_clock> oStart;
        time_point<high_resolution_clock> oEnd;
        duration<double, micro> oDuration;
};


#endif