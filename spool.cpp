/*######################
 # Multiplies each tuple by 2
 ######################*/

#include <fstream>
#include "spool.h"

/**************
 * Constructor
 *************/


spool::spool(char * fileName){
    mFileName = fileName;
}

/**************
 * Interface
 *************/

int spool::open(){
    file.open(mFileName);
    if (!file){
        cout << "Unable to generate file";
        return 1;
    }
    file >> colSize;
    tuple = new int [colSize];
}

int * spool::next(){

    for (int c = 0; c < colSize; c++){
        file >> tuple[c];
    }
    return tuple;
}

void spool::close(){    
    //delete [] tuple;
    op->close();
}

int spool::tSize(){
    return colSize;
}

/**************
 * Helper
 *************/

void spool::print(int * mPtr){
    printf("*2: ");
    for (int i = 0; i < op->tSize(); i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}
