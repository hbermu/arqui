#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

//// Static Params
// Name
static std::string nameProgram = "nasteroids-seq";

// Math values
// static float timeIncrement = 0.1;
// static float minDistance = 2.0;
// static float spaceWidth = 200;
// static float spaceHeight = 200;
// static float m = 1000;
// static float sdm = 50;

// File Paths
static std::string pathInitConfig = "init_config.txt";

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
unsigned int* checkArguments(int argc, char **arguments){

    // Check if all argument are numbers
    char *end;
    for (int i = 1; i < argc; i++){
        strtol(arguments[i], &end, 10);
        if (*end != '\0') {
            wrongMessage();
        }
    }

    // Check if arguments have any negative number
    for (int i = 1; i < argc; i++){
        for (unsigned int j = 0; j < strlen(arguments[i]); j++){
            if (arguments[i][j] == '-'){
                wrongMessage();
            }
        }
    }

    // Define array to return
    unsigned int* argsInt = new unsigned int[4];
    argsInt[0] = std::atoi( arguments[1] );
    argsInt[1] = std::atoi( arguments[2] ); 
    argsInt[2] = std::atoi( arguments[3] );
    argsInt[3] = std::atoi( arguments[4] );

    return argsInt;
}

/*--------------------------------------------
  Write the init config in a file
--------------------------------------------*/
void writeInitConfig(int argsNumber, unsigned int *argsInt, float **asteroids, float **planets){
    // Create and ope the file
    ofstream myfile;
    myfile.open (pathInitConfig);

    // Write params 
    for (int i = 0; i < argsNumber; i++){
        myfile << argsInt[i] << " ";
    }
    myfile << "\n";

    // Write asteroids 
    for (unsigned int i = 0; i < argsInt[0]; i++){
        for (int j = 0; j < 3; i++){
            myfile << asteroids[i][j] << " ";
        }
    }
    myfile << "\n";

    // Write planets 
    for (unsigned int i = 0; i < argsInt[0]; i++){
        for (int j = 0; j < 3; i++){
            myfile << planets[i][j] << " ";
        }
    }
    myfile << "\n";


    myfile.close();
}

int main (int argc, char** argv) {
    // Check Number of Arguments
    checkNumberArguments(argc);

    // Check args and return args as unsigned int format
    unsigned int *args = checkArguments(argc, argv);

    return 0;
}
