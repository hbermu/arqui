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
static std::string nameProgram = "nasteroids-seq";

// File Paths
static std::string pathInitConfig = "init_config.txt";
static std::string pathOutFile = "out.txt";

// Math values
static double gravity = 6.674e-5;
static double maxForce = 200;
static double timeIncrement = 0.1;
static double minDistance = 2.0;
static double spaceWidth = 200;
static double spaceHeight = 200;
static double massPlanet = 10;
static double m = 1000;
static double sdm = 50;

/////////////
// Structs //
/////////////
struct star {
    double x;
    double y;
    double mass;
    double velocityX;
    double velocityY;
};

struct force {
    double x;
    double y;
};

///////////////
// Functions //
///////////////
/*--------------------------------------------
  Create a new star (planet or asteroid) and
  returns it
--------------------------------------------*/
star createStar(double x, double y, double mass){
    star s;
    s.x = x;
    s.y = y;
    s.mass = mass;
    s.velocityX = 0;
    s.velocityY = 0;
    return s;
}

/*--------------------------------------------
  Create a new force and returns it
--------------------------------------------*/
force createForce(double x, double y){
    force m;
    m.x = x;
    m.y = y;
    return m;
}

/*--------------------------------------------
  Truncate a double with 3 decimals
--------------------------------------------*/
string truncateDouble(double number){
    string numberString = to_string(number);
    std::size_t found = numberString.find('.');
    return numberString.substr (0,found + 4); 
}

/*--------------------------------------------
  Return formated string with star values
--------------------------------------------*/
string returnStar(star s){
    return truncateDouble(s.x) + " " + truncateDouble(s.y) + " " + truncateDouble(s.mass);
}

string returnAsteroid(star s){
    return  truncateDouble(s.x) + " " + truncateDouble(s.y) + " " + 
            truncateDouble(s.velocityX) + " " + truncateDouble(s.velocityY) + " " + truncateDouble(s.mass);
}

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
void writeInitConfig(int argc, unsigned int *argsInt, star *stars){
    // Create and open the file
    ofstream myFile;
    myFile.open (pathInitConfig);

    // Write params 
    for (int i = 0; i < argc - 1; i++){
        myFile << argsInt[i] << " ";
    }
    myFile << "\n";

    // Write all stars 
    for (unsigned int i = 0; i < argsInt[0] + argsInt[2]; i++){
        myFile << returnStar(stars[i]) << "\n";
    }

    myFile.close();
}

void writeOutFIle(unsigned int *argsInt, star *stars){
    // Create and ope the file
    ofstream myFile;
    myFile.open (pathOutFile);

    // Write all asteroids 
    for (unsigned int i = 0; i < argsInt[0]; i++){
        myFile << returnAsteroid(stars[i]) << "\n";
    }

    myFile.close();
}

/*--------------------------------------------
  Generate random asteroids params
--------------------------------------------*/
star* createStars(unsigned int *argsInt){
    // Create list of stars
    star* stars = new star[argsInt[0] + argsInt[2]];

    // Generate random fuctions with the seed
    std::default_random_engine re(argsInt[3]);
    std::uniform_real_distribution<double> xdist(0.0, std::nextafter(spaceWidth, std::numeric_limits<double>::max()));
    std::uniform_real_distribution<double> ydist(0.0, std::nextafter(spaceHeight, std::numeric_limits<double>::max()));
    std::normal_distribution<double> mdist(m, sdm);
    
    // Generate asteroids
    for (unsigned int i = 0; i < argsInt[0]; i++){
        stars[i] = createStar(xdist(re), ydist(re), mdist(re));
    }

    // Variable to know where put the planet 
    int order = 0;
    // Generate planets
    for (unsigned int i = 0; i < argsInt[2]; i++){
        switch ( order ){  
            case 0:
                stars[i + argsInt[0]] = createStar(0, ydist(re), mdist(re) * massPlanet);
                order = order +1;
                break;  
            case 1:
                stars[i + argsInt[0]] = createStar(xdist(re), 0, mdist(re) * massPlanet);
                order = order +1;
                break; 
            case 2:
                stars[i + argsInt[0]] = createStar(spaceWidth, ydist(re), mdist(re) * massPlanet);
                order = order +1;
                break;
            default:
              stars[i + argsInt[0]] = createStar(xdist(re), spaceHeight, mdist(re) * massPlanet);
              order = 0;
        }
    }

    return stars;
}

/*--------------------------------------------
  Return the distance between 2 stars
--------------------------------------------*/
double distance(star starA, star starB){
    // sqr( (xa - xb)² + (ya - yb)² )
    double x2 = pow((starA.x - starB.x), 2);
    double y2 = pow((starA.y - starB.y), 2);
    return sqrt(x2 + y2);
}

/*--------------------------------------------
  Return the slope between 2 stars
--------------------------------------------*/
double slope(star starA, star starB){
    // (ya - yb) / (xa - xb) 
    double x = (starA.x - starB.x);
    double y = (starA.y - starB.y);
    double slope = (y / x);
    
    // Correct the slope
    if (slope < -1 || slope > 1){
        slope = slope - ((int)slope / 1);
    }

    return slope;
}

/*--------------------------------------------
  Return the angle from a distance
--------------------------------------------*/
double angle(double slope){
    return atan(slope);
}

/*--------------------------------------------
  Return the force attraction between 
  2 stars: A -> B
--------------------------------------------*/
force attractionForce(star starA, star starB){

    // Calculate the forces
    double division = (gravity * starA.mass * starB.mass) / pow(distance(starA, starB) ,2);
    double xForces = division * cos(angle(slope(starA, starB)));
    double yForces = division * sin(angle(slope(starA, starB)));

    // Check if any value is over 200
    if (xForces > maxForce){
        xForces = maxForce;
    }
    if (yForces > maxForce){
        yForces = maxForce;
    }

    force forces = createForce(xForces, yForces);

    return forces;
}

/*--------------------------------------------
  Calculate the star force
--------------------------------------------*/
star calculateNewPosition(star s, force forces){
    // Calculate aceleration
    double acelerationX = forces.x / s.mass;
    double acelerationY = forces.y / s.mass;

    // Calculate velocity
    s.velocityX = s.velocityX + (acelerationX * timeIncrement);
    s.velocityY = s.velocityY + (acelerationY * timeIncrement);

    // Calculate the new position
    s.x = s.x + (s.velocityX * timeIncrement);
    s.y = s.y + (s.velocityY * timeIncrement);

    // Return the star with the new position
    return s;
}

/*--------------------------------------------
  Do an iteration when the distance is 
  under minDistance with an asteroids
--------------------------------------------*/
star* asteroidColision(star starA, star starB){
    // Generate new array
    star* stars = new star[2];

    // Save the asteroids
    stars[0] = starA;
    stars[1] = starB;

    // Change velocity
    stars[0].velocityX = starB.velocityX;
    stars[0].velocityY = starB.velocityY;
    stars[1].velocityX = starA.velocityX;
    stars[1].velocityY = starA.velocityY;

    // Return stars lists
    return stars;
}

/*--------------------------------------------
  Check if an asteroid is too near to the 
  border
--------------------------------------------*/
star* checkBorder(unsigned int *argsInt, star* allStars){
    for (unsigned int i = 0; i < argsInt[0] - 1; i++){
        if (allStars[i].x < minDistance){
            allStars[i].x = minDistance;
            allStars[i].velocityX = allStars[i].velocityX * (-1);
        }
        if (allStars[i].y < minDistance){
            allStars[i].y = minDistance;
            allStars[i].velocityY = allStars[i].velocityY * (-1);
        }
    }

    return allStars;
}

/*--------------------------------------------
  Do one iteration and depent for the
  distance call the correct fuction
--------------------------------------------*/
star* iteration(unsigned int *argsInt, star* allStars){

    // Array with all asteroid forces all create any forces with 0
    force* forces = new force[argsInt[0]];
    for (unsigned int i = 0; i < argsInt[0]; i++){
        forces[i] = createForce(0, 0);
    }

    // Calculate the forces
    for (unsigned int i = 0; i < argsInt[0]; i++){
        for (unsigned int j = i +1; j < (argsInt[0] + argsInt[2]); j++){
            // Get the distance to decide
            double dista = distance(allStars[i], allStars[j]);

            if (dista > minDistance){
                force f = attractionForce(allStars[i], allStars[j]);
                // Only save the force if the star is a asteroid
                forces[i] = createForce((forces[i].x + f.x), (forces[i].y + f.y));
                if (j < argsInt[0]){
                    forces[j] = createForce((forces[j].x + (-1 * f.x)), (forces[j].y + (-1 * f.y)));
                }
            }
        }
    }

    // Calculate the new position for asteroids
    for (unsigned int i = 0; i < argsInt[0]; i++){
        allStars[i] = calculateNewPosition(allStars[i], forces[i]);
    }

    // Check if any asteroid is in the space border
    allStars = checkBorder(argsInt, allStars);

    // If any astroid ricochets with other asteroid
    for (unsigned int i = 0; i < argsInt[0]; i++){
        for (unsigned int j = i +1; j < (argsInt[0]); j++){
            // Get the distance to decide
            double dista = distance(allStars[i], allStars[j]);

            if (dista <= minDistance){
                star* asteroids = asteroidColision(allStars[i], allStars[j]);
                allStars[i] = asteroids[0];
                allStars[i] = asteroids[1];
            }
        }
    }

    // Return all stars with their new positions
    return allStars;

}

///////////
// MAIN //
//////////
int main (int argc, char** argv) {
    // Check Number of Arguments
    //     argv[1] -> asteroids number
    //     argv[2] -> iterations number
    //     argv[3] -> planets number
    //     argv[4] -> seed

    checkNumberArguments(argc);

    // Check args and return args as unsigned int format
    unsigned int *args = checkArguments(argc, argv);

    // Generate all stars (asteroids and planets)
    star* allStars = createStars(args);

    // Write init config to a file
    writeInitConfig(argc, args, allStars);

    // Exec all iterations
    for (unsigned int i = 0; i < args[1]; i++){
        allStars = iteration(args, allStars);
    }

    // Write final states to a file
    writeOutFIle(args, allStars);

    return 0;
}
