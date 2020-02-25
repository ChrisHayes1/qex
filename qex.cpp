#include <fstream>
#include <string>
#include "oGenData.h"
#include "oAddData.h"
#include "oMultData.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: qex ROWS COLS");
        return 1;
    }
    //Opp to generate data
    oGenData mGeneratedData(std::stoi(argv[1]),std::stoi(argv[2]));
    //Opp to multiple data
    oMultData mMultipliedData(&mGeneratedData);
    //Opp to aggregate data    
    oAddData mAggData(&mMultipliedData);
    
    //Open aggreate
    mAggData.open();
    
    //Itterate through aggregate
    printf("\nAgg Steps\n");
    while (mAggData.next() != nullptr){
        //mAggData.print();
    }

    printf("\nFinal Row Aggregates\n");
    //mAggData.print();    

    mGeneratedData.close();    

    return 0;
}