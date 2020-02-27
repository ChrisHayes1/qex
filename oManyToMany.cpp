/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oManyToMany.h"

/**************
 * Constructor
 *************/

oManyToMany::oManyToMany(Operation * mOpp, int * (*nextFxn)(int *, Operation *)){
    opp = mOpp;
    mNextFxn = nextFxn;
}

/**************
 * Interface
 *************/

void oManyToMany::open(){
    opp->open();    
    //tuple = new int[opp->tSize()];
}

int * oManyToMany::next(){
    int * mVal = opp->next();
    mNextFxn(mVal, opp);
    if(mVal) print (mVal);
    return mVal;
}

void oManyToMany::close(){    
    //delete [] tuple;
    opp->close();
}

int oManyToMany::tSize(){
    return opp->tSize();
}

/**************
 * Helper
 *************/

void oManyToMany::print(int * mPtr){
    printf("*2: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
