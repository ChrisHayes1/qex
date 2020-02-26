
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include "oGenData.h"

/**************
 * Constants
 *************/

const int MAX_RAND = 5;

/**************
 * Constructor
 *************/
oGenData::oGenData(int numRows, int numCols){
    rowCount = numRows;
    colCount = numCols;
    current = 0;
    //tuples = new int* [rowCount];
    // for (int i = 0; i < rowCount; i++){
    //     tuples[i] = new int[colCount];
    // }
}


/**************
 * Interface
 *************/

void oGenData::open(){
    //seed rand    
    srand(time(0));    
    //Generate array
    // for (int r = 0; r < rowCount; r++){
    //     // for (int c = 0; c < colCount; c++){
    //     //     tuples[r][c] = rand()%MAX_RAND;
    //     //     //tuples[r][c] = (r+1)*(c+1);
    //     //     //printf("...x[%d][%d] = %d\n", r, c, tuples[r][c]);
    //     // }
    // } 
}

int * oGenData::next(){
    int * tuple;
    tuple = new int [colCount];
    if (current < rowCount) {       
        for (int c = 0; c < colCount; c++){
            tuple[c] = rand()%MAX_RAND;
            //tuples[r][c] = (r+1)*(c+1);
            //printf("...x[%d][%d] = %d\n", r, c, tuples[r][c]);            
            //print(tuple);
        }
        current++;
        return tuple;
    } else {
        return nullptr;    
    }
}

void oGenData::close(){    
    //printf("Source data:\n");
    //print(nullptr);

    // for (int i = 0; i < rowCount; i++){
    //     delete [] tuples[i];
    // }
    //delete [] tuples;
}

int oGenData::tSize(){
    return colCount;
}

/**************
 * Helper Functions
 *************/

void oGenData::print(int * nvm){
    //Generate array
    // for (int r = 0; r < rowCount; r++){
    //     for (int c = 0; c < colCount; c++){
    //         printf("[%d]", tuples[r][c]);
    //     }
    //     printf("\n");
    // }
    printf("GN: ");
    for (int i = 0; i < tSize(); i++){
        printf("[%d]", nvm[i]);            
        fflush(stdout);
    }
    printf("\n");
}