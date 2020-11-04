#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <math.h>
#include <random>
using namespace std;

///////////////////
// Static Params //
///////////////////
// Name
static std::string nameProgram = "-";

// File Paths
static std::string pathInitConfig = "-";
static std::string pathOutFile = "-";

// Math values


/////////////
// Structs //
/////////////
struct force {
    double x;
    double y;
};

///////////////
// Functions //
///////////////
force createForce(double x, double y){
    force m;
    m.x = x;
    m.y = y;
    return m;
}

string truncateDouble(double number){
}

void wrongMessage(){
    exit(-1);
}

unsigned int* checkArguments(int argc, char **arguments){
    unsigned int* argsInt = new unsigned int[4];

    return argsInt;
}

double distance(star starA, star starB){
}


///////////
// MAIN //
//////////
int main (int argc, char** argv) {
    // Check Number of Arguments
    //     argv[1] -> 
    //     argv[2] -> 
    //     argv[3] -> 
    //     argv[4] -> 

    checkNumberArguments(argc);

    return 0;
}
