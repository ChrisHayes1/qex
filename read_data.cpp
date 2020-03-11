/*########################################
 # Prints generated datasets for QC purposes
 # Author: C. Todd Hayes
 # Date: 3/10/20
 ########################################*/

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{

    //check input
    if (argc != 2){
        cout << "Usage:  <naome> ";
        return 1;
    }

    //open file
    ifstream file;
    file.open(argv[1], ios::binary);

    if (!file){
        cout << "Unable to generate file";
        return 1;
    }

    int cSize;
    file >> cSize;
    cout << "Size is " << cSize << "\n";

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

}