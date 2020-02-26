#include <fstream>
#include "oMultData.h"

oMultData::oMultData(Operation * mOpp){
    opp = mOpp;
}

int oMultData::tSize(){
    return opp->tSize();
}

void oMultData::open(){
    opp->open();    
    //tuple = new int[opp->tSize()];
}

void oMultData::print(int * mPtr){
    printf("MD: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%d]", mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}

int * oMultData::next(){
    int * mVal;
    if ((mVal = opp->next()) != nullptr) {
        for (int i = 0; i < opp->tSize(); i++){
            //tuple[i]  = mVal[i] * 2;
            mVal[i] *= 2;
        }
        //print(mVal);
        return mVal;
    } else {
        return nullptr;
    }
}

void oMultData::close(){    
    //delete [] tuple;
    opp->close();
}
