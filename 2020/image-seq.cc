#include <climits>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

///////////////////
// Static Params //
///////////////////
// Name
static std::string nameProgram = "image-seq";

// File Paths
//static std::string pathInitConfig = "-";

// Program Functions
const char *programFunctions[3] = { "copy", "gauss", "sobel"};

/////////////
// Structs //
/////////////


///////////////
// Functions //
///////////////

/// <summary>Print error and exit program</summary>
/// <param name="message">String to print</param>
/// <returns></returns>
void wrongMessage(const char *message){
    cout    << message << "\n"
            << "  " << nameProgram << " operation in_path out_path:\n"
            << "    operation: copy, gauss, sobel\n";
    exit(-1);
}

/// <summary>Check number of arguments</summary>
/// <param name="argc">Number of arguments</param>
/// <returns></returns>
void checkArgumentsNumber(int argc){
    if (argc != 3){
        const char *message = "Wrong format:";
        wrongMessage(message);
    }
}

/// <summary>Check function parsed</summary>
/// <param name="argc">Function Argument</param>
/// <returns></returns>
void checkArgumentsFunctionName(char *function){
    int rightFunction = 0;
    const int functionsSize = (sizeof(programFunctions)/sizeof(programFunctions[0]));
    for (int i = 0; i < functionsSize; i++){
        if (function.compare(programFunctions[i]) == 0){
            rightFunction = 1;
        }
    }
    if (rightFunction == 0){
        const char *message = "Unexpected operation: " + function;
        wrongMessage(message);
    }
}

/// <summary>Check arguments</summary>
/// <param name="argc">Program arguments number</param>
/// <param name="arguments">Program arguments</param>
/// <returns>Documentation of return type</returns>
void checkArguments(int argc, char **argv){
    checkArgumentsNumber(argc);

    checkArgumentsFunctionName(argv[1]);

}


///////////
// MAIN //
//////////
int main (int argc, char** argv) {
    checkArguments(argc, argv);

    return 0;
}
