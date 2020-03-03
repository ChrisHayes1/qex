#include <fstream>
#include <string>
#include "oGenData.h"
#include "oppManyToMany.h"
#include "oppManyToSome.h"

//int * multFxn(int * mVal, Operation * opp);

/**
 * I should send args in as void argv[]
 * Should allow Open() fxn as well
 * I want them somewhere not with main.  In oppX class?
 * or fxn catalog?
 * Also, the three fxns below are nearly identicle
 * We may be able to move more to the global op
 * and just have a simple fxn carry out the rest
 * 
 ***/

//Generate tuples
// int * addFxn(int * mVal, Operation * opp, int * tuple){
//     if (current < rowCount) {       
//         current++;
//         for (int c = 0; c < colCount; c++)
//             tuple[c] = rand()%MAX_RAND;                
//         print(tuple);
//         return tuple;
//     } 
//     return nullptr; 
// }

//Add tuples together
int * addFxn(int * mVal, Operation * opp, int * tuple){
    if (mVal) {
        for (int i = 0; i < opp->tSize(); i++)
            tuple[i] += mVal[i];                            
        return tuple;
    } 
    return mVal;
}

//Calc max in each row
int * maxFxn(int * mVal, Operation * opp, int * tuple){
    if (mVal) {
        for (int i = 0; i < opp->tSize(); i++)
            if (mVal[i] > tuple[i]) tuple[i] = mVal[i];
        return tuple;
    } 
    return mVal;
}

//Multiplies numerics in tuple by 2
int * multFxn(int * mVal, Operation * opp){
    //int * mVal = opp->next();
    if (mVal){
        for (int i = 0; i < opp->tSize(); i++)
            mVal[i] *= 2;        
    } 
        
    return mVal;
}

int main(int argc, char *argv[])
{
    //Verify input
    if (argc != 3) {
        printf("Usage: qex ROWS COLS");
        return 1;
    }

    /***************
     * Build Query Tree
     **************/
    //Opp to generate data
    srand(time(0));  
    oGenData mGeneratedData(std::stoi(argv[1]),std::stoi(argv[2]));
    oppManyToMany mMultData(&mGeneratedData, multFxn);
    oppManyToSome mAggData(&mMultData, maxFxn);
    //oppManyToSome mAggData(&mAggData, addFxn);    
    
    /**************
     * Open aggreate
     **************/
    mAggData.open();
    
    /**************
     * Itterate through aggregate
     ************/
    while (mAggData.next());

    //Close out tree
    //printf("Closing");
    //fflush(stdout);
    mAggData.close();    

    return 0;
}