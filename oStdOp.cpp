/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oStdOp.h"

/**************
 * Constructor
 *************/

oStdOp::oStdOp(Operation * mOpp, int * (*nextFxn)(Operation *, int *), int mColCount){
    colCount = mColCount;
    op = mOpp;
    mNextFxn = nextFxn;
}

/**************
 * Interface
 *************/

int oStdOp::open(){
    op->open();    
    mOut = new int[colCount];
}

int * oStdOp::next(){
    mOut = mNextFxn(op, mOut);
    return mOut;
}

void oStdOp::close(){    
    op->close();
    delete [] mOut; 
}

int oStdOp::tSize(){
    return colCount;
}

/**************
 * Helper
 *************/

void oStdOp::print(int * mPtr, int size, const char * mStr){
    printf("%s: ", mStr);
    printf("size = %d: ", size);
    fflush(stdout);
    for (int i = 0; i < size; i++){
       //printf("[%*d]", 3, mPtr[i]);            
       printf("mOut[%d] = ", i);
       fflush(stdout);
       printf("%d\n", mPtr[i]);
        fflush(stdout);
    }
    printf("\n");
}

