/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oJoin.h"

/**************
 * Constructor
 *************/


oJoin::oJoin(Operation ** mOps, int * (*mNextFxn)(Operation *, int *, int *), int mColCount, int * mArgs){
    args = mArgs;
    ops = mOps;
    nextFxn = mNextFxn;
    colCount = mColCount;
    setPrint(false);    
}

/**************
 * Interface
 *************/

int oJoin::open(){
    int getSize = colCount;
    if (getSize == -1){
        colCount = 0;
        // Find true out tuple size
        for (int i = 0; i < args[1]; i++){
            ops[i]->open();
            colCount += ops[i]->tSize();
        }
    }

    outTuple = new int[colCount];
}

int * oJoin::next(){
    outTuple = nextFxn(this, outTuple, args);
    if (outTuple && showPrintout) print(outTuple, colCount, "-->");
    return outTuple;
}

void oJoin::close(){    
    for (int i = 0; i < args[1]; i++){
        ops[i]->close();
    }
}

int oJoin::tSize(){
    return colCount;
}

/**************
 * Helper
 *************/

void oJoin::print(int * mPtr, int size, const char * mStr){
    printf("%s: ", mStr);
    for (int i = 0; i < size; i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}

void oJoin::setPrint(bool sPrint){
    showPrintout = sPrint;
}

bool oJoin::getPrint(){
    return showPrintout;
}

Operation * oJoin::getUpsOp(){
    return nullptr;
}

Operation ** oJoin::getUpsOps(){
    return ops;
}