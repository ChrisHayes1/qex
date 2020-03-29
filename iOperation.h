#ifndef IOPERATION_H
#define IOPERATION_H

class Operation {
    public:
        virtual int open() = 0;
        virtual int * next() = 0;
        virtual void close() = 0;
        virtual int tSize() = 0;     
        virtual void print(int *, int, const char *) = 0;          
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