#include <cstring>
#include <chrono>
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

// Gauss params
const unsigned short int gauss_mask[5][5] = {
        {1, 4,  7,  4,  1} ,
        {4, 16, 26, 16, 4} ,
        {7, 26, 41, 26, 7} ,
        {4, 16, 26, 16, 4} ,
        {1, 4,  7,  4,  1} ,
};
const unsigned short int gauss_weight = 273;

// Sobel params
const short int sobel_mask_x[3][3] = {
        { 1,  2,  1} ,
        { 0,  0,  0} ,
        {-1, -2, -1}
};
const short int sobel_mask_y[3][3] = {
        {-1,  0,  1} ,
        {-2,  0,  2} ,
        {-1,  0,  1}
};
const unsigned short int sobel_weight = 8;

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
    string path;
    unsigned char info[54];
    unsigned char* post_info;
    vector <vector<bmp_pixel> > data;
    chrono::duration<long, ratio<1, 1000000>> time_read  = chrono::microseconds(0);
    chrono::duration<long, ratio<1, 1000000>> time_gauss = chrono::microseconds(0);
    chrono::duration<long, ratio<1, 1000000>> time_sobel = chrono::microseconds(0);
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
/// <returns></returns>get_images_paths
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


/// <summary>Check if BPM can be processed</summary>
/// <param name="images_paths">Image info</param>
/// <returns>Boolean with false if wrong image or true if right image info</returns>
bool can_process_image(const unsigned char image_info[54]){

    // Wrong number of planes
    if (*(short int*)&image_info[26] != 1){
        wrong_message("Planes is not 1");
    }

    // Wrong points size
    if (*(short int*)&image_info[28] != 24){
        wrong_message("Points size is not 24");
    }

    // Wrong Compression
    if (*(int*)&image_info[30] != 0){
        wrong_message("Image is compressed");
    }

    return true;

}

/// <summary>Read BMP images</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <returns>Vector with all images</returns>
vector<bmp_image> read_images(const vector<string>& images_paths){

    vector<bmp_image> images;

    for(const string& image_path : images_paths){

        auto t1 = chrono::high_resolution_clock::now();
        bmp_image image{};

        // Open the image (read a binary file: rb)
        FILE* image_file = fopen(image_path.c_str(), "rb");

        // Get the file name
        fs::path image_path_filesystem(image_path.c_str());
        image.name = image_path_filesystem.filename().string();
        image.path = image_path_filesystem.relative_path();

        // Read BMP header
        if (fread(image.info, sizeof(unsigned char), 54, image_file) != 54)
            exit(-1);

        // If the image can be processes
        if (can_process_image(image.info)){
            int image_width = *(int*)&image.info[18];
            int row_padded = (image_width*3 + 3) & (~3);
            int image_height = *(int*)&image.info[22];

            int image_post_header_size = *(int*)&image.info[10] - 54;
            unsigned  char* image_post_header = new unsigned char[image_post_header_size];

            if (fread(image_post_header, sizeof(unsigned char), image_post_header_size, image_file) != (unsigned)image_post_header_size)
                exit(-1);
            image.post_info = image_post_header;

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
        }
        // Close the image
        fclose(image_file);

        image.time_read = chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - t1);
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

        auto t1 = chrono::high_resolution_clock::now();

        // Open the image (read and write a binary file: rwb)
        string image_path = path_destination + string("/") + image.name;
        FILE* image_file = fopen(image_path.c_str(), "wb");

        // Write the bmp metadata
        fwrite(image.info, sizeof(unsigned char), 54, image_file);

        const int image_width = *(int*)&image.info[18];
        const int row_padded = (image_width*3 + 3) & (~3);
        const int image_height = *(int*)&image.info[22];

        fwrite(image.post_info, sizeof(unsigned char), *(int*)&image.info[10] - 54, image_file);

        unsigned char* image_data_row = new unsigned char[row_padded];

        for(int i = 0; i < image_height; i += 1){
            for(int j = 0; j < image_width; j += 1){
                image_data_row[(j*3)] = image.data[i][j].blue;
                image_data_row[(j*3) + 1] = image.data[i][j].green;
                image_data_row[(j*3) + 2] = image.data[i][j].red;
            }
            // Write the row
            fwrite(image_data_row, sizeof(unsigned char), row_padded, image_file);
        }
        // Close the image
        fclose(image_file);

        auto time_write = chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - t1);

        unsigned int total_time = image.time_read.count() + time_write.count();
        if(image.time_gauss.count() != 0)
            total_time += image.time_gauss.count();
        if(image.time_sobel.count() != 0)
            total_time += image.time_sobel.count();

        // Print times
        cout << "File: \"" << image.path << "\"(time : " << total_time << ")" <<endl;
        cout << "  Load time: " << image.time_read.count() << endl;
        if(image.time_gauss.count() != 0)
            cout << "  Gauss time: " << image.time_gauss.count() << endl;
        if(image.time_sobel.count() != 0)
            cout << "  Sobel time: " << image.time_sobel.count() << endl;
        cout << "  Store time: " << time_write.count() << endl;
    }

}

/// <summary>Copy all images from source to destination</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <param name="path_destination">Destination path</param>
/// /// <returns></returns>
void function_copy(const vector<string>& images_paths, const char *path_destination){

//    for(const string& image_path : images_paths){
//        fs::copy(image_path, path_destination);
//    }
    vector<bmp_image> images = read_images(images_paths);

    write_images(images, path_destination);

}

/// <summary>Calculate the Gauss</summary>
/// <param name="images_paths">Vector with all images</param>
/// <returns>bmp_image vector with all BMP Images</returns>
vector<bmp_image>  calculate_function_gauss(vector<bmp_image> images){

    for(bmp_image& image : images){

        auto t1 = chrono::high_resolution_clock::now();

        const int image_width = *(int*)&image.info[18];
        const int image_height = *(int*)&image.info[22];

        for(int i = 0; i < image_height; i += 1){
            for(int j = 0; j < image_width; j += 1) {

                unsigned short int blue = 0;
                unsigned short int green = 0;
                unsigned short int red = 0;

                for(int m_i = -2; m_i <= 2; m_i += 1){
                    for(int m_j = -2; m_j <= 2; m_j += 1) {
                        // Control borders
                        if((i + m_i) >= 0 && (i + m_i) < image_height && (j + m_j) >= 0 && (j + m_j) < image_width ){
                            blue += gauss_mask[m_i+2][m_j+2] * (unsigned short int)image.data[i + m_i][j + m_j].blue;
                            green += gauss_mask[m_i+2][m_j+2] * (unsigned short int)image.data[i + m_i][j + m_j].green;
                            red += gauss_mask[m_i+2][m_j+2] * (unsigned short int)image.data[i + m_i][j + m_j].red;
                        }
                    }
                }
                image.data[i][j].blue = (unsigned char)(blue/gauss_weight);
                image.data[i][j].green = (unsigned char)(green/gauss_weight);
                image.data[i][j].red = (unsigned char)(red/gauss_weight);
            }
        }
        image.time_gauss = chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - t1);
    }
    return images;

}

/// <summary>Apply gauss function to all images and save them</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <param name="path_destination">Destination path</param>
/// <returns></returns>
void function_gauss(const vector<string>& images_paths, const char *path_destination){

    vector<bmp_image> images = read_images(images_paths);

    images = calculate_function_gauss(images);

    write_images(images, path_destination);

}

/// <summary>Calculate the Gauss</summary>
/// <param name="images_paths">Vector with all images</param>
/// <returns>bmp_image vector with all BMP Images</returns>
vector<bmp_image>  calculate_function_sobel(vector<bmp_image> images){

    for(bmp_image& image : images){

        auto t1 = chrono::high_resolution_clock::now();

        const int image_width = *(int*)&image.info[18];
        const int image_height = *(int*)&image.info[22];

        for(int i = 0; i < image_height; i += 1){
            for(int j = 0; j < image_width; j += 1) {

                int blue_x = 0;
                int blue_y = 0;
                int green_x = 0;
                int green_y = 0;
                int red_x = 0;
                int red_y = 0;

                for(int m_i = -1; m_i <= 1; m_i += 1){
                    for(int m_j = -1; m_j <= 1; m_j += 1) {
                        // Control borders
                        if((i + m_i) >= 0 && (i + m_i) < image_height && (j + m_j) >= 0 && (j + m_j) < image_width ){
                            blue_x += sobel_mask_x[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].blue;
                            blue_y += sobel_mask_y[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].blue;
                            green_x += sobel_mask_x[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].green;
                            green_y += sobel_mask_y[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].green;
                            red_x += sobel_mask_x[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].red;
                            red_y += sobel_mask_y[m_i+1][m_j+1] * (unsigned short int)image.data[i + m_i][j + m_j].red;
                        }
                    }
                }
                image.data[i][j].blue = (unsigned char)(abs(blue_x/sobel_weight)+abs(blue_y/sobel_weight));
                image.data[i][j].green = (unsigned char)(abs(green_x/sobel_weight)+abs(green_y/sobel_weight));
                image.data[i][j].red = (unsigned char)(abs(red_x/sobel_weight)+abs(red_y/sobel_weight));
            }
        }
        image.time_sobel = chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - t1);
    }

    return images;

}

/// <summary>Apply sobel function to all images and save them</summary>
/// <param name="images_paths">Vector with all images paths</param>
/// <param name="path_destination">Destination path</param>
/// <returns></returns>
void function_sobel(const vector<string>& images_paths, const char *path_destination){

    vector<bmp_image> images = read_images(images_paths);

    images = calculate_function_gauss(images);

    images = calculate_function_sobel(images);

    write_images(images, path_destination);

}

///////////
// MAIN //
//////////
int main (int argc, char** argv) {

    check_arguments(argc, argv);

    cout << "Input path: " << argv[2] << endl;
    cout << "Output path: " << argv[3] << endl;

    if (strcmp(argv[1],"copy") == 0){
        function_copy(get_images_paths(argv[2]), argv[3]);
    }
    if (strcmp(argv[1],"gauss") == 0){
        function_gauss(get_images_paths(argv[2]), argv[3]);
    }
    if (strcmp(argv[1],"sobel") == 0){
        function_sobel(get_images_paths(argv[2]), argv[3]);
    }
    return 0;

}
