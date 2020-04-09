/*######################
 # Spool
 #      On first call generates linked list of tuples, returns tuple it pulls in
 #      Can then rewind, at which point next() starts returning items from
 #      linked list.  
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
    setPrint(false);
    rewound = false;
}

/**************
 * Interface
 *************/
int oSpool::open(){   
    if (!rewound){
        op->open();
        colSize = op->tSize();
        init_list();
    }
}



int * oSpool::next(){
    //The issue seems to be that temp is creating a pointer to the tuple it is getting
    //sent, but the tuple it is getting sent keeps changing.  So all the downstream
    //pointers are pointing to the same stack memory in the previous app.  I need
    //to transfer the filestream data to its own location in the heap
    if (!rewound){
         int * temp;
        temp = op->next();

        //
        if (temp){
            int * mTuple = new int [colSize];
            for (int c = 0; c < colSize; c++){
                mTuple[c] = temp[c];
            }
            if (mTuple && showPrintout) print(mTuple, colSize, "-->");
            if (mTuple) enQ(mTuple);
            //print(mTuple, colSize, "   tContents");
            
            return mTuple;
        }
        return nullptr;
    } else {
        //Rewound
        if (current){
            int * temp = current->tuple;
            current = current->next;
            return temp;
        }        

        return nullptr;        
    }
   

    //
    
}

void oSpool::close(){    

    //printsList();
    if (op){
        close_list();
        op->close();
        op = nullptr;
    }
    
}

int oSpool::tSize(){
    return colSize;
}

//Returns upstream operator
Operation * oSpool::getUpsOp(){
    return op;
}

bool oSpool::getPrint(){
    return showPrintout;
}

void oSpool::setPrint(bool sPrint){
    showPrintout = sPrint;
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
        print(temp->tuple, colSize, "   tContents");
        temp = temp->next;
    }
}