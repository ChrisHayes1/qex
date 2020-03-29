#ifndef M_STATIC_H
#define M_STATIC_H

#include <string>

using namespace std;

class mStatic{
    public:
        static void test();
        static int genData(string, int, int, string); //generate new data
        static int readData(string); //print out data file
};


#endif