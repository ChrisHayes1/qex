/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oFScan.h"
#include <sys/stat.h>
#include <iostream>
#include <chrono> 

using namespace std;

/**************
 * Constructor
 *************/
oFScan::oFScan(string fileName){
    mFileName = fileName;
}

/**************
 * Interface
 *************/
int oFScan::open(){
    setColSize(0);
    oStart = high_resolution_clock::now();
    tuple = new int [getColCount()];
    file.open(mFileName);
    if (!file){
        cerr << "Unable to open file\n";
        fflush(stderr);
        return -1;
    }
    int mColSize, mRowSize;
    file >> mRowSize;
    file >> mColSize;
    setColSize(mColSize);
    oEnd = high_resolution_clock::now();
    oDuration  = duration_cast<microseconds>(oEnd - oStart);
    return getFileSize();
}

int oFScan::getFileSize(){
    struct stat stat_buf;
    int rc = stat(mFileName.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int * oFScan::next(){
    oStart = high_resolution_clock::now();
    if (file.good()){
        for (int c = 0; c < getColCount(); c++){
            file >> tuple[c];
        }
        oEnd = high_resolution_clock::now();
        oDuration  += duration_cast<microseconds>(oEnd - oStart);
        return tuple;
    }
    oEnd = high_resolution_clock::now();
    oDuration  += duration_cast<microseconds>(oEnd - oStart);
    return nullptr;    
}

void oFScan::close(){ 
    if (SHOW_SPEED) cout << "Total duration (fscan) = " << oDuration.count()/1000 << "\n";   
    delete tuple;
}