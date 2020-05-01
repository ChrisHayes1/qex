/*######################
 # Spool
 #      On first call generates linked list of tuples, returns tuple it pulls in
 #      Can then rewind, at which point next() starts returning items from
 #      linked list.  
 ######################*/
#include "oSpool.h"
#include <sys/stat.h>
#include <iostream>
#include <chrono> 

using namespace std;
using namespace std::chrono;

/**************
 * Constructor
 *************/
oSpool::oSpool(Operation * mOp) : Operation {mOp, nullptr}{
    //op = mOp;
    
    rewound = false;
    isOpened = false;
}

/**************
 * Interface
 *************/
int oSpool::open(){   
    if (!isOpened){
        bool badOpen = false;
        if (op->open() == -1) badOpen = true;
        setColSize(op->getColCount());
        init_list();
        isOpened = true;

        oStart = high_resolution_clock::now();
        oEnd = high_resolution_clock::now();
        oDuration  = duration_cast<microseconds>(oEnd - oStart);

        if (badOpen){
            return -1;
        }
        return 0;
    }
}



int * oSpool::next(){
    oStart = high_resolution_clock::now();    
    if (!rewound){
         int * temp;
        temp = op->next();
        if (temp){
            int * mTuple = new int [getColCount()];
            for (int c = 0; c < getColCount(); c++){
                mTuple[c] = temp[c];
            }
            if (mTuple && showPrintout) print(mTuple, getColCount(), "-->");
            if (mTuple) enQ(mTuple);
            //set clock to 0            
            oEnd = high_resolution_clock::now();
            oDuration  += duration_cast<microseconds>(oEnd - oStart);
            return mTuple;
        }
        rewind();
        //track run time
        oEnd = high_resolution_clock::now();
        oDuration  += duration_cast<microseconds>(oEnd - oStart);
        return nullptr;
    } else {
        //Rewound
        if (current){
            int * temp = current->tuple;
            current = current->next;
            oEnd = high_resolution_clock::now();
            oDuration  += duration_cast<microseconds>(oEnd - oStart);
            return temp;
        }     
        rewind();
        oEnd = high_resolution_clock::now();
        oDuration  += duration_cast<microseconds>(oEnd - oStart);
        return nullptr;        
    }    
}

void oSpool::close(){    
    if (op){
        close_list(); //Do we need to move?
        op->close();
        op = nullptr;
        if (SHOW_SPEED) cout << "Total duration (Spool) = " << oDuration.count()/1000 << "\n";
    }    
}


/**************
 * Helper
 *************/
void oSpool::rewind(){
    rewound = true;
    current = head;
}

/**************
 * Linked List
 *************/
void oSpool::init_list(){
    head = nullptr;
    end = nullptr;
}

void oSpool::enQ(int * mTuple){
    node * temp = new node;
    temp->tuple = mTuple;
    temp->next = nullptr;
    if (head == nullptr) {
        head = temp;
        end = temp;
    } else {
        end->next = temp;
        end = temp;
    }    
}

void oSpool::close_list(){
    node * current, * next;
    current = head;
    next = current;
    while (current){
        next = current->next;
        delete current->tuple;
        delete current;
        current = next;
    }
}

void oSpool::printsList(){
    node * temp;
    temp = head;        
    while(temp){
        print(temp->tuple, getColCount(), "   tContents");
        temp = temp->next;
    }
}