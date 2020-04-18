/************************
 * TODO
 ************************
 * 1) Merge oJoin and oStdOp, I should be able to do this with one class
 * 2) Is there a way to remove the size fxn?  Only used on projection?
 ************************


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
#include <list>
#include <unordered_map>

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
int genData(string, int, int, int, int, string, int);
int readData(string); //print out data file
int summarizeData(string);

//Menu
void init_menu();
void print_menu();
void menu();

//Queries
void runQryA(string);
void runQryB(string, int, int);
void runQryCWide(string, int, int);
void runQryD6Wide(string, int, int, int);

// void runQryD(string, string, int, int, int);

//Join Functions
int * singleLineJoin(Operation *, int *, int *);

//StOp Fxns

int * hashDistFxn(Operation *, int *, int *);
int * hashAggFxn(Operation *, int *, int *);
int * removeDupFxn(Operation *, int *, int *);
int * sortFxn(Operation *, int *, int *);
int * displayFxn(Operation *, int *, int *);
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

const int MENU_COUNT = 10;

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
const int M_QRY_Ca = 6;
const int M_QRY_Cb = 7;
const int M_QRY_Da = 8;
const int M_QRY_Dc = 9;


void init_menu(){
    //Generate menu
    //Exit
    menu_items[M_EXIT].intro = "Exit Program";
    menu_items[M_EXIT].usage = "";
    menu_items[M_EXIT].param_count = 0;
    //Generate data files
    menu_items[M_GEN_DATA].intro = "Generate Data file";
    menu_items[M_GEN_DATA].usage = "<file_name> <num_rows> <num_cols> <max_value> <max_value_col_A (-1 to ignore)> <set_as_null (-1 to ignore)>";
    menu_items[M_GEN_DATA].param_count = 6;
    //Read data files
    menu_items[M_READ_DATA].intro = "Print Data file   ";
    menu_items[M_READ_DATA].usage = "<file_name>";
    menu_items[M_READ_DATA].param_count = 1;
    //Summarize data files
    menu_items[M_SUMMARIZE_DATA].intro = "Sumarize Data file   ";
    menu_items[M_SUMMARIZE_DATA].usage = "<file_name>";
    menu_items[M_SUMMARIZE_DATA].param_count = 1;
    //Run Query 1
    menu_items[M_QRY_A].intro = "QRY A: \n"
                                "                 select count (*) as Z from T";
    menu_items[M_QRY_A].usage = "<file name A>";
    menu_items[M_QRY_A].param_count = 1;
    //Run Query 2
    menu_items[M_QRY_B].intro = "QRY B: \n"
                                "                 select \n"
                                "                 count (colA) as X,\n"
                                "                 count (colB) as Y\n"
                                "                 from tblA";
    menu_items[M_QRY_B].usage = "<file name A> <col a (as int)> <col b (as int)>";
    menu_items[M_QRY_B].param_count = 3;
    //Run Query 3a
    menu_items[M_QRY_Ca].intro = "QRY C (bushy): \n"
                                "                 select\n"
                                "                 count (distinct A) as X,\n"
                                "                 count (distinct B) as Y\n"
                                "                 from tblA";
    menu_items[M_QRY_Ca].usage = "<file name A> <col a (as int)> <col b (as int)>";
    menu_items[M_QRY_Ca].param_count = 3;
    //Run Query 3b
    menu_items[M_QRY_Cb].intro = "QRY C (narrow)";
    menu_items[M_QRY_Cb].usage = "<file name A> <col a (as int)> <col b (as int)>";
    menu_items[M_QRY_Cb].param_count = 3;
    //Run Query 4
    menu_items[M_QRY_Da].intro = "QRY D (bushy 6 branch) \n"
                                "                 select A,\n"
                                "                 count (*) as U, count (distinct *) as V,\n"
                                "                 count (B) as W, count (distinct B) as X,\n"
                                "                 count (C) as Y, count (distinct C) as Z\n"
                                "                 from tblA group by A order by A";
    menu_items[M_QRY_Da].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_Da].param_count = 4;
    menu_items[M_QRY_Dc].intro = "QRY D (narrow)";
    menu_items[M_QRY_Dc].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_Dc].param_count = 4;
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
                int rows, cols, max, maxA, set_as_null; 
                //Verify and pull out int data
                try{
                    rows = stoi(tokens[2]);
                    cols = stoi(tokens[3]);
                    max = stoi(tokens[4]);
                    maxA = stoi(tokens[5]);
                    set_as_null = stoi(tokens[6]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                }           
                genData(tokens[1],rows, cols, max, maxA, menu_items[userChoice].usage, set_as_null);
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
                int sumColA, sumColB;
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
            case M_QRY_Ca:
                int sumColC, sumColD;
                //Verify and pull out int data
                try{
                    sumColC = stoi(tokens[2]);
                    sumColD = stoi(tokens[3]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryCWide(tokens[1], sumColC, sumColD);
                break;   
            case M_QRY_Da:
                int daColA, daColB, daColC;
                //Verify and pull out int data
                try{
                    daColA = stoi(tokens[2]);
                    daColB = stoi(tokens[3]);
                    daColC = stoi(tokens[3]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryD6Wide(tokens[1], daColA, daColB, daColC);
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
int genData(string fileName, int rows, int cols, int maxSize, int maxSizeColA, string usage, int set_as_null){
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
            if (y == 0 && maxSizeColA > -1) { num = rand()%maxSizeColA; }
            else {num = rand()%maxSize;}
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
    oFScan fileA(inFile); //Scan File
    oSpool spoolA(&fileA); //Spool results
    //count all rows
    //updastream op, fxn to run, size of new tuple, no args
    oStdOp countA(&spoolA, countFxn, 1, nullptr); 

    //execute tree, show final output
    oStdOp displayResult(&countA, displayFxn, -1, nullptr);
    displayResult.open();
    displayResult.next();
    displayResult.close();
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
 *                 Single Row Join
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
    //Count total number of args in A
    oStdOp countA(&removeNullA, countFxn, 1, nullptr); 

    /*****
     * Branch b
     ****/
    //Project down to just A
    int prjB [2] = {1, sumColB};
    oStdOp projectB(&spool, projectFxn, 1, prjB);
    //Remove null from A
    int rmvB [3] = {1, 1};
    oStdOp removeNullB(&projectB, removeNullFxn, 1, rmvB);    
    //Count total number of args in B
    oStdOp countB(&removeNullB, countFxn, 1, nullptr); 

    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    Operation * joinOps[2];
    joinOps[0] = &countA;
    joinOps[1] = &countB;
    int jArgs [2] = {1, 2}; //send in exepcted # of operations
    oJoin cartJoin(joinOps, singleLineJoin, -1, jArgs);
    //Execute tree and display results
    oStdOp displayResult(&cartJoin, displayFxn, -1, nullptr);
    displayResult.open();
    displayResult.next();
    displayResult.close();
}


/**
 * Qry 15 in paper
 * SELECT COUNT (DISTINCT colA) as X, COUNT (DISTINCT colB) as Y from tblA
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
 *           Sort A              Sort B
 *             |                    |
 *    In Stream distinct A   In Stream distinct B
 *             |                    |
 *     Scalar Counting A       Saclar Counting B
 *             |____________________|
 *                       |
 *                   Single Row Join
 ***/
void runQryCWide(string inFile, int sumColA, int sumColB){
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
    //Sort
    int srtA [3] = {2, 0, 0}; //1 arg, bool is sorted? if 0 then sort first
    oStdOp sortA(&removeNullA, sortFxn, -1, srtA);
    //In stream duplicate removal
    oStdOp dupRemoveA(&sortA, removeDupFxn, -1, nullptr );
    //dupRemoveA.setPrint(true);    
    //Scala Count total number of args in A
    oStdOp countA(&dupRemoveA, countFxn, 1, nullptr); 
    //countA.setPrint(true);  
    
    

    /*****
     * Branch b
     ****/    
    //Project down to just A
    int prjB [2] = {1, sumColB};
    oStdOp projectB(&spool, projectFxn, 1, prjB);
    //Remove null from A
    int rmvB [3] = {1, 1};
    oStdOp removeNullB(&projectB, removeNullFxn, 1, rmvB);  
    //Sort
    int srtB [3] = {2, 0, 0}; //2 arg, bool is sorted? if 0 then sort first, sort col
    oStdOp sortB(&removeNullB, sortFxn, -1, srtB);
    //In stream duplicate removal
    oStdOp dupRemoveB(&sortB, removeDupFxn, -1, nullptr );
    //dupRemoveB.setPrint(true);      
    //Count total number of args in B
    oStdOp countB(&dupRemoveB, countFxn, 1, nullptr); 
    //countB.setPrint(true);      

    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    Operation * joinOps[2];
    joinOps[0] = &countA;
    joinOps[1] = &countB;
    int jArgs [2] = {1, 2}; //send in exepcted # of operations
    oJoin cartJoin(joinOps, singleLineJoin, -1, jArgs);
    //Execute tree and display results
    oStdOp displayResult(&cartJoin, displayFxn, -1, nullptr);
    //oStdOp displayResult(&countA, displayFxn, -1, nullptr);
    displayResult.open();
    displayResult.next();
    displayResult.close();
}


/**
 * Qry 20 (wide, fig 3) in paper - 6 way branch
 * select a, count (*) as U, count (distinct *) as V,
 *           count (B) as W, count (distinct B) as X,
 *           count (C) as Y, count (distinct C) as Z
 *           from tblA group by A order by A
 * 
 *                                     Scan
 *                                       |
 *                                     Spool
 *      _________________________________|___________________________________
 *     |             |             |             |             |             |
 *     |             |         Project B      Project B    Project C      Project C
 *     |             |             |             |             |             |
 *     |             |     Filter Null B   Filter Null B   Filter Null C   Filter Null C
 *     |             |             |             |             |             |
 *     |       Hash Distinct       |      Hash distinct        |         Hash distinct
 *     |             |             |             |             |             |
 *  Hash Agg       Hash Agg    Hash Agg      Hash Agg      Hash Agg      Hash Agg
 *     |_____________|_____________|_____________|_____________|_____________|
 *                                       |
 *                                Multi Row Join on A                               
 ***/
void runQryD6Wide(string inFile, int ColA, int ColB, int colC){
    //Scan in file
    oFScan fileA(inFile);     
    //Spool (needs all columns due to count distinc *)
    oSpool spool(&fileA);

    /*****
     * Count *
     ****/
    // All we need is to run hash aggregation on colA
    int hashAllAgg [3] = {3, 0, ColA}; //1 arg, bool is sorted? if 0 then sort first
    oStdOp hashAggAll(&spool, hashAggFxn, 2, hashAllAgg);
    
    // hashAggAll.open();
    // hashAggAll.next();
    // hashAggAll.close();
    /*****
     * Count distinct*
     ****/
    //Hash Distincts
    int hashAllDistVar [3] = {3, 0, ColA}; //1 arg, bool is sorted? if 0 then sort first
    oStdOp hashAllDist(&spool, hashDistFxn, -1, nullptr);
    //Hash Aggregate
    int hashAllAggDist [3] = {3, 0, ColA}; //1 arg, bool is sorted? if 0 then sort first
    oStdOp hashAggAllDist(&hashAllDist, hashAggFxn, 2, hashAllAggDist);

    /*****
     * Count B
     ****/
    //project down to a, b
    int prjAB [3] = {2, ColA, ColB};
    ColA = 1;
    ColB = 2;
    oStdOp projectAB(&spool, projectFxn, 2, prjAB);
    projectAB.setPrint(true);
    //Remove null from B
    int rmvB [3] = {1, ColB};
    oStdOp removeNullB(&projectAB, removeNullFxn, 1, rmvB);  
    //Hash agg A, B
    int hashBAll [3] = {3, 0, ColA}; //1 arg, bool is sorted? if 0 then sort first
    oStdOp hashAggB(&removeNullB, hashAggFxn, 2, hashBAll);

    /*****
     * Count Distinct B
     ****/

    /*****
     * Count C
     ****/

    /*****
     * Count Distinct C
     ****/

    /*****
     * Join
     ****/

    //Execute tree and display results
    oStdOp displayResult(&hashAggB, displayFxn, -1, nullptr);
    //oStdOp displayResult(&countA, displayFxn, -1, nullptr);
    displayResult.open();
    while(displayResult.next());
    displayResult.close();
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
# Multi Op Functions
############################*/
/**
 * single line cartesian join
 * Assumes each input only sends in one item
 ***/
int * singleLineJoin(Operation * me, int * mOut, int * mArgs){
    Operation ** op = me->getUpsOps();

    //Itterate through ops
    int count = 0;
    for (int i = 0; i < mArgs[1]; i++){
        // cout << "working through operator " << i << "\n";
        // fflush(stdout);
        int * mInput  = op[i]->next();        
        if (mInput){
            //Itterate through tuples in op and add to output
            for (int c = 0; c < op[i]->tSize(); c++){

                mOut[count] = mInput[c];
                count++;
            }
        } else {
            return nullptr;
        }       
    }    

    if (me->getPrint()) cout << "   join";
    fflush(stdout);
    return mOut;
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
 * Simply displays output
 ***/
int * displayFxn(Operation * me, int * mOut, int * mIgnore){
    Operation * op = me->getUpsOp();
    me->setPrint(true);
    int * mInput = op->next();
    if (mInput) {   
        cout << "   $T";
        for (int i = 0; i < me->tSize(); i++){
            mOut[i] = mInput[i];
        }
        return mOut;
    }     
    me->setPrint(false);
    return mInput;
}

/**
 * Remove Duplicates
 ***/
int * dupPrev = nullptr;

int * removeDupFxn(Operation * me, int * mOut, int * mIgnore){
    Operation * op = me->getUpsOp(); 
    int * mInput = op->next();
    if (mInput){
        if (dupPrev == nullptr){
            //cout << "about to generate new dupPrev\n";
            //fflush(stdout);
            dupPrev = new int [op->tSize()];
            //cout << "about to build dupPrev\n";
            //fflush(stdout);
            for (int i = 0; i < op->tSize(); i++){
                dupPrev[i] = mInput[i];
                mOut[i] = mInput[i];
            }
            //cout << "about to return input dupPrev\n";
            //fflush(stdout);
            return mOut;
        } 

        while (true && mInput){
            bool tMatch = true;
            for (int i = 0; i < op->tSize(); i++){
                //cout << "comparing dupPrev[i] (" << dupPrev[i] << ") to mInput[i] (" << mInput[i] << ")\n";
                //fflush(stdout);
                tMatch &= (dupPrev[i] == mInput[i]);
                mOut[i] = mInput[i];
            }
            //cout << "tMatch = " << tMatch << "\n";
            if (!tMatch) { // Unique
                for (int i = 0; i < op->tSize(); i++){
                    dupPrev[i] = mInput[i];
                }
                return mOut;
            }
            mInput = op->next();
        }
    }

    //cout << "removeDup about to return null ptr\n";
    return nullptr;
    
         
}


unordered_map<string, int> distinctMap;
/**
 * Identify distinct with has function.  This is potentially problematic for multi-key rows
 ****/

int * hashDistFxn(Operation * me, int * mOut, int * mArgs){ 
    Operation * op = me->getUpsOp();
    string mKey;
    int * mInput;
    do{
        mInput = op->next();
        if (mInput){
            mKey = "";
            for (int i = 0; i < op->tSize(); i++){
                mKey += mInput[i] + " ";        
            }
        } else {
            return nullptr;
        }        
    } while (distinctMap.count(mKey) != 0);

    distinctMap[mKey];
    if (me->getPrint()) cout << "   hDist";
    return mInput;
}

/**
 * Hash Aggregate implementation
 * Not comming in sorted. So similar to sortFxn, I need
 * to build hash table on first round, and then can 
 * return entry by entry until we run out
 ***/
unordered_map<int,int> aggMap;
unordered_map<int, int>:: iterator i;

int * hashAggFxn(Operation * me, int * mOut, int * mArgs){ 
    Operation * op = me->getUpsOp();    
    //I hash is not built yet, must build first
    if (mArgs[1] == 0) {
        //Build hash table
        while (true){
            int * mInput = op->next();
            if (mInput){
                int mapKey = mArgs[2] - 1;
                aggMap[mInput[mapKey]]++;
            } else {
                break;
            }
            
        }
        i = aggMap.begin();
        mArgs[1] = 1; 
    } else {
        i++;
    }

    //Return next pair
    if (i != aggMap.end()){
        if (me->getPrint()) cout << "   hAgg";
        mOut[0] = i->first;
        mOut[1] = i->second;
        return mOut;
    }
        
    aggMap.clear();
    return nullptr;    
}

/**
 * Basic sort function
 * TODO: Seems like I should be able to do this without re-allocating memory
 * I am alreayd building a structure in the spool, so that is what we should be sorting...
 ***/
list <int *> sortList;

int * sortFxn(Operation * me, int * mOut, int * mArgs){ 
    Operation * op = me->getUpsOp();    

    //I list is not built yet, must build first
    //Sort must consume all tuples prior to releasing output
    if (mArgs[1] == 0)    {
        // cout << "Building List\n";
        // fflush(stdout);
        
        while (true){
            int * mInput = new int [op->tSize()];            
            int * temp = op->next();            
            if (temp){
                for (int i = 0; i < op->tSize(); i++){
                    mInput[i] = temp[i];
                }
                //cout << "pushing " << mInput[0] << "\n";
                sortList.push_back(mInput);
            } else {
                break;
            }
            //cout << "Peek at first, last " << sortList.front()[0] << " " << sortList.back()[0] << "\n";
        }

        //Sort
        int sortCol = mArgs[2];
        sortList.sort([sortCol](int * first, int * second){
            return (first[sortCol] < second[sortCol]);
        });

        mArgs[1] = 1;        
    }    
    

    //items should be sorted now
    if (!sortList.empty()){
        if (me->getPrint()) cout << "   sort";
        int * mOut = sortList.front();
        sortList.pop_front();
        return mOut;
    }

    //pop_front should remove elements
    //delete sortList;
    sortList.clear();
    return nullptr;
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
        if (me->getPrint()) cout << "   countFxn";
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
        if (me->getPrint()) cout << "   addFxn";
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
        if (me->getPrint()) cout << "   prj";
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
    if (me->getPrint()) cout << "   rmvNull";
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
