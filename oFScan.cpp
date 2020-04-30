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
    //setPrint(false);
}

/**************
 * Interface
 *************/
int oFScan::open(){
    oStart = high_resolution_clock::now();
    file.open(mFileName);
    if (!file){
        cerr << "Unable to generate file";
        return -1;
    }
    int mColSize, mRowSize;
    file >> mRowSize;
    file >> mColSize;
    setColSize(mColSize);
    tuple = new int [getColCount()];
    
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

// int oFScan::getColCount(){
//     return colSize;
// }

// Operation * oFScan::getUpsOp(){
//     return nullptr;
// }

// Operation ** oFScan::getUpsOps(){
//     return nullptr;
// }

// bool oFScan::getPrint(){
//     return showPrintout;
// }
// void oFScan::setPrint(bool sPrint){
//     showPrintout = sPrint;
// }

// /**************
//  * Helper
//  *************/
// void oFScan::print(int * mPtr, int size, const char * mStr){
//     printf("%s: ", mStr);
//     for (int i = 0; i < size; i++){
//         printf("[%*d]", 3, mPtr[i]);            
//         fflush(stdout);
//     }
//     printf("\n");
// }


