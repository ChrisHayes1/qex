/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oStdOp.h"
#include <iostream>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

/**************
 * Constructor
 *************/

oStdOp::oStdOp(Operation * mOpp, int * (*mNextFxn)(Operation *, int *, int *), int mColCount, int * mArgs){
    args = mArgs;
    op = mOpp;
    nextFxn = mNextFxn;
    colCount = mColCount;
    setPrint(false);
}

/**************
 * Interface
 *************/

int oStdOp::open(){
    //printf("Opening\n");
    fflush(stdout);
    op->open();    
    if (colCount == -1) colCount = op->tSize();  
    outTuple = new int[colCount];
    oStart = high_resolution_clock::now();
    oEnd = high_resolution_clock::now();
    oDuration  = duration_cast<microseconds>(oEnd - oStart);
}

int * oStdOp::next(){
    oStart = high_resolution_clock::now();
    outTuple = nextFxn(this, outTuple, args);
    if (outTuple && showPrintout) print(outTuple, colCount, "-->");
    oEnd = high_resolution_clock::now();
    oDuration  += duration_cast<microseconds>(oEnd - oStart);
    return outTuple;
}

void oStdOp::close(){    
    op->close();
    cout << "Total duration = " << oDuration.count()/1000 << "\n";
    delete [] outTuple; 
}

int oStdOp::tSize(){
    return colCount;
}

/**************
 * Helper
 *************/

void oStdOp::print(int * mPtr, int size, const char * mStr){
    printf("%s: ", mStr);
    for (int i = 0; i < size; i++){
        if (mPtr[i] != -1) printf("[%*d]", 3, mPtr[i]);     
        else printf("[%*s]", 3, "N");
        fflush(stdout);
    }
    printf("\n");
}

void oStdOp::setPrint(bool sPrint){
    showPrintout = sPrint;
}

bool oStdOp::getPrint(){
    return showPrintout;
}
Operation * oStdOp::getUpsOp(){
    return op;
}

Operation ** oStdOp::getUpsOps(){
    return nullptr;
}
