/*######################
 # Sum tuples
 ######################*/ 

#include <fstream>
#include "oAddData.h"

/**************
 * Constructor
 *************/

oAddData::oAddData(Operation * mOpp){
    opp = mOpp;
}

/**************
 * Interface
 *************/

void oAddData::open(){
    opp->open();
    tuple = new int[opp->tSize()];
}

int * oAddData::next(){
    int * mVal =  opp->next();
    if (mVal) {
        for (int i = 0; i < opp->tSize(); i++)
            tuple[i] += mVal[i];                            
        print(tuple);
        return tuple;
    } 
    return mVal;
}

void oAddData::close(){        
    printf("Final output\n");
    print(tuple);
    fflush(stdout);
    opp->close();
    delete [] tuple;    
}

int oAddData::tSize(){
    return opp->tSize();
}
/**************
 * Helper function
 *************/

void oAddData::print(int * mPtr){
    printf("+=: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n\n");
}