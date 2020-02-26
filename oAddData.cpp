#include <fstream>
#include "oAddData.h"

oAddData::oAddData(Operation * mOpp){
    opp = mOpp;
}

int oAddData::tSize(){
    return opp->tSize();
}

void oAddData::open(){
    opp->open();
    tuple = new int[opp->tSize()];
}

void oAddData::print(int * mPtr){
    printf("AD: ");
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%d]", mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}

int * oAddData::next(){
    int * mVal;
    if ((mVal = opp->next()) != nullptr) {
        for (int i = 0; i < opp->tSize(); i++){
            tuple[i] += mVal[i];            
        }        
        return tuple;
    } else {
        return nullptr;
    }
}

void oAddData::close(){    
    opp->close();
    oAddData::print(tuple);
    delete [] tuple;    
}
