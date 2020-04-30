#ifndef IOPERATION_H
#define IOPERATION_H
#include <iostream>

/*############################
# Constants
############################*/
const int M_NULL = -1;
const bool SHOW_SPEED = false;

/*############################
# Abstract operations class
############################*/


class Operation {

    public:
        
        //Virtual Functions
        virtual int open() = 0;
        virtual int * next() = 0;
        virtual void close() = 0;
           

        //Constructor
        Operation(){
            setPrint(false);
        }
        Operation(Operation * mOp, int * mArgs) : Operation{}{
            args = mArgs;            
            numOps = 1;
            Operation * mOps[numOps];
            mOps[0] = mOp;            
            setOps(mOps);
        }
        Operation(Operation ** mOps, int mNumOps, int * mArgs) : Operation{}{
            args = mArgs;   
            numOps = mNumOps;
            setOps(mOps);
        }

        void setOps(Operation ** mOps){
            ops = new Operation * [numOps];
            for (int i = 0; i < numOps; i++){
                ops[i] = mOps[i];
            }
            
            op = ops[0];
        }
        
        //Member Functions
        int getColCount(){ return colCount; }
        void setColSize(int size){ colCount = size;}

        void print(int * mPtr, int size, const char * mStr){
            //printf("about to print with size = %d\n", size);
            printf("%s: ", mStr);
            for (int i = 0; i < size; i++){
                
                if (mPtr[i] != M_NULL) printf("[%*d]", 3, mPtr[i]);     
                else printf("[%*s]", 3, "N");
                fflush(stdout);
            }
            printf("\n");
        }
        Operation * getUpsOp() {return op;}
        Operation ** getUpsOps() {return ops;}
        int getNumOps() {return numOps;}
        void setPrint(bool setPrint) {showPrintout = setPrint;}  
        bool getPrint() {return showPrintout;} 
        
        
    protected:
        //Shared attributes
        Operation ** ops; // List of upstream operators
        Operation * op; // Single upstream operator
        int numOps;
        bool showPrintout; // Show detailed printout
        int colCount;  // Total # of columns in out tuple
        int * args = nullptr;
        
};

#endif

/***
 * Create super classes recognizing key operation archtypes
 *  1) Reduction - Open carries out processing, creates new tuple(s), next just returns pointer
 *      a) Aggregates such as count, containg one 'running cout' tuple that is sent on next
 *      b) Group by and other ops that reduce numerous tuples down.  May be able to run per group instead of on open?
 *      c) Distinct operations? If sorted distinct would not need to run on open.  Could add to hash as you go
 *          and check for duplicates
 *  2) Tuples modified as they come (via next)
 *      a) Tuple sent in from previous op, manipulated, sent on
 *      b) tuple(s) sent in, new tuple sent out (joins)
 *  3) Generation of data - scans
 * 
 ****/