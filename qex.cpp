/*############################
# Includes
############################*/
#include <fstream>
#include <string>
//#include <cstring>
#include <chrono> 
#include <iostream>
#include <vector> 
#include <sstream>
#include <unistd.h>

#include "oFScan.h"
#include "oSpool.h"
#include "oStdOp.h"
#include "oJoin.h"
//#include "oppManyToSome.h"
//#include "oHashJoin.h"
#include "iOperation.h"


using namespace std;
using namespace std::chrono; 

/*############################
# Constants
############################*/
const int M_NULL = -1;
const int ARG_START = 1;
/*############################
# Declare functions
############################*/

//Dataset Manipulation
int genData(string, int, int, int, string, int); //generate new data
int readData(string); //print out data file
int summarizeData(string);

//Menu
void init_menu();
void print_menu();
void menu();

//Queries
void runQryA(string);
void runQryB(string, int, int);
void runQryC(string, string, int, int, int);

//Query Fxns

int * singleLineJoin(Operation *, int *, int *);
int * countFxn(Operation *, int *, int *);
int * projectFxn(Operation *, int *, int *);
int * removeNullFxn(Operation *, int *, int *);

int * addFxn(Operation *, int *, int *);
// int * maxFxn(int *, Operation *, int *);

//Helper Functions
std::vector<std::string> split(const std::string&, char);


/*############################
# Main
############################*/
int main(int argc, char *argv[])
{
    menu();
    return 0;
}

/*############################
# Menu
############################*/

const int MENU_COUNT = 7;

struct menu_item{
    string intro;
    string usage;
    int param_count;
} menu_items[MENU_COUNT];

const int M_EXIT = 0;
const int M_GEN_DATA = 1;
const int M_READ_DATA = 2;
const int M_SUMMARIZE_DATA = 3;
const int M_QRY_A = 4;
const int M_QRY_B = 5;
const int M_QRY_C = 6;


void init_menu(){
    //Generate menu
    //Exit
    menu_items[M_EXIT].intro = "Exit Program";
    menu_items[M_EXIT].usage = "";
    menu_items[M_EXIT].param_count = 0;
    //Generate data files
    menu_items[M_GEN_DATA].intro = "Generate Data file";
    menu_items[M_GEN_DATA].usage = "<file_name> <num_rows> <num_cols> <max_value> <set_as_null (-1 to ignore)>";
    menu_items[M_GEN_DATA].param_count = 5;
    //Read data files
    menu_items[M_READ_DATA].intro = "Print Data file   ";
    menu_items[M_READ_DATA].usage = "<file_name>";
    menu_items[M_READ_DATA].param_count = 1;
    //Summarize data files
    menu_items[M_SUMMARIZE_DATA].intro = "Sumarize Data file   ";
    menu_items[M_SUMMARIZE_DATA].usage = "<file_name>";
    menu_items[M_SUMMARIZE_DATA].param_count = 1;
    //Run Query 1
    menu_items[M_QRY_A].intro = "QRY: SELECT COUNT (*) as Z FROM T";
    menu_items[M_QRY_A].usage = "<file name A>";
    menu_items[M_QRY_A].param_count = 1;
    //Run Query 2
    menu_items[M_QRY_B].intro = "QRY: SELECT COUNT (colA) as X, COUNT (colB) as Y from tblA";
    menu_items[M_QRY_B].usage = "<file name A> <col a (as int)> <col b (as int)>";
    menu_items[M_QRY_B].param_count = 3;
    //Run Query 3
    menu_items[M_QRY_C].intro = "QRY: Select SUM(col) from tblA, tblB WHERE tblA.col# = tblB.col#";
    menu_items[M_QRY_C].usage = "<file name A> <file name B> <sum col> <join col a> <join col b>";
    menu_items[M_QRY_C].param_count = 5;
}

void print_menu(){
    cout << "\n*****************************\n";
    cout << "* What would you like to do?\n";
    cout << "*****************************\n";
    cout << "Opt #";
    for (int i = 0; i < MENU_COUNT; i++){
        cout << "\t" << i << " - " << menu_items[i].intro << "\n"; 
    }
    cout << ">> ";
}

void menu(){
    init_menu();    
    
    string userInput;
    bool stay = true;
    int userChoice = -999;

    //run menu options and repeate
    while (stay){
        if (userChoice != -999) {
            sleep(1);
        }
        print_menu();    
        getline(cin, userInput);
        //Ask again if blank input
        if (userInput == "") continue;
        //tokenize user input
        vector<string> tokens;                             
        tokens = split(userInput, ' ');   
        //Validate params
        //Need to make sure that first item is numeric, and that if it is, that the number of params match
        userChoice = -1;
        try {
            userChoice = stoi(tokens[0]);
        } catch (...) {
            //error will be handled below
        }        
        //Verfy number of params (ignores 0)
        if (userChoice >= 0 && userChoice <= MENU_COUNT 
                && menu_items[userChoice].param_count != tokens.size() - 1) {
            cout << "\nInvalid number of parametrs\n";
            cout << "Usage: " << menu_items[userChoice].usage << "\n";
            continue;
        }
        //Branch user input
        cout << "\n";
        switch (userChoice)
        {
            case M_EXIT:
                stay = false;
                break;            
            case M_GEN_DATA:
                int rows; 
                int cols;
                int max;
                int set_as_null;
                //Verify and pull out int data
                try{
                    rows = stoi(tokens[2]);
                    cols = stoi(tokens[3]);
                    max = stoi(tokens[4]);
                    set_as_null = stoi(tokens[5]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                }           
                genData(tokens[1],rows, cols, max, menu_items[userChoice].usage, set_as_null);
                break;
            case M_READ_DATA:
                readData(tokens[1]);
                break;
            case M_SUMMARIZE_DATA:
                summarizeData(tokens[1]);
                break;                
            case M_QRY_A:
                runQryA(tokens[1]);
                break;
            case M_QRY_B:
                int sumColA;
                int sumColB;
                //Verify and pull out int data
                try{
                    sumColA = stoi(tokens[2]);
                    sumColB = stoi(tokens[3]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryB(tokens[1], sumColA, sumColB);
                break;
            case M_QRY_C:
                int sumCol; 
                int joinColA;
                int joinColB;
                //Verify and pull out int data
                try{
                    sumCol = stoi(tokens[3]);
                    joinColA = stoi(tokens[4]);
                    joinColB = stoi(tokens[5]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryC(tokens[1], tokens[2], sumCol, joinColA, joinColB);
                break;
            default:
                cout << "Invalid input\n";
                break;
        }
    }
}


/*############################
# Generate new datasets
############################*/
int genData(string fileName, int rows, int cols, int maxSize, string usage, int set_as_null){
    // Send in file name, # of rows, # of columns
    if (rows == 0 || cols == 0){
        cout << "Usage: " << usage;
        return 1;
    }
    // generate file
    ofstream file;
    file.open(fileName, ios::binary);

    if (!file){
        cout << "Unable to generate file";
        return 1;
    }
    // seed random
    srand(time(0));
    // First line is number of cols per tuple
    file << rows << " " << cols; 
    int num;
    // generate data
    for (int x = 0; x < rows; x++){
        for (int y = 0; y < cols; y++){
            num = rand()%maxSize;
            if (num == set_as_null) num = M_NULL;
            file << " " << num;
        }
    }
    file.close();
    cout << fileName << " was generated successfully\n";
    return 0;
}

/*############################
# Print out data from datasets
############################*/
int readData(string fileName){
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
    cout << "Dimensions are " << rSize << " rows by " << cSize << " columns\n";
    int next; //Store next value
    int count = 0;
    while (file.good()){
        file >> next;
        if (next == M_NULL) cout << "N" << "\t";
        else cout << next << "\t";
        count ++;
        if (count == cSize) {
            count = 0;
            cout << "\n";
        }
    }
    cout << "\n";
    return 0;
}

/*############################
# Summarize Data
############################*/
int summarizeData(string fileName){
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
    cout << "Dimensions are " << rSize << " rows by " << cSize << " columns\n";
    int next; //Store next value
    int count = 0;
    int rowCount = 0;
    while (file.good()){
        file >> next;
        count ++;
        if (count == cSize) {
            count = 0;
            rowCount++;
        }
    }
    cout << "True Row Size = " << rowCount << "\n";
    return 0;
}

/*############################
# Queries
############################*/
/**
 * Qry 1 in paper
 * SELECT COUNT (*) as Z FROM T
 ***/
void runQryA(string inFile){
    oFScan fileA(inFile);
    oSpool spoolA(&fileA);
    oStdOp countA(&spoolA, countFxn, 1, nullptr); //updastream op, fxn to run, size of new tupple

    countA.open();
    int * temp = countA.next();
    cout << "Z = [" << temp[0] << "]\n";
    fflush(stdout);

    // while (temp) {
    //     cout << "Z = [" << temp[0] << "]\n";
    //     fflush(stdout);
    //     temp = countA.next();
    // };


    // fflush(stdout);
    countA.close();

}
/**
 * Qry 13 in paper
 * SELECT COUNT (colA) as X, COUNT (colB) as Y from tblA
 * 
 *                     Scan --> Bring into memory or spool
 *                       |    
 *                    Project (colA, colB)               
 *                       |
 *                     Spool
 *              _________|__________
 *             |                    |
 *          Project A             Project B
 *             |                    |
 *        Filter Null A        Filter Null B
 *             |                    |
 *           Count A             Count B
 *             |____________________|
 *                       |
 *                   Join Result
 ***/
void runQryB(string inFile, int sumColA, int sumColB){
    //Scan in file
    oFScan fileA(inFile); 
    //Project file down to A, B
    int prjAB [3] = {2, sumColA, sumColB};
    oStdOp projectAB(&fileA, projectFxn, 2, prjAB);
    sumColA = 1;
    sumColB = 2;
    
    //Spool A, B
    oSpool spool(&projectAB);
    
    /*****
     * Branch A
     ****/
    //Project down to just A
    int prjA [2] = {1, sumColA};
    oStdOp projectA(&spool, projectFxn, 1, prjA);
    
    //Remove null from A
    int rmvA [2] = {1, 1};
    oStdOp removeNullA(&projectA, removeNullFxn, 1, rmvA);
    //removeNullA.setPrint(true);

    // //Count total number of args in A
    // int countAargs [2] = {1, 1};
    // Count A needs to be joined with final response
    oStdOp countA(&removeNullA, countFxn, 1, nullptr); 
    //oStdOp countA(&removeNullA, addFxn, -1, nullptr); 
 
    //Execute Branch A count;
    // cout << "Executing branch A\n";
    // countA.open();
    // countA.setPrint(true);
    // int * cntX;
    // cntX = countA.next();
    // //while (countA.next());
    // //countA.close();
    // fflush(stdout);

    countA.setPrint(true);


    /*****
     * Branch b
     ****/
    //spool.rewind();    
    //Project down to just A
    int prjB [2] = {1, sumColB};
    oStdOp projectB(&spool, projectFxn, 1, prjB);

    //Remove null from A
    int rmvB [3] = {1, 1};
    oStdOp removeNullB(&projectB, removeNullFxn, 1, rmvB);    

    // //Count total number of args in A
    // int countAargs [2] = {1, 1};
    // Count A needs to be joined with final response
    oStdOp countB(&removeNullB, countFxn, 1, nullptr); 
    //oStdOp countB(&removeNullB, addFxn, 1, nullptr); 
    countB.setPrint(true);
    // cout << "Executing branch B\n";
    // countB.open();
    // countB.setPrint(true);
    // while (countB.next());
    // countB.close();
    // fflush(stdout);
 
    // // //Display count;
    // // // countA.open();
    // // // countA.setPrint(true);
    // // // int * temp = countA.next();
    // // // while (countA.next());
    // countA.close();


    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    cout << "Preping join\n";
    fflush(stdout);
    Operation * joinOps[2];
    joinOps[0] = &countA;
    joinOps[1] = &countB;
    int jArgs [2] = {1, 2}; //send in exepcted # of operations
    oJoin cartJoin(joinOps, singleLineJoin, -1, jArgs);
    cout << "Opening join\n";
    fflush(stdout);
    cartJoin.open();
    cartJoin.setPrint(true);
    cout << "Running join\n";
    fflush(stdout);
    cartJoin.next();
    //while (cartJoin.next());
    cout << "Closing join\n";
    fflush(stdout);
    cartJoin.close();
}
/**
 * If spooling can be used by multiple objects, then it needs to keep
 * some information in memory.  A simple apporach would be to have 
 * spool track a pointer based on the operation that is calling it
 * 
 *                      Scan --> Bring into memory or spool
 *                       |                   
 *                     Spool
 *                       |
 *                     _____
 *                    |     |
 *                  Op A   Op B
 *                    |_____|
 *                       |
 *                   Join Result
 * 
 * Scan: Open file, bring it into memory?
 * oScan fileA(inFileA)
 * oSpool spoolA(fileA);
 * oRunPred mPredA(&fileA, colA);
 * oRunPred mPredB(&fileA, colB);
 * 
 * col A          col B
 *   1   <--mPA     3
 *   2              5  <--mPB
 *   4              7
 ***/
void runQryC(string inFileA, string inFileB, int sumFld, int joinColA, int joinColB){
    // // 1) Spool tables a and b
    // oFScan fileA(inFileA);
    // oFScan fileB(inFileB);

    // // 2) Hash join
    // oHashJoin mJoin(&fileA, &fileB);

    // // 3) Addition
    // oStdOp mAggData(&mJoin, maxFxn);
    // mAggData.open();
    // while (mAggData.next());
    // mAggData.close();
}

/*############################
# Helper Functions
############################*/

//Thanks to https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
vector<string> split(const string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


/*############################
# StdOp Functions
############################*/
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

/**
 * single line cartesian join
 * Assumes each input only sends in one item
 ***/
int * singleLineJoin(Operation * me, int * mOut, int * mArgs){
    Operation ** op = me->getUpsOps();

    //Itterate through ops
    int count = 0;
    for (int i = 0; i < mArgs[1]; i++){
        cout << "Running join on op " << i << "\n";
        fflush(stdout);

        int * mInput  = op[i]->next();
        cout << "...got next\n";
        fflush(stdout);
        
        if (mInput){
            //Itterate through tuples in op and add to output
            cout << "...adding to response\n";
            fflush(stdout);
            for (int c = 0; c < op[i]->tSize(); c++){
                cout << "......count " << count << "\n";
                fflush(stdout);
                mOut[count] = mInput[c];
                count++;
            }
        } else {
            cout << "...returning null\n";
            fflush(stdout);
            return nullptr;
        }       
    }    

    if (me->getPrint()) cout << "   join";
    fflush(stdout);
    return mOut;
}


//Count total number of tuples, return count only
//Single next call itterates through all upstream data
int * countFxn(Operation * me, int * mOut, int * mIgnore){
    Operation * op = me->getUpsOp();
    mOut[0] = 0;    
    int * mInput = op->next();
    if (mInput) {
        while (mInput){
            mOut[0] ++;                            
            mInput = op->next();
        }   
        if (op->getPrint()) cout << "   countFxn";
        return mOut;
    }     
    return mInput;
}

//Count total number of tuples, return count only
//Single next call itterates through all upstream data
int * addFxn(Operation * me, int * mOut, int * mIgnore){
    Operation * op = me->getUpsOp();
    mOut[0] = 0;    
    int * mInput = op->next();
    if (mInput) {
        while (mInput){
            mOut[0] += mInput[0];                            
            mInput = op->next();
        }   
        if (op->getPrint()) cout << "   countFxn";
        return mOut;
    }     
    return mInput;
}

//Project tuple to limit rows
//mArgs = tot args, columns we want to project from in to out
int * projectFxn(Operation * me, int * mOut, int * mArgs){    
    Operation * op = me->getUpsOp();
    int * mInput = op->next();    
    if (mInput) {
        for (int i = 0; i < mArgs[0]; i++){            
            //cout << " mArgs[" << i  + ARG_START << "] = " << mArgs[i];
            //cout << " mInput[" << mArgs[i+ARG_START] -1 << "] = " << mInput[mArgs[i] + ARG_START];
            mOut[i] = mInput[mArgs[i+ARG_START] -1];
        }
        if (op->getPrint()) cout << "   prj";
        return mOut;
    }
    return mInput;
}

//Removes incoming 
//mArgs = tot args, col we are checking null on
//args = Column to look at for removal
int * removeNullFxn(Operation * me, int * mOut, int * mArgs){   
    Operation * op = me->getUpsOp();
    int * mInput = op->next();
    while (mInput)
    {
        if (mInput && mInput[mArgs[ARG_START]-1] != M_NULL) break;
        mInput = op->next();
    } 
    if (op->getPrint()) cout << "   rmvNull";
    return mInput;    
}


// //Add tuples together
// int * addFxn(int * mVal, Operation * opp, int * tuple){
//     if (mVal) {
//         for (int i = 0; i < opp->tSize(); i++)
//             tuple[i] += mVal[i];                            
//         return tuple;
//     } 
//     return mVal;
// }

// //Calc max in each row
// int * maxFxn(int * mVal, Operation * opp, int * tuple){
//     if (mVal) {
//         for (int i = 0; i < opp->tSize(); i++)
//             if (mVal[i] > tuple[i]) tuple[i] = mVal[i];
//         return tuple;
//     } 
//     return mVal;
// }

// //Multiplies numerics in tuple by 2
// int * multFxn(int * mVal, Operation * opp){
//     //int * mVal = opp->next();
//     if (mVal){
//         for (int i = 0; i < opp->tSize(); i++)
//             mVal[i] *= 2;        
//     } 
        
//     return mVal;
// }


// int * multFxn(int * mVal, Operation * opp){
    
// }
