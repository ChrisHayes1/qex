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
        //Ops with single upstream op
        Operation(Operation * mOp, void * mArgs) : Operation{}{
            args = mArgs;            
            numOps = 1;
            Operation * mOps[numOps];
            mOps[0] = mOp;            
            setOps(mOps);
        }
        //Ops with multiple upstream ops (such as joins);
        Operation(Operation ** mOps, int mNumOps, void * mArgs) : Operation{}{
            args = mArgs;   
            numOps = mNumOps;
            setOps(mOps);
        }

        //set upstream op or ops
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
        int numOps; // Number of upstream ops that feed into me
        bool showPrintout; // Show detailed printout
        int colCount;  // Total # of columns in out tuple
        void * args = nullptr;
        
};

#endif
