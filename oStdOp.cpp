/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oStdOp.h"

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
    op->open();    
    if (colCount == -1) colCount = op->tSize();  
    outTuple = new int[colCount];
}

int * oStdOp::next(){
    outTuple = nextFxn(this, outTuple, args);
    if (outTuple && showPrintout) print(outTuple, colCount, "-->");
    return outTuple;
}

void oStdOp::close(){    
    op->close();
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
    //printf("size = %d: ", size);
    fflush(stdout);
    for (int i = 0; i < size; i++){
       //printf("[%*d]", 3, mPtr[i]);            
       printf("mOut[%d] = ", i);
       fflush(stdout);
       printf("%d ", mPtr[i]);
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

