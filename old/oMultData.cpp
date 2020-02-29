/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "oMultData.h"

/**************
 * Constructor
 *************/

oMultData::oMultData(Operation * mOpp){
    opp = mOpp;
}

/**************
 * Interface
 *************/

void oMultData::open(){
    opp->open();    
    //tuple = new int[opp->tSize()];
}

int * oMultData::next(){
    int * mVal = opp->next();
    if (mVal){
        for (int i = 0; i < opp->tSize(); i++)
            mVal[i] *= 2;        
        print(mVal);
    } 
        
    return mVal;
}

void oMultData::close(){    
    //delete [] tuple;
    opp->close();
}

int oMultData::tSize(){
    return opp->tSize();
}

/**************
 * Helper
 *************/

void oMultData::print(int * mPtr){
    printf("*2: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
