#include <fstream>
#include <string>
#include "oGenData.h"
#include "oAddData.h"
#include "oMultData.h"
#include "oManyToMany.h"

//int * multFxn(int * mVal, Operation * opp);

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
    oGenData mGeneratedData(std::stoi(argv[1]),std::stoi(argv[2]));
    oManyToMany mMultData(&mGeneratedData, multFxn);
    //Opp to multiple data
    //oMultData mMultipliedData(&mGeneratedData);
    //Opp to multiple data 2
    //oMultData mMultipliedData2(&mMultipliedData);
    //Opp to aggregate data    
    oAddData mAggData(&mMultData);
    
    
    /**************
     * Open aggreate
     **************/
    mAggData.open();
    
    /**************
     * Itterate through aggregate
     ************/
    while (mAggData.next());

    //Close out tree
    printf("Closing");
    fflush(stdout);
    mAggData.close();    

    return 0;
}