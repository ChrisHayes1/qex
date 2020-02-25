#ifndef IOPERATION_H
#define IOPERATION_H

class Operation {
    public:
        virtual void open() = 0;
        virtual int * next() = 0;
        virtual void close() = 0;
        virtual int tSize() = 0;     
        virtual void print() = 0;          
};

#endif