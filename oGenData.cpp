
#include <fstream>
#include "oGenData.h"



oGenData::oGenData(int numRows, int numCols){
    rowCount = numRows;
    colCount = numCols;
    current = 0;
    tuples = new int* [rowCount];
    for (int i = 0; i < rowCount; i++){
        tuples[i] = new int[colCount];
    }
}

int oGenData::tSize(){
    return colCount;
}

void oGenData::open(){
    //Generate array
    for (int r = 0; r < rowCount; r++){
        for (int c = 0; c < colCount; c++){
            tuples[r][c] = (r+1)*(c+1);
            //printf("...x[%d][%d] = %d\n", r, c, tuples[r][c]);
        }
    }
}

void oGenData::print(){
    //Generate array
    for (int r = 0; r < rowCount; r++){
        for (int c = 0; c < colCount; c++){
            printf("[%d]", tuples[r][c]);
        }
        printf("\n");
    }
}
int * oGenData::next(){
    if (current < rowCount)
        return tuples[current++];
    else
        return nullptr;
    
}

void oGenData::close(){    
    for (int i = 0; i < rowCount; i++){
        delete [] tuples[i];
    }
    delete [] tuples;
}
