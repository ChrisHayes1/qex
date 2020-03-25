/*########################################
 # A series of static functions utilized throughout qex
 # Author: C. Todd Hayes
 # Date: 3/10/20
 ########################################*/
#include <fstream>
#include <iostream>
#include "static.h"

using namespace std;

void mStatic::test(){
    cout << "Test";
}


/*############################
# Generate new datasets
############################*/

/**************
 * Constants
 *************/

const int MAX_RAND = 30;

int mStatic::genData(string fileName, int rows, int cols, string usage){
    //Send in file name, # of rows, # of columns

    if (rows == 0 || cols == 0){
        cout << "Usage: " << usage;
        return 1;
    }

    //generate file
    ofstream file;
    file.open(fileName, ios::binary);

    if (!file){
        cout << "Unable to generate file";
        return 1;
    }
     //seed random
    srand(time(0));
    //First line is number of cols per tuple
    file << rows << " " << cols; 
    int num;
    //generate data
    for (int x = 0; x < rows; x++){
        for (int y = 0; y < cols; y++){
            num = rand()%MAX_RAND;
            //cout << "Outputing " << num << "\n";
            file << " " << num;
        }
    }

    file.close();

    cout << fileName << " was generated successfully\n";
    return 0;
}


int mStatic::readData(string fileName){
    //open file
    ifstream file;
    file.open(fileName, ios::binary);

    if (!file){
        cout << "Unable to generate file";
        return 1;
    }

    int cSize;
    int rSize;
    file >> rSize >> cSize;
    cout << "Size is " << rSize << " rows and " << cSize << " columns\n";

    int next; //Store next value
    int count = 0;

    while (file.good()){
        file >> next;
        cout << next << "\t";
        count ++;
        if (count == cSize) {
            count = 0;
            cout << "\n";
        }
    }
    cout << "\n";

    return 0;
}