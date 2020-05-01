/*############################
# Includes
############################*/
#include <fstream>
#include <string>
#include <chrono> 
#include <time.h>
#include <iostream>
#include <vector> 
#include <sstream>
#include <unistd.h>
#include <list>
#include <unordered_map>

#include "oFScan.h"
#include "oSpool.h"
#include "oGenericOp.h"
#include "iOperation.h"


using namespace std;
using namespace std::chrono; 

/*############################
# Constants
############################*/
const int ARG_START = 1;

/*############################
# Structs
############################*/
//sent into sortFxn
struct sortNode{    
    list<int *> sortList;
    int sortCol;
    int isSorted;
};

//sent into hashAggFxn
struct hashNode{
    unordered_map<string,int *> map;
    unordered_map<string, int *>:: iterator it;
    bool isBuilt = false;    
    bool sumAndCount = false;
    int numGroupCols = 0;
    int * groupCols;
};


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
void runQryCWideSort(string, int, int, int);
void runQryCNarrow(string, int, int, int);
void runQryD4Wide(string, int, int, int);
void runQryD2Wide(string, int, int, int);
void runQryDNarrow(string, int, int, int);
void runQryE6Wide(string, int, int, int);


//Generic Fxns
int * multiLineJoin(Operation *, int *, void *);
int * singleLineJoin(Operation *, int *, void *);
int * pivotFxn(Operation *, int *, void *);
int * unPivotFxn(Operation *, int *, void *);
int * hashDistFxn(Operation *, int *, void *);
int * hashAggFxn(Operation *, int *, void *);
int * hashAggFxnBasic(Operation *, int *, void *);
int * removeDupFxn(Operation *, int *, void *);
int * sortFxn(Operation *, int *, void *);
int * displayFxn(Operation *, int *, void *);
int * countFxn(Operation *, int *, void *);
int * projectFxn(Operation *, int *, void *);
int * removeNullFxn(Operation *, int *, void *);

//Helper Functions
std::vector<std::string> split(const std::string&, char);
void RunMyQry(Operation *, bool);

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

const int MENU_COUNT = 12;

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
const int M_QRY_Db = 9;
const int M_QRY_Dc = 10;
const int M_QRY_E = 11;


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
                                "                 select count (*) as Z from Tbl";
    menu_items[M_QRY_A].usage = "<file name A>";
    menu_items[M_QRY_A].param_count = 1;
    //Run Query 2
    menu_items[M_QRY_B].intro = "QRY B: \n"
                                "                 select \n"
                                "                 count (colA) as X,\n"
                                "                 count (colB) as Y\n"
                                "                 from Tbl";
    menu_items[M_QRY_B].usage = "<file name A> <col a (as int)> <col b (as int)>";
    menu_items[M_QRY_B].param_count = 3;
    //Run Query 3a
    menu_items[M_QRY_Ca].intro = "QRY C (bushy - sort based): \n"
                                "                 select\n"
                                "                 count (distinct A) as X,\n"
                                "                 count (distinct B) as Y\n"
                                "                 count (distinct C) as Z\n"
                                "                 from Tbl";
    menu_items[M_QRY_Ca].usage = "<file name A> <col a (as int)> <col b (as int)> <col c (as int)>";
    menu_items[M_QRY_Ca].param_count = 4;
    //Run Query 3b
    menu_items[M_QRY_Cb].intro = "QRY C (narrow)";
    menu_items[M_QRY_Cb].usage = "<file name A> <col a (as int)> <col b (as int)> <col c (as int)>";
    menu_items[M_QRY_Cb].param_count = 4;
    //Run Query 4
    menu_items[M_QRY_Da].intro = "QRY D (bushy 4 branch) \n"
                                "                 select A,\n"
                                "                 count (B) as W, count (distinct B) as X,\n"
                                "                 count (C) as Y, count (distinct C) as Z\n"
                                "                 from Tbl group by A order by A";
    menu_items[M_QRY_Da].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_Da].param_count = 4;
    //Run Query 4b
    menu_items[M_QRY_Db].intro = "QRY D (bushy 2 branch)";
    menu_items[M_QRY_Db].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_Db].param_count = 4;
    //Run Query 4C
    menu_items[M_QRY_Dc].intro = "QRY D (narrow)";
    menu_items[M_QRY_Dc].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_Dc].param_count = 4;
    //Run Query 5
    menu_items[M_QRY_E].intro = "QRY E (bushy 6 branch) \n"
                                "                 select A,\n"
                                "                 count (*) as U, count (distinct *) as V,\n"
                                "                 count (B) as W, count (distinct B) as X,\n"
                                "                 count (C) as Y, count (distinct C) as Z\n"
                                "                 from tblA group by A order by A";
    menu_items[M_QRY_E].usage = "<file name A> <col A (as int)> <col B (as int)> <col C (as int)>";
    menu_items[M_QRY_E].param_count = 4;
}

void print_menu(){
    cout << "\n*****************************\n";
    cout << "* What would you like to do?\n";
    cout << "*****************************\n";
    cout << "Note: Column references start at 1\n";
    cout << "      Select # only for usage\n";
    cout << "Opt #\n";
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
        //Start tracking function time
        auto start = high_resolution_clock::now();
        

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
                int caColA, caColB, caColC;
                //Verify and pull out int data
                try{
                    caColA = stoi(tokens[2]);
                    caColB = stoi(tokens[3]);
                    caColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryCWideSort(tokens[1], caColA, caColB, caColC);
                break; 
            case M_QRY_Cb:
                int cbColA, cbColB, cbColC;
                //Verify and pull out int data
                try{
                    cbColA = stoi(tokens[2]);
                    cbColB = stoi(tokens[3]);
                    cbColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryCNarrow(tokens[1], cbColA, cbColB, cbColC);
                break;   
            case M_QRY_Da:
                int daColA, daColB, daColC;
                //Verify and pull out int data
                try{
                    daColA = stoi(tokens[2]);
                    daColB = stoi(tokens[3]);
                    daColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryD4Wide(tokens[1], daColA, daColB, daColC);
                break;
            case M_QRY_Db:
                int dbColA, dbColB, dbColC;
                //Verify and pull out int data
                try{
                    dbColA = stoi(tokens[2]);
                    dbColB = stoi(tokens[3]);
                    dbColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryD2Wide(tokens[1], dbColA, dbColB, dbColC);
                break;
            case M_QRY_Dc:
                int dcColA, dcColB, dcColC;
                //Verify and pull out int data
                try{
                    dcColA = stoi(tokens[2]);
                    dcColB = stoi(tokens[3]);
                    dcColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryDNarrow(tokens[1], dcColA, dcColB, dcColC);
                break;
            case M_QRY_E:
                int eaColA, eaColB, eaColC;
                //Verify and pull out int data
                try{
                    eaColA = stoi(tokens[2]);
                    eaColB = stoi(tokens[3]);
                    eaColC = stoi(tokens[4]);
                } catch (...){
                    cout << "Usage: " << menu_items[userChoice].usage << "\n";
                    continue;
                } 
                runQryE6Wide(tokens[1], eaColA, eaColB, eaColC);
                break;         
            default:
                cout << "Invalid input\n";
                break;
        }

        //Stop tracking time and report
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);
        if (SHOW_SPEED) cout << "\nRun Time = " << duration.count()/1000 << " miliseconds\n";
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
    oFScan fileA(inFile); // Scan File
    // Agg results
    oGenericOp countA(&fileA, countFxn, 1, nullptr); 
    //execute tree, show final output
    RunMyQry(&countA, false);
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
    oGenericOp projectAB(&fileA, projectFxn, 2, prjAB);
    sumColA = 1;
    sumColB = 2;    
    //Spool A, B
    oSpool spool(&projectAB);
    
    /*****
     * Branch A
     ****/    
    //Project down to just A
    int prjA [2] = {1, sumColA};
    oGenericOp projectA(&spool, projectFxn, 1, prjA);    
    //Remove null from A
    int rmvA = 1;
    oGenericOp removeNullA(&projectA, removeNullFxn, 1, &rmvA);    
    //Count total number of args in A
    oGenericOp countA(&removeNullA, countFxn, 1, nullptr); 

    /*****
     * Branch b
     ****/
    //Project down to just A
    int prjB [2] = {1, sumColB};
    oGenericOp projectB(&spool, projectFxn, 1, prjB);
    //Remove null from A
    int rmvB = 1;
    oGenericOp removeNullB(&projectB, removeNullFxn, 1, &rmvB);    
    //Count total number of args in B
    oGenericOp countB(&removeNullB, countFxn, 1, nullptr); 

    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    int numArgs = 2; //send in exepcted # of operations
    Operation * joinOps[numArgs] = {&countA, &countB};    
    oGenericOp cartJoin(joinOps, numArgs, singleLineJoin, -1, nullptr);

    //Execute tree and display results
    RunMyQry(&cartJoin, false);
}


void runQryCWideSort(string inFile, int sumColA, int sumColB, int sumColC){
    //Scan in file
    oFScan fileA(inFile); 
    //Project file down to A, B, C
    int prjABC [4] = {3, sumColA, sumColB, sumColC};
    oGenericOp projectABC(&fileA, projectFxn, prjABC[0], prjABC);
    sumColA = 1;
    sumColB = 2;    
    sumColC = 3;  
    //Spool A, B
    oSpool spool(&projectABC);
    
    /*****
     * Branch A
     ****/
    //Project down to just A
    int prjA [2] = {1, sumColA};
    oGenericOp projectA(&spool, projectFxn, 1, prjA);    
    //Remove null from A
    int rmvA = 1; 
    oGenericOp removeNullA(&projectA, removeNullFxn, 1, &rmvA);        
    //In stream duplicate removal
    oGenericOp dupRemoveA(&removeNullA, hashDistFxn, -1, nullptr );    
    //Scalar Count total number of args in A
    oGenericOp countA(&dupRemoveA, countFxn, 1, nullptr); 
    
    /*****
     * Branch b
     ****/    
    //Project down to just A
    int prjB [2] = {1, sumColB};
    oGenericOp projectB(&spool, projectFxn, 1, prjB);
    //Remove null from A
    int rmvB = 1; 
    oGenericOp removeNullB(&projectB, removeNullFxn, 1, &rmvB);  
    //Sort
    int srtB = 1; //is item sorted yet, what col are we sorting on
    //In stream duplicate removal
    oGenericOp dupRemoveB(&removeNullB, hashDistFxn, -1, nullptr );
    //Count total number of args in B
    oGenericOp countB(&dupRemoveB, countFxn, 1, nullptr); 

    /*****
     * Branch C
     ****/    
    //Project down to just A
    int prjC [2] = {1, sumColC};
    oGenericOp projectC(&spool, projectFxn, 1, prjC);
    //Remove null from A
    int rmvC  = 1; 
    oGenericOp removeNullC(&projectC, removeNullFxn, 1, &rmvC);  
    //Sort
    int srtC = 1; //is item sorted yet, what col are we sorting on
    //In stream duplicate removal
    oGenericOp dupRemoveC(&removeNullC, hashDistFxn, -1, nullptr );
    //Count total number of args in B
    oGenericOp countC(&dupRemoveC, countFxn, 1, nullptr); 
    

    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    int numOps = 3;
    Operation * joinOps[numOps] = {&countA, &countB, &countC};
    oGenericOp cartJoin(joinOps, numOps, singleLineJoin, -1, nullptr);
    //Execute tree and display results
    RunMyQry(&cartJoin, false);
}

/**
 * QryC, but narrow grouping plan instead of bushy 
 ***/
void runQryCNarrow(string inFile, int sumColA, int sumColB, int sumColC){
    //Scan in file
    oFScan fileA(inFile); 
    //Project file down to A, B, C
    int prjABC [4] = {3, sumColA, sumColB, sumColC};
    oGenericOp projectABC(&fileA, projectFxn, prjABC[0], prjABC);
    sumColA = 1;
    sumColB = 2;    
    sumColC = 3;  
    
    /****
     * Pivot
     *****/
    int unpiv = 0; //Send in group on rows
    oGenericOp unPivot(&projectABC, unPivotFxn, unpiv + 2, &unpiv);

    //Remove null from A
    int rmvCol  = 2; 
    oGenericOp removeNull(&unPivot, removeNullFxn, -1, &rmvCol);  
    
    //Hash Distinct - duplicate removal
    unordered_map<string, int>  distinctMap;
    oGenericOp hashDist(&removeNull, hashDistFxn, -1, &distinctMap);

    //Hash agg A, B
    // hashNode * mHashNode = new hashNode;
    // unordered_map<string,int *> map = new unordered_map<string,int *>;
    hashNode * mNode = new hashNode;
    mNode->numGroupCols = 1;
    mNode->groupCols = &sumColA;
    oGenericOp hashAgg(&hashDist, hashAggFxn, -1, mNode);    

    //unpivot
    int piv[2] = {1, 0}; //# group cols, group on cols
    oGenericOp pivot(&hashAgg, pivotFxn, 3, piv);
    
    RunMyQry(&pivot, true);
}


/**
 * Qry 20 sans *  4 way branch
 * select a, count (B) as W, count (distinct B) as X,
 *           count (C) as Y, count (distinct C) as Z
 *           from tblA group by A order by A
 * 
 *                        Scan
 *                          |
 *                        Spool
 *     _____________________|_____________________
 *     |             |             |             |
 * Project B      Project B    Project C      Project C
 *     |             |             |             |
 * Filter Null B   Filter Null B   Filter Null C   Filter Null C
 *     |             |             |             |
 *     |      Hash distinct        |         Hash distinct
 *     |             |             |             |
 *  Hash Agg      Hash Agg      Hash Agg      Hash Agg
 *     |_____________|_____________|_____________|
 *                          |
 *                   Multi Row Join on A                               
 ***/
void runQryD4Wide(string inFile, int ColA, int ColB, int ColC){
    //Scan in file
    oFScan fileA(inFile);     
    //Spool (needs all columns due to count distinc *)
    oSpool spool(&fileA);

    /*****
     * Count B
     ****/
    //project down to a, b
    int prjAB [3] = {2, ColA, ColB};
    int ColAMod = 1;
    int ColBMod = 2;
    oGenericOp projectAB(&spool, projectFxn, 2, prjAB);
    //Remove null from B
    oGenericOp removeNullB(&projectAB, removeNullFxn, -1, &ColBMod);  
    //Hash agg A, B
    hashNode * mNode = new hashNode;
    mNode->numGroupCols = 1;
    mNode->groupCols = &ColAMod;
    oGenericOp hashAggB(&removeNullB, hashAggFxn, 2, mNode);

    /*****
     * Count Distinct B
     ****/
    //project down to a, b
    int prjABDist [3] = {2, ColA, ColB};    
    oGenericOp projectABDist(&spool, projectFxn, 2, prjABDist);
    //Remove null from B
    oGenericOp removeNullBDist(&projectABDist, removeNullFxn, -1, &ColBMod);  
    //Hash Distincts
    oGenericOp hashDistB(&removeNullBDist, hashDistFxn, -1, nullptr);
    //Hash agg A, B
    oGenericOp hashAggBDist(&hashDistB, hashAggFxn, 2, mNode);    

    /*****
     * Count C
     ****/
    // project down to A, C
    int prjAC [3] = {2, ColA, ColC};
    int ColCMod = 2;
    oGenericOp projectAC(&spool, projectFxn, 2, prjAC);
    // Remove null from C
    oGenericOp removeNullC(&projectAC, removeNullFxn, 1, &ColCMod);  
    // Hash agg A, B
    oGenericOp hashAggC(&removeNullC, hashAggFxn, 2, mNode);

    /*****
     * Count Distinct C
     ****/
    //project down to a, C
    int prjACDist [3] = {2, ColA, ColC};    
    oGenericOp projectACDist(&spool, projectFxn, 2, prjACDist);
    //Remove null from C
    oGenericOp removeNullCDist(&projectACDist, removeNullFxn, -1, &ColCMod);  
    //Hash Distincts
    oGenericOp hashDistC(&removeNullCDist, hashDistFxn, -1, nullptr);
    //Hash agg A, B
    oGenericOp hashAggCDist(&hashDistC, hashAggFxn, 2, mNode);
    
    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    int numOps = 4;
    Operation * joinOps[numOps] = {&hashAggB, &hashAggBDist, &hashAggC, &hashAggCDist} ;
    int jArgs [3] = {4, 0, ColAMod}; //send in exepcted # of operations
    oGenericOp multiJoin(joinOps, numOps, multiLineJoin, jArgs[0]+1, jArgs);

    list <int *> mySortList;
    sortNode * mSortArgs = new sortNode;
    mSortArgs->sortList = mySortList;
    mSortArgs->sortCol = ColAMod;
    mSortArgs->isSorted = 0;
    oGenericOp sortOut(&multiJoin, sortFxn, -1, mSortArgs);

    //Execute tree and display results
    RunMyQry(&sortOut, true);
}


/**
 * QryC, but 2 branches instead of 4, with duplicate counting
 ***/
void runQryD2Wide(string inFile, int sumColA, int sumColB, int sumColC){
    //Scan in file
    oFScan fileA(inFile);     
    //Spool (needs all columns due to count distinc *)
    oSpool spool(&fileA);

    /*****
     * Count B, B Distinct
     ****/
    //project down to a, b
    int prjAB [3] = {2, sumColA, sumColB};
    int ColAMod = 1;
    int ColBMod = 2;
    oGenericOp projectAB(&spool, projectFxn, 2, prjAB);
    //Remove null from B
    oGenericOp removeNullB(&projectAB, removeNullFxn, -1, &ColBMod);  
    //Hash agg on A B
    int paramAggB[2] = {ColAMod, ColBMod};
    hashNode * mNode = new hashNode;
    mNode->numGroupCols = 2;
    mNode->groupCols = paramAggB;
    oGenericOp hashAggB(&removeNullB, hashAggFxn, 3, mNode);
    //Hash agg on A, but count occurences (COUNT distcint) and sum count (COUNT)
    hashNode * mNodeSum = new hashNode;
    mNodeSum->numGroupCols = 1;
    mNodeSum->sumAndCount = true;
    mNodeSum->groupCols = &ColAMod;
    oGenericOp hashSumCountB(&hashAggB, hashAggFxn, 3, mNodeSum);
    
    /*****
     * Count C, C Distinct
     ****/
    //project down to a, c
    int prjAC [3] = {2, sumColA, sumColC};
    int ColCMod = 2;
    oGenericOp projectAC(&spool, projectFxn, 2, prjAC);
    //Remove null from C
    oGenericOp removeNullC(&projectAC, removeNullFxn, -1, &ColCMod);  
    //Hash agg on A C
    int paramAggC[2] = {ColAMod, ColCMod};
    mNode->groupCols = paramAggC;
    oGenericOp hashAggC(&removeNullC, hashAggFxn, 3, mNode);

    //Hash agg on A, but count occurences (COUNT distcint) and sum count (COUNT)
    oGenericOp hashSumCountC(&hashAggC, hashAggFxn, 3, mNodeSum);


    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    int numOps = 2;
    Operation * joinOps[numOps];
    joinOps[0] = &hashSumCountB;
    joinOps[1] = &hashSumCountC;    
    int jArgs [3] = {2, 0, ColAMod}; //send in exepcted # of operations
    oGenericOp multiJoin(joinOps, numOps, multiLineJoin, 5, jArgs);

    list <int *> mySortList;
    sortNode * mSortArgs = new sortNode;
    mSortArgs->sortList = mySortList;
    mSortArgs->sortCol = ColAMod;
    mSortArgs->isSorted = 0;
    oGenericOp sortOut(&multiJoin, sortFxn, -1, mSortArgs);

    //Execute tree and display results
    RunMyQry(&sortOut, true);
}

/**
 * QryC, but narrow grouping plan instead of bushy 
 ***/
void runQryDNarrow(string inFile, int sumColA, int sumColB, int sumColC){
    //Scan in file
    oFScan fileA(inFile); 
    //Project file down to A, B, C
    int prjABC [4] = {3, sumColA, sumColB, sumColC};
    oGenericOp projectABC(&fileA, projectFxn, prjABC[0], prjABC);
    sumColA = 1;
    sumColB = 2;    
    sumColC = 3;  
    
    /****
     * Pivot
     *****/
    int unpiv[2] = {1, sumColA}; //Send in group on rows
    oGenericOp unPivot(&projectABC, unPivotFxn, unpiv[0] + 2, unpiv);

    // Remove null from A
    int rmvCol  = 3; 
    oGenericOp removeNull(&unPivot, removeNullFxn, -1, &rmvCol);  
    //Hash agg on A B
    int paramAgg[3] = {sumColA, sumColB, sumColC};
    hashNode * mNode = new hashNode;
    mNode->numGroupCols = 3;
    mNode->groupCols = paramAgg;
    oGenericOp hashAgg(&removeNull, hashAggFxn, 4, mNode);
    //Hash agg on A, but count occurences (COUNT distcint) and sum count (COUNT)
    int paramSumCount[2] = {sumColA, sumColB};
    hashNode * mNodeSum = new hashNode;
    mNodeSum->numGroupCols = 2;
    mNodeSum->sumAndCount = true;
    mNodeSum->groupCols = paramSumCount;
    oGenericOp hashSumCount(&hashAgg, hashAggFxn, 4, mNodeSum);
    //unpivot
    int piv[3] = {2, 1, sumColA}; //# group cols, group on cols
    oGenericOp pivot(&hashSumCount, pivotFxn, 5, piv);

    list <int *> mySortList;
    sortNode * mSortArgs = new sortNode;
    mSortArgs->sortList = mySortList;
    mSortArgs->sortCol = sumColA;
    mSortArgs->isSorted = 0;
    oGenericOp sortOut(&pivot, sortFxn, -1, mSortArgs);

    RunMyQry(&sortOut, true);
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
 *  Hash Agg      Hash Agg     Hash Agg      Hash Agg      Hash Agg      Hash Agg
 *     |_____________|_____________|_____________|_____________|_____________|
 *                                       |
 *                                Multi Row Join on A                               
 ***/
void runQryE6Wide(string inFile, int ColA, int ColB, int ColC){
    //Scan in file
    oFScan fileA(inFile);     
    //Spool (needs all columns due to count distinc *)
    oSpool spool(&fileA);

    /*****
     * Count *
     ****/
    // All we need is to run hash aggregation on colA
    hashNode * mNode = new hashNode;
    mNode->numGroupCols = 1;
    mNode->groupCols = &ColA;
    oGenericOp hashAggAll(&spool, hashAggFxn, 2, mNode);
    
    
    /*****
     * Count distinct*
     ****/
    //Hash Distinct
    oGenericOp hashAllDist(&spool, hashDistFxn, -1, nullptr);
    //Hash Aggregate    
    oGenericOp hashAggAllDist(&hashAllDist, hashAggFxn, 2, mNode);

    /*****
     * Count B
     ****/
    //project down to a, b
    int prjAB [3] = {2, ColA, ColB};
    int ColAMod = 1;
    int ColBMod = 2;
    oGenericOp projectAB(&spool, projectFxn, 2, prjAB);
    //Remove null from B
    oGenericOp removeNullB(&projectAB, removeNullFxn, -1, &ColBMod);  
    //Hash agg A, B
    mNode->groupCols = &ColAMod;
    oGenericOp hashAggB(&removeNullB, hashAggFxn, 2, mNode);

    /*****
     * Count Distinct B
     ****/
    //project down to a, b
    int prjABDist [3] = {2, ColA, ColB};    
    oGenericOp projectABDist(&spool, projectFxn, 2, prjABDist);
    //Remove null from B
    oGenericOp removeNullBDist(&projectABDist, removeNullFxn, -1, &ColBMod);  
    //Hash Distincts
    oGenericOp hashDistB(&removeNullBDist, hashDistFxn, -1, nullptr);
    //Hash agg A, B
    oGenericOp hashAggBDist(&hashDistB, hashAggFxn, 2, mNode);

    /*****
     * Count C
     ****/
    //project down to A, C
    int prjAC [3] = {2, ColA, ColC};
    int ColCMod = 2;
    oGenericOp projectAC(&spool, projectFxn, 2, prjAC);
    //Remove null from C
    oGenericOp removeNullC(&projectAC, removeNullFxn, -1, &ColCMod);  
    //Hash agg A, B
    oGenericOp hashAggC(&removeNullC, hashAggFxn, 2, mNode);

    /*****
     * Count Distinct C
     ****/
    //project down to a, C
    int prjACDist [3] = {2, ColA, ColC};    
    oGenericOp projectACDist(&spool, projectFxn, 2, prjACDist);
    //Remove null from C
    oGenericOp removeNullCDist(&projectACDist, removeNullFxn, -1, &ColCMod);  
    //Hash Distincts
    oGenericOp hashDistC(&removeNullCDist, hashDistFxn, -1, nullptr);
    //Hash agg A, B
    oGenericOp hashAggCDist(&hashDistC, hashAggFxn, 2, mNode);
    
    /*****
     * Join A and B
     * May need to think about how this would work with a group by
     * Would this join be different than a join between two tables?
     ****/    
    int numOps = 6;
    Operation * joinOps[numOps];
    joinOps[0] = &hashAggAll;
    joinOps[1] = &hashAggAllDist;
    joinOps[2] = &hashAggB;
    joinOps[3] = &hashAggBDist;
    joinOps[4] = &hashAggC;
    joinOps[5] = &hashAggCDist;
    int jArgs [3] = {6, 0, ColAMod}; //send in exepcted # of operations
    oGenericOp multiJoin(joinOps, numOps,  multiLineJoin, 7, jArgs);

    list <int *> mySortList;
    sortNode * mSortArgs = new sortNode;
    mSortArgs->sortList = mySortList;
    mSortArgs->sortCol = ColAMod;
    mSortArgs->isSorted = 0;

    oGenericOp sortOut(&multiJoin, sortFxn, -1, mSortArgs);

    //Execute tree and display results
    RunMyQry(&sortOut, true);
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


/*******
 * Executes displayResult with mOp as upstream operator
 * @param mOp: upstream operator
 * @param runMultiple: Loop on next.  Not going to lie, should always
 *                      work with while (mResult) but wasn't. This
 *                      was an easy fix.
 *****/
void RunMyQry(Operation * mOp, bool runMultiple){
    oGenericOp displayResult(mOp, displayFxn, -1, nullptr);
    int mResult = displayResult.open();
    if (mResult != -1){
        displayResult.setPrint(true);
        int * mResult;
        mResult = displayResult.next();
        while(mResult && runMultiple){
            mResult = displayResult.next();
        }        
    } 

    displayResult.close();
        
}

/*############################
# Multi Op Functions
############################*/
/**
 * single line cartesian join
 * Assumes each input only sends in one item
 ***/
int * singleLineJoin(Operation * me, int * mOut, void * inArgs){
    int * mArgs = (int *)inArgs;
    Operation ** op = me->getUpsOps();

    //Itterate through ops
    int count = 0;
    for (int i = 0; i < me->getNumOps(); i++){
        int * mInput  = op[i]->next();        
        if (mInput){
            //Itterate through tuples in op and add to output
            for (int c = 0; c < op[i]->getColCount(); c++){
                mOut[count] = mInput[c];
                count++;
            }
        } else {
            return nullptr;
        }       
    }    

    if (me->getPrint()) cout << "   join";
    if (!mOut) if (me->getPrint()) cout << "   join nullptr\n";
    fflush(stdout);
    return mOut;
}


/**
 * Multi line join accross multiple operators
 * Builds a hash table containing each operators output, grouped on colA (mArgs[2])
 * Tracks if table has been build based on mArgs[1]
 * current issue: if a key in colA is missing in first go, then wont exists in subsequent goes
 ****/ 
unordered_map<int, int *> joinMap;
unordered_map<int, int *>:: iterator joinI;

int * multiLineJoin(Operation * me, int * mOut, void * inArgs){        
    int * mArgs = (int *)inArgs;
    /**
     * Build hash table for join
     ****/

    if (mArgs[1] == 0) {
        Operation ** op = me->getUpsOps();
        int numCols = 0, mCount = 0;
        int * mHashNode;
        int * mInput;
        int colA = mArgs[2] - 1; //id which col is col A
        // Identify size of final tuple
        for (int i = 0; i < mArgs[0]; i++){
            numCols += (op[i]->getColCount()-1);            
        }
        numCols ++; //add 1 for group by col

        //Loop through each op, add cols except colA (unless this is first iteration)
        for (int i = 0; i < mArgs[0]; i++){
            //Build one operator at a time
            mInput  = op[i]->next(); 
            //For each incoming row from operators
            while(mInput){
                bool newKey = (joinMap.count(mInput[colA]) == 0);
                //Key needs to be added
                if (newKey){
                    mHashNode = new int [numCols];
                    for (int x = 0; x < numCols; x++){
                        mHashNode[x] = 0;
                    }
                } else {
                    //If key has already been added
                    mHashNode = joinMap[mInput[colA]];
                }                
                //Itterate through tuples in op and add to output
                for (int c = 0; c < op[i]->getColCount(); c++){
                    //Check key and see if it has already been added
                    if (c != colA){ //all cols for first operator, skip colA for rest
                        mHashNode[mCount+c] = mInput[c];
                    } else if (newKey) { //add new key to table
                        mHashNode[c] = mInput[c];
                    }
                }
                //Add node back to hash table
                joinMap[mInput[colA]] = mHashNode;                
                mInput  = op[i]->next();                 
            }        
            mCount += op[i]->getColCount() - 1; //update count            
        }
        //done building hash table
        joinI = joinMap.begin();
        mArgs[1] = 1;

    } else {
        joinI++;
    }

    if (joinI != joinMap.end()){
        if (me->getPrint()) cout << "   hJoin";
        mOut  = joinI->second;
        return mOut;
    }
        
    //Clear map
    joinMap.clear();
    return nullptr;  
}


/*############################
# oGeneric Functions
############################*/

/**
 * Prints out final output
 ***/
int * displayFxn(Operation * me, int * mOut, void * mIgnore){
    Operation * op = me->getUpsOp();    
    me->setPrint(true);    
    int * mInput = op->next();
    if (mInput) {   
        cout << "   $T";
        for (int i = 0; i < me->getColCount(); i++){
            mOut[i] = mInput[i];
        }
        return mOut;
    }         
    me->setPrint(false);
    return nullptr;
}


/**
 * Pivots data
 * May be on a grouping column or columns, or could be
 * all tuples turning into one tuple
 * Col should exist within tuple, this field should be removed
 * And resulting wide tuple should be ordered based on tuple
 * Note: Currently operates of static width.  To pivot in multiple
 * directions you would want something potentially more dynamic.
 * Note: Keys can be anywhere in incoming tuple but will be shifted
 * to front of outgoing tuple in order they are listed in args[2-n]
 * @param args[0]: pivot index
 * @param args [1]: # of grouping columns
 * @param args[2-n]: list of grouping columns
 ***/
unordered_map<string,int *> pivMap;
unordered_map<string, int *>:: iterator pivI;
int pivHashBuilt = 0;

int * pivotFxn(Operation * me, int * mOut, void * inArgs){
    int * mArgs = (int *)inArgs;
    Operation * op = me->getUpsOp();
    int * mHashNode; // Store new tuple in hash table
    int * mInput; // Get upstream tuple
    // 1) Build tuples
    if (pivHashBuilt == 0){
        mInput  = op->next(); 
        // For each incoming tuple
        while(mInput){            
            string mKey = "";
            //Build key (for multi column grouping potential)
            if ( mArgs[1] == 0) mKey = "0";
            for (int i = 0; i < mArgs[1]; i++){
                mKey = mKey + to_string(mInput[i]-1) + " ";
            }
            // If key exists, add to it on index col
            bool newKey = (pivMap.count(mKey) == 0);
            //Key needs to be added
            if (newKey){
                mHashNode = new int [me->getColCount()];
                //Set to 0 because we are alway unpivoting counts
                //But this would be problemeatic if we wanted to pivot\unpivot
                for (int c = 0; c < me->getColCount(); c++){
                    mHashNode[c] = 0;
                }                
                //Go ahead and set the keys                    
                for (int i = 0; i < mArgs[1]; i++){
                    mHashNode[i] = mInput[mArgs[2+i]-1];
                }
            } else {
                //Key already added
                mHashNode = pivMap[mKey];                
            } 

            // Each incoming tuple only contributes to 1 cell in out
            int numGroups = mArgs[1];
            // offset for index is = mInput[mArgs[0]-1]
            int index = mArgs[0]- 1;
            // Column offset is -1
            // Columns being populated = size of incoming tuple - # grouping col - 1 (for index)
            int cursor = 0;
            int pivColCount = op->getColCount() - numGroups - 1;
            for (int i = 0; i < pivColCount; i++){
                cursor = numGroups + (mInput[index] - 1)*pivColCount + i;
                mHashNode[cursor] = mInput[op->getColCount()-(pivColCount-i)];
            }

            //Add node back to hash table
            pivMap[mKey] = mHashNode; 
            //Get next
            mInput  = op->next();
        }

        pivI = pivMap.begin();
        pivHashBuilt = 1;
    } else {
        pivI++;
    }
    // 2) Iterate and return 

    if (pivI != pivMap.end()){
        if (me->getPrint()) cout << "   piv";
        mOut = pivI->second;
        return mOut;
    }
        
    pivMap.clear();
    pivHashBuilt = 0;
    return nullptr; 
}


/***
 * Unpivots
 * @param args[0]: # of grouping columns
 * @param args[1-n]: list of grouping columns
 * Assumes that grouped columns are projected to front of tuple
 * Note:  Currently downstream ops result in single row or group by
 * but if I wanted to be able to unpivot the repivot I would need to add a row
 * on unpivot
 ***/ 
int cursor = 0;
int * current;
int * unPivotFxn(Operation * me, int * mOut, void * inArgs){
    int * mArgs = (int *)inArgs;
    Operation * op = me->getUpsOp();
    //1) Need to pull in next()
    if (cursor == 0) {
        int * mInput = op->next();
        current = mInput; //may cause an issue
        cursor = mArgs[0];
    }
    if (current){
        //Add group on columns.  Note: currently compacts to front
        //For time being assumption is grouping columns are projected to front anyways
        for (int i = 0; i < mArgs[0]; i++){
            mOut[i] = current[mArgs[i] - 1];
        }
        //Add new column    
        mOut[mArgs[0]] = cursor+1-mArgs[0];
        mOut[mArgs[0]+1] = current[cursor];
        cursor++;
        if ( cursor > (me->getColCount() - mArgs[0])) cursor = 0;
        if (me->getPrint()) cout << "   unpiv";
        return mOut;
    }
    cursor = 0;
    return nullptr;
}


/**
 * Sorted DISTINCT approach
 * Remove Duplicates from stream
 * Duplicates based on entire tuple
 * Expects that upstream op is returning sorted tuples
 * dupPrev tracks previous tuple, as point of comparison
 * and updates to new tuple after comparison
 * TODO: Shouldn't have to loop through minput twice
 * and can break as soon as I find a difference in tuple
 ***/
int * dupPrev = nullptr;
int * removeDupFxn(Operation * me, int * mOut, void * mIgnore){
    Operation * op = me->getUpsOp(); 
    int * mInput = op->next();

    while (mInput){
        bool tMatch = !(dupPrev == nullptr);
        for (int i = 0; i < op->getColCount() && tMatch; i++)
            tMatch &= (dupPrev[i] == mInput[i]);            
        if (!tMatch) { // Unique
            for (int i = 0; i < op->getColCount(); i++){
                mOut[i] = mInput[i];
                dupPrev = mOut;
            }
            if (me->getPrint()) cout << "   dRem";
            return mOut;
        }
        mInput = op->next();
    }
    dupPrev = nullptr;
    return nullptr;         
}


/**
 * Hash based DISTINCT approach
 * Removes duplicates from stream sans sort
 * distinctMap stores distinct list for growing comparison
 * Distinct is based on entire tuple, not specific column
 ****/
unordered_map<string, int> distinctMap;

int * hashDistFxn(Operation * me, int * mOut, void * mIgnore){ 
    Operation * op = me->getUpsOp();
    string mKey = "";
    int * mInput;

    do{        
        mInput = op->next();                
        if (mInput){
            mKey = "";            
            for (int i = 0; i < op->getColCount(); i++){
                mKey = mKey + to_string(mInput[i]) + " ";        
            }                        
        } else {
            distinctMap.clear();
            return nullptr;
        }    
    } while (distinctMap.count(mKey) != 0);
    distinctMap[mKey];
    if (me->getPrint()) cout << "   hDist";
    return mInput;
}


/**
 * Aggregates based on a grouping column.  Currently based on
 * using cheat-key approach used in hash distinct (hashDistFxn)
 * 
 * @param mArgs[0]: # of Grouping Column
 * @param mArgs[1]: Sum and Count - True modifies into Grouped sum and count
 * @param mArgs[2-n]: Grouping Columns
 ***/
int * hashAggFxn(Operation * me, int * mOut, void * inArgs){ 
    hashNode * mHash = (hashNode *) inArgs;
    Operation * op = me->getUpsOp();    
    string mKey = "";
    int * mHashNode; // Store new tuple in hash table
    //I hash is not built yet, must build first
    if (!mHash->isBuilt) {        
        //Build hash table
        while (true){
            int * mInput = op->next();
            if (mInput){                
                mKey = "";
                for (int i = 0; i < mHash->numGroupCols; i++){
                    mKey = mKey + to_string(mInput[i]) + " ";        
                } 
                // If key exists, add to it on index col
                bool newKey = (mHash->map.count(mKey) == 0);
                //Key needs to be added
                if (newKey){
                    mHashNode = new int [me->getColCount()];
                    //Set to null in case we don't have a value
                    for (int c = 0; c < me->getColCount(); c++){
                        mHashNode[c] = 0;
                    }                
                    //Go ahead and set the keys                    
                    for (int i = 0; i < mHash->numGroupCols; i++){
                        mHashNode[i] = mInput[mHash->groupCols[i]-1];
                    } 
                    // Set initial values
                    mHashNode[me->getColCount()-1] = 1;
                    if (mHash->sumAndCount) mHashNode[me->getColCount()-2] = mInput[op->getColCount()-1];
                } else {
                    //Key already added
                    mHashNode = mHash->map[mKey];                                    
                    mHashNode[me->getColCount()-1]++;
                    if (mHash->sumAndCount) mHashNode[me->getColCount()-2] += mInput[op->getColCount()-1];
                } 
                mHash->map[mKey] = mHashNode;                
            } else {
                break;
            }            
        }
        mHash->it = mHash->map.begin();
        mHash->isBuilt = true;
    } else {
        mHash->it++;
    }

    //Return next pair
    if (mHash->it != mHash->map.end()){
        if (me->getPrint() && !mHash->sumAndCount) cout << "   hAgg";
        if (me->getPrint() && mHash->sumAndCount) cout << "   hSuC";
        
        mOut = mHash->it->second;
        return mOut;
    }
        
    mHash->map.clear();
    mHash->isBuilt = false;
    return nullptr;    
}


/**
 * Basic sort function.  Uses built in c++ sort alg
 * 
 * TODO: Seems like I should be able to do this without re-allocating memory
 * I am alreayd building a structure in the spool, so that is what we should be sorting...
 * 
 * @param inArgs[0]: contains sortNode
 *          sortNode->sortList = list we are sorting
 *          sortNode->sortCol = col we are sorting on
 *          sortNode->isSorted = Has this list been sorted yet
 ***/
int * sortFxn(Operation * me, int * mOut, void * inArgs){ 
    //Extract params
    sortNode * mSortArgs = (sortNode *) inArgs;
    Operation * op = me->getUpsOp();    
    
    //I list is not built yet, must build first
    //Sort must consume all tuples prior to releasing output
    if (mSortArgs->isSorted == 0)    {
        while (true){
            int * mInput = new int [op->getColCount()];            
            int * temp = op->next();            
            if (temp){
                for (int i = 0; i < op->getColCount(); i++){
                    mInput[i] = temp[i];
                }
                mSortArgs->sortList.push_back(mInput);
            } else {
                break;
            }
        }

        //Sort
        int sortCol = mSortArgs->sortCol - 1;
        mSortArgs->sortList.sort([sortCol](int * first, int * second){
            return (first[sortCol] < second[sortCol]);
        });
 
        mSortArgs->isSorted = 1;    
    }    
    
    //items should be sorted now
    if (!mSortArgs->sortList.empty()){
        if (me->getPrint()) cout << "   sort";
        int * mOut = mSortArgs->sortList.front();
        mSortArgs->sortList.pop_front();
        return mOut;
    }

    //pop_front should remove elements
    //delete sortList;
    mSortArgs->sortList.clear();
    mSortArgs->isSorted = 0;
    return nullptr;
}


/****
 *  Count total number of tuples, return count only
 * Single next call itterates through all upstream data
 ****/
int * countFxn(Operation * me, int * mOut, 
                            void * mIgnore){
    Operation * op = me->getUpsOp();
    mOut[0] = 0;    
    int * mInput = op->next();
    if (mInput) {
        while (mInput){
            mOut[0] ++;                            
            mInput = op->next();
        }   
        if (me->getPrint()) cout << "   count";
        fflush(stdout);
        return mOut;
    }     
    return nullptr;
}

/***
 * Project to subset of rows
 * @param args[0]: Number of rows we are projecting down to
 * @param args[1-n]: Columns to include, in order, in the final projection
 ****/
int * projectFxn(Operation * me, int * mOut, void * inArgs){    
    int * mArgs = (int *)inArgs;
    Operation * op = me->getUpsOp();
    int * mInput = op->next();    
    if (mInput) {
        for (int i = 0; i < mArgs[0]; i++){            
            mOut[i] = mInput[mArgs[i+1] -1];
        }
        if (me->getPrint()) cout << "   prj";
        fflush(stdout);
        return mOut;
    }
    return nullptr;
}


/**
 * Checks single column for null value, if present
 * does not pass downstream
 * @param args[]: Column we are checking for null
 * @return: Upstream op, if args[1ks] is not null
 ***/
int * removeNullFxn(Operation * me, int * mOut, void * inArgs){   
    int * mArgs = (int *)inArgs;
    Operation * op = me->getUpsOp();
    int * mInput = op->next();
    while (mInput)
    {
        if (mInput && mInput[mArgs[0]-1] != M_NULL) break;
        mInput = op->next();
    } 
    if (me->getPrint()) cout << "   rmvNull";
    fflush(stdout);
    return mInput;    
}
