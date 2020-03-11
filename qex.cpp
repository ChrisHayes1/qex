#include <fstream>
#include <string>
#include "oGenData.h"
#include "oGenDataFull.h"
#include "oppManyToMany.h"
#include "oppManyToSome.h"
#include <chrono> 
#include <iostream>
using namespace std;
using namespace std::chrono; 
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

int main(int argc, char *argv[])
{
    //Verify input
    if (argc != 3) {
        printf("Usage: qex ROWS COLS");
        return 1;
    }

    //Start with basic join between two files 
    // SELECT * from tblA INNER JOIN tblB ON tblA(1) = tblB(1); Where (1) represents the first column
    // 1) Spool tables a and b
    // 2) Hash join
    // 3) Project results

    //auto start = high_resolution_clock::now(); 

    // /***************
    //  * Build Query Tree
    //  **************/
    // //Opp to generate data
    // // srand(time(0));  
    // oGenData mGeneratedData(std::stoi(argv[1]),std::stoi(argv[2]));    
    // oppManyToMany mMultData(&mGeneratedData, multFxn);
    // //oppManyToSome mAggData(&mMultData, maxFxn);
    // oppManyToSome mAggData(&mMultData, addFxn);    
    
    // /**************
    //  * Open aggreate
    //  **************/
    // mAggData.open();
    // printf("\n");
    
    // /**************
    //  * Itterate through aggregate
    //  ************/
    // while (mAggData.next());

    // //Close out tree
    // //printf("Closing");
    // //fflush(stdout);
    // mAggData.close();    

    // auto stop = high_resolution_clock::now(); 
    // auto duration = duration_cast<microseconds>(stop - start);
    // cout << "durration lite " << duration.count()/60000 << endl; 
    // printf("\n");

    // /***
    //  * Full dataset
    //  *****/
    // auto start2 = high_resolution_clock::now(); 

    // oGenDataFull mGeneratedData2(std::stoi(argv[1]),std::stoi(argv[2]));
    // oppManyToMany mMultData2(&mGeneratedData2, multFxn);
    // //oppManyToSome mAggData(&mMultData, maxFxn);
    // oppManyToSome mAggData2(&mMultData2, addFxn);    
    
    // /**************
    //  * Open aggreate
    //  **************/
    // mAggData2.open();
    
    // /**************
    //  * Itterate through aggregate
    //  ************/
    // while (mAggData2.next());

    // //Close out tree
    // //printf("Closing");
    // //fflush(stdout);
    // mAggData2.close();    

    // auto stop2 = high_resolution_clock::now(); 
    // auto duration2 = duration_cast<microseconds>(stop2 - start2);
    // cout << "durration full " << duration2.count()/60000 << endl; 
    // printf("\n");
    // cout << "durration dif " << (duration2.count() - duration.count())/60000 << endl; 
    // printf("\n");
    
    return 0;
}


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