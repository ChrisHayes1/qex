/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oHashJoin.h"

/**************
 * Constructor
 *************/


oHashJoin::oHashJoin(Operation * iFileA, Operation * iFileB){
    fileA = iFileA;
    fileB = iFileB;
}

/**************
 * Interface
 *************/

int oHashJoin::open(){
    int szA = fileA->open();
    cout <<  "FileA has a size of " << szA << "\n";
    int szB = fileB->open();
    cout <<  "FileB has a size of " << szB << "\n";

    //Build has file on smaller of the two relations
}

int * oHashJoin::next(){
    //Hash larger of the two relations against the table
    int * mTplA;
    const char * strA;
    strA = "A=";
    const char * strB;
    strB  = "B=";
    int * mTplB;
    mTplA = fileA->next();
    if (mTplA) print(mTplA, fileA->tSize(), strA);
    
    mTplB = fileB->next();
    //if (mTplB) print(mTplB, fileB->tSize(), strB);

    if (mTplB || mTplA) return mTplA;
    else return nullptr;
}

void oHashJoin::close(){    
    fileA->close();
    fileB->close();
}

int oHashJoin::tSize(){
    return fileA->tSize();
}

/**************
 * Helper
 *************/

void oHashJoin::print(int * mPtr, int size, const char * mStr){
    printf("%s: ", mStr);
    for (int i = 0; i < size; i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
