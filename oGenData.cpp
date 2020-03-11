/*######################
 # Generates random data sets of size numRows, numCols
 ######################*/ 
#include <fstream>
#include <cstdlib>
#include <ctime> 
#include "oGenData.h"

/**************
 * Constants
 *************/

const int MAX_RAND = 30;

/**************
 * Constructor
 *************/
oGenData::oGenData(int numRows, int numCols){
    rowCount = numRows;
    colCount = numCols;
    current = 0;
}


/**************
 * Interface
 *************/

void oGenData::open(){
    //seed rand    
    srand(5);    
    tuple = new int [colCount];
}

int * oGenData::next(){    
    if (current < rowCount) {       
        current++;
        for (int c = 0; c < colCount; c++)
            tuple[c] = rand()%MAX_RAND;                
        print(tuple);
        return tuple;
    } 
    return nullptr;        
}

void oGenData::close(){   
    delete tuple;
}

int oGenData::tSize(){
    return colCount;
}

/**************
 * Helper Functions
 *************/

void oGenData::print(int * nvm){
    printf("$$: ");
    for (int i = 0; i < tSize(); i++){
        printf("[%*d]", 3, nvm[i]);            
        fflush(stdout);
    }
    printf("\n");
}