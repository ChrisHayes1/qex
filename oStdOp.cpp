/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oStdOp.h"

/**************
 * Constructor
 *************/

oStdOp::oStdOp(Operation * mOpp, int * (*mNextFxn)(Operation *, int *, int *), int * mArgs){
    args = mArgs;
    op = mOpp;
    nextFxn = mNextFxn;
}

/**************
 * Interface
 *************/

int oStdOp::open(){
    op->open();    
    if (args != nullptr && args[1] != -1) colCount = args[1];
    else colCount = op->tSize();  
    outTuple = new int[colCount];
}

int * oStdOp::next(){
    outTuple = nextFxn(op, outTuple, args);
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

