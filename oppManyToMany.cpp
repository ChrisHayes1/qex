/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oppManyToMany.h"

/**************
 * Constructor
 *************/

oppManyToMany::oppManyToMany(Operation * mOpp, int * (*nextFxn)(int *, Operation *)){
    opp = mOpp;
    mNextFxn = nextFxn;
}

/**************
 * Interface
 *************/

void oppManyToMany::open(){
    opp->open();    
    //tuple = new int[opp->tSize()];
}

int * oppManyToMany::next(){
    int * mVal = opp->next();
    mNextFxn(mVal, opp);
    if(mVal) print (mVal);
    return mVal;
}

void oppManyToMany::close(){    
    //delete [] tuple;
    opp->close();
}

int oppManyToMany::tSize(){
    return opp->tSize();
}

/**************
 * Helper
 *************/

void oppManyToMany::print(int * mPtr){
    printf("*2: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
