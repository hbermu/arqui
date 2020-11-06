#include <cstring>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace std;
namespace fs = experimental::filesystem;

///////////////////
// Static Params //
///////////////////
// Name
const char *nameProgram = "image-seq";

// File Paths
//static string pathInitConfig = "-";

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
    if (argc != 4){
        const char *message = "Wrong format:";
        wrongMessage(message);
    }
}

/// <summary>Check function parsed</summary>
/// <param name="argc">Function Argument</param>
/// <returns></returns>
void checkArgumentsFunctionName(const char *function){
    bool rightFunction = false;
    // const int functionsSize = (sizeof(programFunctions)/sizeof(programFunctions[0]));
    // for (int i = 0; i < functionsSize; i++){
    for (auto & programFunction : programFunctions){
        if (strcmp(function,programFunction) == 0){
            rightFunction = true;
        }
    }
    if (!rightFunction){
        string messageError = "Unexpected operation: ";
        messageError += function;
        const char *message = messageError.c_str();
        wrongMessage(message);

    }
}

/// <summary>Check if the path exists</summary>
/// <param name="path">Path to check</param>
/// <returns>Boolean with false if wrong path or true if right path</returns>
bool pathExists(const char *path){

    struct stat info{};
    int statRC = stat( path, &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return false; } // something along the path does not exist
        if (errno == ENOTDIR) { return false; } // something in path prefix is not a dir
        return -1;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

/// <summary>Check paths to read and write</summary>
/// <param name="pathSource">Source path</param>
/// <param name="pathDestination">Destination path</param>
/// <returns></returns>
void checkArgumentsPaths(const char *pathSource, const char *pathDestination){

    if (!pathExists(pathSource)){
        string message = string("Input path: ") + pathSource + string("\n");
        message += string("Output path: ") + pathDestination + string("\n");
        message += string("Cannot open directory [") + pathSource + string("]");
        wrongMessage(message.c_str());
    }
    if (!pathExists(pathDestination)){
        string message = string("Input path: ") + pathSource + string("\n");
        message += string("Output path: ") + pathDestination + string("\n");
        message += string("Output directory [") + pathDestination + string("] does not exist");
        wrongMessage(message.c_str());
    }

}

/// <summary>Check arguments</summary>
/// <param name="argc">Program arguments number</param>
/// <param name="arguments">Program arguments</param>
/// <returns></returns>
void checkArguments(int argc, char **argv){
    checkArgumentsNumber(argc);

    checkArgumentsFunctionName(argv[1]);

    checkArgumentsPaths(argv[2], argv[3]);

}

/// <summary>Get BMP images Paths</summary>
/// <param name="path">Path to search the images</param>
/// <returns>Vector with all paths</returns>
vector<string> getImagesPaths(const char *path){

    vector<string> imagesPathsVector;
    for (const auto & entry : fs::directory_iterator(path)){
        if (string(entry.path()).find(string(".bmp")) != string::npos) {
            imagesPathsVector.push_back( entry.path().string() );
        }
    }
    return imagesPathsVector;
}

/// <summary>Copy all images from source to destination</summary>
/// <param name="imagesPaths">Vector with all images paths</param>
/// <param name="pathDestination">Destination path</param>
/// /// <returns></returns>
void checkArguments(const vector<string>& imagesPaths, const char *pathDestination){

    for(const string& imagePath : imagesPaths){
        
        fs::copy(imagePath,pathDestination);
    }

}

///////////
// MAIN //
//////////
int main (int argc, char** argv) {
    checkArguments(argc, argv);

    vector<string> imagesPaths = getImagesPaths(argv[2]);

    if (strcmp(argv[1],"copy") == 0){
        checkArguments(imagesPaths, argv[3]);
    }

    return 0;
}
