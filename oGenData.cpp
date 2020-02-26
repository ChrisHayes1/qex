
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include "oGenData.h"

const int MAX_RAND = 5;

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
    //seed rand
    srand(time(0));
    //Generate array
    for (int r = 0; r < rowCount; r++){
        for (int c = 0; c < colCount; c++){
            tuples[r][c] = rand()%MAX_RAND;
            //tuples[r][c] = (r+1)*(c+1);
            //printf("...x[%d][%d] = %d\n", r, c, tuples[r][c]);
        }
    }

    
}

void oGenData::print(int * nvm){
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
    //printf("Source data:\n");
    //print(nullptr);

    for (int i = 0; i < rowCount; i++){
        delete [] tuples[i];
    }
    delete [] tuples;
}
