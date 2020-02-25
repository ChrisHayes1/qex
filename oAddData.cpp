#include <fstream>
#include "oAddData.h"

oAddData::oAddData(Operation * mOpp){
    opp = mOpp;
}

int oAddData::tSize(){
    return colCount;
}

void oAddData::open(){
    opp->open();
    printf("Source data:\n");
    opp->print();
    tuple = new int[opp->tSize()];
}

void oAddData::print(){
    for (int i = 0; i < opp->tSize(); i++){
        printf("[%d]", tuple[i]);            
        fflush(stdout);
    }
    printf("\n");
}

int * oAddData::next(){
    int * mTup;
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
    delete [] tuple;
    opp->close();
}
