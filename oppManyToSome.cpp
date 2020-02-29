/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oppManyToSome.h"

/**************
 * Constructor
 *************/

oppManyToSome::oppManyToSome(Operation * mOpp, int * (*nextFxn)(int *, Operation *, int *)){
    opp = mOpp;
    mNextFxn = nextFxn;
}

/**************
 * Interface
 *************/

void oppManyToSome::open(){
    opp->open();    
    tuple = new int[opp->tSize()];
}

int * oppManyToSome::next(){
    int * mVal = opp->next();
    mVal = mNextFxn(mVal, opp, tuple);
    if(mVal) print (mVal);
    return mVal;
}

void oppManyToSome::close(){    
    printf("Final output\n");
    print(tuple);
    fflush(stdout);
    opp->close();
    delete [] tuple; 
}

int oppManyToSome::tSize(){
    return opp->tSize();
}

/**************
 * Helper
 *************/

void oppManyToSome::print(int * mPtr){
    printf("+=: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n\n");
}
