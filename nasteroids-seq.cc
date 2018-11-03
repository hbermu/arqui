#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Static Params
static std::string nameProgram = "nasteroids-seq";

/*--------------------------------------------
  Print wrong arguments and the correct use
  of the sofware
--------------------------------------------*/
void wrongMessage(){
    cout    << nameProgram << ": Wrong arguments.\n"
            << "Correct use:\n"
            << nameProgram << " num_asteroides num_iteraciones num_planetas semilla\n";
    exit(-1);
}

/*--------------------------------------------
  Determinate if the numbers of params 
  are correct
--------------------------------------------*/
void checkNumberArguments(int numberArguments){
    if (numberArguments != 5){
        wrongMessage();
    }
}

/*--------------------------------------------
  Determinate params are correct and return 
  int array to use them
--------------------------------------------*/
unsigned int* checkArguments(char** arguments){

    // Check if all argument are numbers
    char *end;
    strtol(arguments[1], &end, 10);
    if (*end != '\0') {
        wrongMessage();
    }
    strtol(arguments[2], &end, 10);
    if (*end != '\0') {
        wrongMessage();
    }
    strtol(arguments[3], &end, 10);
    if (*end != '\0') {
        wrongMessage();
    }
    strtol(arguments[4], &end, 10);
    if (*end != '\0') {
        wrongMessage();
    }

    // Check if arguments have any negative number
    if (arguments[0].find('-')) {
        wrongMessage();
    }

    unsigned int* argsInt = new unsigned int[4];
    argsInt[0] = std::atoi( arguments[1] );
    argsInt[1] = std::atoi( arguments[2] ); 
    argsInt[2] = std::atoi( arguments[3] );
    argsInt[3] = std::atoi( arguments[4] );


    return argsInt;
}


int main (int argc, char** argv) {
    // Check Number of Arguments
    checkNumberArguments(argc);

    // Check args and return with correct format
    unsigned int *args = checkArguments(argv);

    cout << args[0] << "\n";
    cout << args[1] << "\n";
    cout << args[2] << "\n";
    cout << args[3] << "\n";

    return 0;
}
