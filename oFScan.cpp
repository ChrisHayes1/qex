/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oFScan.h"
#include <sys/stat.h>

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
    file.open(mFileName);
    if (!file){
        cerr << "Unable to generate file";
        return -1;
    }
    file >> colSize;
    file >> colSize;
    tuple = new int [colSize];
    return getFileSize();
}

int oFScan::getFileSize(){
    struct stat stat_buf;
    int rc = stat(mFileName.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int * oFScan::next(){
    if (file.good()){
        for (int c = 0; c < colSize; c++){
            file >> tuple[c];
        }
        return tuple;
    }
    return nullptr;    
}

void oFScan::close(){    
    delete tuple;
}

int oFScan::tSize(){
    return colSize;
}

/**************
 * Helper
 *************/
void oFScan::print(int * mPtr, int size, const char * mStr){
    printf("%s: ", mStr);
    for (int i = 0; i < size; i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
