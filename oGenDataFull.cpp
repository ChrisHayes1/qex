/*######################
 # Generates random data sets of size numRows, numCols
 ######################*/ 
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include "oGenDataFull.h"

/**************
 * Constants
 *************/

const int MAX_RAND = 30;

/**************
 * Constructor
 *************/
oGenDataFull::oGenDataFull(int numRows, int numCols){
    rowCount = numRows;
    colCount = numCols;
    current = 0;
}


/**************
 * Interface
 *************/

void oGenDataFull::open(){
    //seed rand    
    srand(5);    
    tuples = new int * [rowCount];
    for (int i = 0; i < rowCount; i++){
        tuples[i] = new int [colCount];
    }

    //generate data
    for (int x = 0; x < rowCount; x++){
        for (int y = 0; y < colCount; y++){
            tuples[x][y] = rand()%MAX_RAND;                
        }
    }

    printf("$$: \n");
}

int * oGenDataFull::next(){    
    if (current < rowCount) {               
        for (int c = 0; c < colCount; c++)
            tuples[current][c] = rand()%MAX_RAND;                
        print(tuples[current]);
        current++;
        return tuples[current];
    } 
    return nullptr;        
}

void oGenDataFull::close(){    
    // print(tuples[current-1]);
    for (int i = 0; i < rowCount; i++){
        delete tuples[i];
    }
    delete tuples;
}

int oGenDataFull::tSize(){
    return colCount;
}

/**************
 * Helper Functions
 *************/

void oGenDataFull::print(int * nvm){ 
    printf("$$: ");   
    for (int i = 0; i < tSize(); i++){        
        printf("[%*d]", 3, nvm[i]);            
        fflush(stdout);
    }
    printf("\n");

}