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
        return tuple;
    } 
    return mVal;
}

void oAddData::close(){        
    oAddData::print(tuple);
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
    printf("AD: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%d]", mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}