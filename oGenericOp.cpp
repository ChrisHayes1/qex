/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oGenericOp.h"
#include <iostream>
#include <chrono> 

using namespace std;

/**************
 * Constructor
 *************/

oGenericOp::oGenericOp(Operation * mOp, int * (*mNextFxn)(Operation *, int *, void *), 
                int mColCount, void * mArgs) : Operation{mOp, mArgs} {    
    nextFxn = mNextFxn;
    setColSize(mColCount);
}


oGenericOp::oGenericOp(Operation ** mOps, int numOps,  int * (*mNextFxn)(Operation *, int *, void *), 
                int mColCount, void * mArgs) : Operation {mOps, numOps, mArgs}{    
    nextFxn = mNextFxn;
    setColSize(mColCount);
}



/**************
 * Interface
 *************/

int oGenericOp::open(){
    int getSize = getColCount();
    bool badOpen = false;
    if (getSize == -1){
        setColSize(0);   
        //else setColSize(op->getColCount());     
    }
    for (int i = 0; i < numOps; i++){
        if (ops[i]->open() == -1) badOpen = true;
        if (getSize == -1) setColSize(getColCount() + ops[i]->getColCount());
    }

    // cout << "Join has size of " << getColCount() << " and getSize = " << getSize << "\n";
    outTuple = new int[getColCount()];
    oStart = high_resolution_clock::now();
    oEnd = high_resolution_clock::now();
    oDuration  = duration_cast<microseconds>(oEnd - oStart);
    if (badOpen){
        return -1;
    }
    return 0;
}

int * oGenericOp::next(){
    oStart = high_resolution_clock::now();
    outTuple = nextFxn(this, outTuple, args);
    if (outTuple && showPrintout) print(outTuple, getColCount(), "-->");
    oEnd = high_resolution_clock::now();
    oDuration  += duration_cast<microseconds>(oEnd - oStart);
    return outTuple;
}

void oGenericOp::close(){    
    //op->close();
    for (int i = 0; i < numOps; i++){
        ops[i]->close();
    }
    delete outTuple;
    if (SHOW_SPEED) cout << "Total duration (join)  = " << oDuration.count()/1000 << "\n";
}

