/*######################
 # Multiplies each tuple by 2
 ######################*/

// #include <fstream>
// #include <iostream>
#include "oSpool.h"
#include <sys/stat.h>

/**************
 * Constructor
 *************/
oSpool::oSpool(Operation * mOp){
    op = mOp;
}

/**************
 * Interface
 *************/
int oSpool::open(){    
    op->open();
    colSize = op->tSize();
    init_list();
}



int * oSpool::next(){
    //The issue seems to be that temp is creating a pointer to the tuple it is getting
    //sent, but the tuple it is getting sent keeps changing.  So all the downstream
    //pointers are pointing to the same stack memory in the previous app.  I need
    //to transfer the filestream data to its own location in the heap
    int * temp;
    temp = op->next();

    //
    if (temp){
        int * mTuple = new int [colSize];
        for (int c = 0; c < colSize; c++){
            mTuple[c] = temp[c];
        }
        if (mTuple) enQ(mTuple);
        //print(mTuple, colSize, "   tContents");
        return mTuple;
    }
    return nullptr;

    //
    
}

void oSpool::close(){    

    //printsList();

    close_list();

    op->close();
}

int oSpool::tSize(){
    return colSize;
}

/**************
 * Helper
 *************/
void oSpool::print(int * mPtr, int size, const char * mStr){
    fflush(stdout);
    for (int i = 0; i < size; i++){
        printf("[%*d]", 3, mPtr[i]);            
        fflush(stdout);
    }
    printf("\n");
}

void oSpool::rewind(int colNum){
    
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
        print(temp->tuple, colSize, "   tContents");
        temp = temp->next;
    }
}