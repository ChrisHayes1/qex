/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oppOneToMany.h"

/**************
 * Constructor
 *************/

oppOneToMany::oppOneToMany(Operation * mOpp, int * (*openFxn)(int *, Operation *, int *)){
    opp = mOpp;
    mOpenFxn = openFxn;
}

/**************
 * Interface
 *************/

void oppOneToMany::open(){
    opp->open();    
    tuple = new int[opp->tSize()];
    mOpenFxn(mVal, opp, tuple);
}

int * oppOneToMany::next(){
    int * mVal = opp->next();
    mVal = mNextFxn(mVal, opp, tuple);
    if(mVal) print (mVal);
    return mVal;
}

void oppOneToMany::close(){    
    printf("Final output\n");
    print(tuple);
    fflush(stdout);
    opp->close();
    delete [] tuple; 
}

int oppOneToMany::tSize(){
    return opp->tSize();
}

/**************
 * Helper
 *************/

void oppOneToMany::print(int * mPtr){
    printf("+=: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n\n");
}
