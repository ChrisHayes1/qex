/*########################################
 # Generate datasets for use in quex
 # Author: C. Todd Hayes
 # Date: 3/10/20
 ########################################*/

#include <iostream>
#include <fstream>
using namespace std;

/**************
 * Constants
 *************/

const int MAX_RAND = 30;

int main(int argc, char *argv[])
{
    //Send in file name, # of rows, # of columns
    int rows;
    int cols;

    //check input
    if (argc != 4){
        cout << "Usage:  <name> <rows> <columns>";
        return 1;
    }

    rows = atoi(argv[2]);
    cols = atoi(argv[3]);

    if (rows == 0 || cols == 0){
        cout << "Usage:  <name> <rows> <columns>";
        return 1;
    }

    //generate file
    ofstream file;
    file.open(argv[1], ios::binary);

    if (!file){
        cout << "Unable to generate file";
        return 1;
    }
     //seed random
    srand(time(0));
    //First line is number of cols per tuple
    file << cols; 
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
    return 0;
}