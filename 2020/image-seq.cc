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
const char *name_program = "image-seq";

// Program Functions
const char *program_functions[3] = {"copy", "gauss", "sobel"};

/////////////
// Structs //
/////////////
struct bmp_pixel {
    unsigned char red;
    unsigned char blue;
    unsigned char green;
};
struct bmp_image {
    string name;
    unsigned char info[54];
    vector <vector<bmp_pixel> > data;
    vector <unsigned  char> footer;
};

///////////////
// Functions //
///////////////

/// <summary>Print error and exit program</summary>
/// <param name="message">String to print</param>
/// <returns></returns>
void wrong_message(const char *message){

    cout << message << "\n"
         << "  " << name_program << " operation in_path out_path:\n"
            << "    operation: copy, gauss, sobel\n";
    exit(-1);

}

/// <summary>Check number of arguments</summary>
/// <param name="argc">Number of arguments</param>
/// <returns></returns>
void check_arguments_number(int argc){

    if (argc != 4){
        const char *message = "Wrong format:";
        wrong_message(message);
    }

}

/// <summary>Check function parsed</summary>
/// <param name="argc">Function Argument</param>
/// <returns></returns>
void check_arguments_function_name(const char *function){

    bool right_function = false;
    // const int functionsSize = (sizeof(program_functions)/sizeof(program_functions[0]));
    // for(int i = 0; i < functionsSize; i++){
    for(auto & program_function : program_functions){
        if (strcmp(function, program_function) == 0){
            right_function = true;
        }
    }
    if (!right_function){
        string message_error = "Unexpected operation: ";
        message_error += function;
        const char *message = message_error.c_str();
        wrong_message(message);
    }

}

/// <summary>Check if the path exists</summary>
/// <param name="path">Path to check</param>
/// <returns>Boolean with false if wrong path or true if right path</returns>
bool path_exists(const char *path){

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
/// <param name="path_source">Source path</param>
/// <param name="path_destination">Destination path</param>
/// <returns></returns>
void check_arguments_paths(const char *path_source, const char *path_destination){

    if (!path_exists(path_source)){
        string message = string("Input path: ") + path_source + string("\n");
        message += string("Output path: ") + path_destination + string("\n");
        message += string("Cannot open directory [") + path_source + string("]");
        wrong_message(message.c_str());
    }
    if (!path_exists(path_destination)){
        string message = string("Input path: ") + path_source + string("\n");
        message += string("Output path: ") + path_destination + string("\n");
        message += string("Output directory [") + path_destination + string("] does not exist");
        wrong_message(message.c_str());
    }

}

/// <summary>Check arguments</summary>
/// <param name="argc">Program arguments number</param>
/// <param name="arguments">Program arguments</param>
/// <returns></returns>
void check_arguments(int argc, char **argv){

    check_arguments_number(argc);

    check_arguments_function_name(argv[1]);

    check_arguments_paths(argv[2], argv[3]);

}

/// <summary>Get BMP images Paths</summary>
/// <param name="path">Path to search the images</param>
/// <returns>Vector with all paths</returns>
vector<string> get_images_paths(const char *path){

    vector<string> images_paths_vector;
    for(const auto & entry : fs::directory_iterator(path)){
        if (string(entry.path()).find(string(".bmp")) != string::npos) {
            images_paths_vector.push_back(entry.path().string() );
        }
    }
    return images_paths_vector;

}

/// <summary>Copy all images from source to destination</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <param name="path_destination">Destination path</param>
/// /// <returns></returns>
void function_copy(const vector<string>& images_paths, const char *path_destination){

    for(const string& image_path : images_paths){
        fs::copy(image_path, path_destination);
    }

}

/// <summary>Check if BPM can be processed</summary>
/// <param name="images_paths">Image info</param>
/// <returns>Boolean with false if wrong image or true if right image info</returns>
bool can_process_image(const unsigned char image_info[54]){

    // Wrong number of planes
    if (*(short int*)&image_info[26] != 1)
        return false;

    // Wrong points size
    if (*(short int*)&image_info[28] != 24)
        return false;

    // Wrong Compression
    if (*(int*)&image_info[30] != 0)
        return false;

    return true;

}

/// <summary>Read BMP images</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <returns>Vector with all images</returns>
vector<bmp_image> read_images(const vector<string>& images_paths){

    vector<bmp_image> images;

    for(const string& image_path : images_paths){

        bmp_image image{};

        // Open the image (read a binary file: rb)
        FILE* image_file = fopen(image_path.c_str(), "rb");

        // Get the file name
        fs::path image_path_filesystem(image_path.c_str());
        image.name = image_path_filesystem.filename().string();

        // Read BMP header
        if (fread(image.info, sizeof(unsigned char), 54, image_file) != 54)
            exit(-1);

        // If the image can be processes
        if (can_process_image(image.info)){
            int image_width = *(int*)&image.info[18];
            int row_padded = (image_width*3 + 3) & (~3);
            int image_height = *(int*)&image.info[22];

            unsigned char* image_data_row = new unsigned char[row_padded];

            // Create the image
            for(int i = 0; i < image_height; i += 1){
                // Read a image row
                if (fread(image_data_row, sizeof(unsigned char), row_padded, image_file) != (unsigned)row_padded)
                    exit(-1);
                vector<bmp_pixel> data_line;
                for(int j = 0; j < image_width*3; j += 3){
                    // B - G - R
                    bmp_pixel pixel{};
                    pixel.blue = image_data_row[j];
                    pixel.green = image_data_row[j + 1];
                    pixel.red = image_data_row[j + 2];
                    data_line.push_back(pixel);
                }
                image.data.push_back(data_line);
            }

            // Read the image footer
            unsigned char* buffer = new unsigned char[4];
            while (fread(buffer, 1, 4, image_file) == 4){
                image.footer.push_back(buffer[0]);
                image.footer.push_back(buffer[1]);
                image.footer.push_back(buffer[2]);
                image.footer.push_back(buffer[3]);
            }
        }
        // Close the image
        fclose(image_file);
        images.push_back(image);
    }

    return images;

}

/// <summary>Write BMP images</summary>
/// <param name="images">Vector with all images</param>
/// <param name="path_destination">Destination path</param>
/// <returns></returns>
void write_images(const vector<bmp_image>& images, const char *path_destination){

    for(const bmp_image& image : images){
        // Open the image (read and write a binary file: rwb)
        string image_path = path_destination + string("/") + image.name;
        FILE* image_file = fopen(image_path.c_str(), "wb");

        // Write the bmp metadata
        fwrite(image.info, sizeof(unsigned char), 54, image_file);

        int image_width = *(int*)&image.info[18];
        int row_padded = (image_width*3 + 3) & (~3);
        int image_height = *(int*)&image.info[22];

        unsigned char* image_data_row = new unsigned char[row_padded];

        for(int i = 0; i < image_height; i += 1){
            for(int j = 0; j < image_width; j += 1){
                image_data_row[(j*3)] = image.data[i][j].blue;
                image_data_row[(j*3) + 1] = image.data[i][j].green;
                image_data_row[(j*3) + 2] = image.data[i][j].red;
            }
            // Write the data
            fwrite(image_data_row, sizeof(unsigned char), row_padded, image_file);
        }
        // Write the image footer
        unsigned char* image_footer = new unsigned char[image.footer.size()];
        for(int i = 0; i < (int)image.footer.size(); i += 1){
            image_footer[i] = image.footer[i];
        }
        fwrite(image_footer, sizeof(unsigned char), 1, image_file);

        // Close the image
        fclose(image_file);
    }

}

/// <summary>Apply gauss function to all images and save them</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <param name="path_destination">Destination path</param>
/// <returns></returns>
void function_gauss(const vector<string>& images_paths, const char *path_destination){

    vector<bmp_image> images = read_images(images_paths);

    write_images(images, path_destination);

}

///////////
// MAIN //
//////////
int main (int argc, char** argv) {

    check_arguments(argc, argv);

    if (strcmp(argv[1],"copy") == 0){
        function_copy(get_images_paths(argv[2]), argv[3]);
    }
    if (strcmp(argv[1],"gauss") == 0){
        function_gauss(get_images_paths(argv[2]), argv[3]);
    }
    return 0;

}
