#include <fstream>
#include <string>
#include "oGenData.h"
#include "oAddData.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: qex ROWS COLS");
        return 1;
    }
    //Opp to generate data
    oGenData mGeneratedData(std::stoi(argv[1]),std::stoi(argv[2]));
    //Opp to aggregate data    
    oAddData mAggData(&mGeneratedData);
    
    //Open aggreate
    mAggData.open();
    
    //Itterate through aggregate
    int * mVal;   
    printf("\nAgg Steps\n");
    while (mAggData.next() != nullptr){
        mAggData.print();
    }

    printf("\nFinal Row Aggregates\n");
    mAggData.print();    

    mGeneratedData.close();    

    return 0;
}