#include "../include/simulator_periodic.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__ || __unix__
#include <unistd.h>
#endif

// #################################################################### //
// ##################### CONSTRUCTOR & DESTRUCTOR ##################### //
// #################################################################### //

SimulatorPeriodic::SimulatorPeriodic(int street_length, int initial_cars, int vmax, int iterations, float dawdle_probability, bool always_unlimited, bool start_velocity_zero, bool multicore)
{
    parameters.street_length = street_length;
    parameters.initial_cars = initial_cars;
    parameters.vmax = vmax; // os set to -1 if the max speed is unlimited
    parameters.iterations = iterations;
    parameters.dawdle_probability = dawdle_probability;
    parameters.always_unlimited = always_unlimited;
    parameters.start_velocity_zero = start_velocity_zero;
    parameters.multicore = multicore;

// generate the output file name in the format "output_YYYYMMDD_HHMMSS.csv"
// find the directory of the executable

// for windows systems
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD count = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (count == 0)
        throw std::runtime_error("Error: Could not get the path of the executable (Code: 113)");
    std::string path(buffer, count);

// for linux or other unix systems
#elif __linux__ || __unix__
    // for linux systems
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count == -1)
        throw std::runtime_error("Error: Unable to resolve executable path");
    std::string path(result, count);

// for mac systems
#elif __APPLE__
    // for mac systems
    char result[PATH_MAX];
    uint32_t size = sizeof(result);
    if (_NSGetExecutablePath(result, &size) != 0)
        throw std::runtime_error("Error: Unable to resolve executable path");
    std::string path(result);
#endif

    // get the parent directory of the executable
    std::filesystem::path exeDir = std::filesystem::path(path).parent_path();

    // put the subdirectory "output" in the parent directory
    std::filesystem::path outputDir = exeDir / "output";

    // check if the output directory exists, if not create it
    if (!std::filesystem::exists(outputDir))
    {
        if (!std::filesystem::create_directory(outputDir))
            throw std::runtime_error("Error: Could not create output directory (Code: 114)");
    }

    // get current date and time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);

    // create a string with the current date and time in the format "YYYYMMDD_HHMMSS"
    char timeBuffer[20];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%d%m%Y_%H%M%S", &local_tm);

    // create the output file name
    std::filesystem::path filePath = outputDir / ("output_" + std::string(timeBuffer) + ".csv");
    parameters.output_file_name = filePath.string();
}

/// @brief Destructor to delete the cars from the street
SimulatorPeriodic::~SimulatorPeriodic()
{
    for (Car *car : reading_street)
    {
        if (car)
            delete car;
    }
    for (Car *car : writing_street)
    {
        if (car)
            delete car;
    }
}

// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

// ====================================================== //
// ================ Simulation-Run-Method =============== //
// ====================================================== //

void SimulatorPeriodic::perform_simulation()
{
    if (parameters.multicore)
        perform_simulation_multicore();
    else
        perform_simulation_singlecore();
}

/// @brief Method to perform the simulation without multicore support
void SimulatorPeriodic::perform_simulation_singlecore()
{
    // initialize a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());

    // initialize the street
    initialize_street();
    // fill the street with the initial cars
    fill_street(reading_street);

    // write the parameters and the initial state of the street to the output file
    print_parameters();
    print_street(reading_street);

    // perform the simulation steps for the given number of iterations
    for (int i = 0; i < parameters.iterations; i++)
    {
        // accelerate the cars
        accelerate_cars(reading_street, writing_street, 0, reading_street.size() - 1);
        // decelerate the cars
        decelerate_cars(reading_street, writing_street, 0, reading_street.size() - 1);
        // dawdle the cars
        dawdle_cars(reading_street, writing_street, 0, reading_street.size() - 1, parameters.dawdle_probability, rng);
        // move the cars
        move_cars(reading_street, writing_street, 0, reading_street.size() - 1);

        // write the new state of the street to the output file
        print_street(reading_street);
    }
}

void SimulatorPeriodic::perform_simulation_multicore()
{
    // TO BE IMPLIMENTED
}
// ====================================================== //
// ================= Initializer-Methods ================ //
// ====================================================== //

/// @brief Method to initialize the street
void SimulatorPeriodic::initialize_street()
{
    this->reading_street.resize(parameters.street_length);
    this->writing_street.resize(parameters.street_length);
}

/// @brief Method to fill the street with the given number of initial cars
/// @param street The street to fill with cars
/// @param initial_cars The number of initial cars to place on the street
void SimulatorPeriodic::fill_street(std::vector<Car *> &street)
{
    if (parameters.initial_cars > static_cast<int>(street.size()))
        throw std::runtime_error("Error: Number of initial cars exceeds street size (Code: 111)");

    // Create a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());

    // Create a distribution to get a random number between 0 and the street length
    std::uniform_int_distribution<> dis(0, street.size() - 1);

    // generate a vector with the indices of the street
    std::vector<int> indices(street.size());
    std::iota(indices.begin(), indices.end(), 0);
    // shuffle the indices to get a random order
    std::shuffle(indices.begin(), indices.end(), rng);

    // iterate over the indices and place a car at the index
    for (int i = 0; i < parameters.initial_cars; i++)
    {
        // create a new car and place it at the index
        if (parameters.always_unlimited){
            street[indices[i]] = new Car(parameters.start_velocity_zero, true, rng);
        }
        else if (parameters.always_unlimited == false && parameters.vmax == -1) {
            street[indices[i]] = new Car(parameters.start_velocity_zero, false, rng);
        }
        else {
            street[indices[i]] = new Car(parameters.start_velocity_zero, parameters.vmax, rng);
        }
    }
}

// ====================================================== //
// ================= Computation-Methods ================ //
// ====================================================== //

/// @brief Accelerate the cars by 1 if their speed is below the max speed
/// @param reading_street The street to read the cars from
/// @param writing_street The street to write the updated cars to
/// @param start_index The start index of the street section to accelerate the cars at
/// @param end_index The end index of the street section to accelerate the cars at
void SimulatorPeriodic::accelerate_cars(std::vector<Car *> &reading_street, std::vector<Car *> &writing_street, int start_index, int end_index)
{
    if (start_index > end_index || end_index >= static_cast<int>(reading_street.size()) || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for acceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 101)");

    for (int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;

        if (reading_street[i]->speed == 0)
            writing_street[i] = reading_street[i]; // if the car is already at speed 0, just move it to the writing street

        if (reading_street[i]->speed < reading_street[i]->max_speed)
        {
            // move the car p to the writing street
            writing_street[i] = reading_street[i]; // Move the car to the writing street and delete it from the reading street
            writing_street[i]->speed++;            // Accelerate the car
        }
        else if (reading_street[i]->speed == reading_street[i]->max_speed)
        {
            writing_street[i] = reading_street[i]; // if the car is already at max speed, just move it to the writing street
        }
        else
        {
            throw std::runtime_error("Error: Speed of car is above max speed " + std::to_string(reading_street[i]->speed) + " (Code: 102)");
        }
    }
    // If the computation was successful, swap the reading and writing street adresses
    std::swap(reading_street, writing_street);
    std::fill(writing_street.begin(), writing_street.end(), nullptr);
}

/// @brief Decelerate the cars if they would collide with another car
/// @param reading_street The street to read the cars from
/// @param writing_street The street to write the updated cars to
/// @param start_index The start index of the street section to decelerate the cars at
/// @param end_index The end index of the street section to decelerate the cars at
void SimulatorPeriodic::decelerate_cars(std::vector<Car *> &reading_street, std::vector<Car *> &writing_street, int start_index, int end_index)
{
    // iterate over the street and decelerate the cars by 1
    if (start_index > end_index || end_index >= static_cast<int>(reading_street.size()) || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for deceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 104)");

    for (int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;

        // Iterate over the speed of the car and check if the car would collide with another car
        for (int distance = 1; distance <= reading_street[i]->speed; distance++)
        {
            // check if the street is empty at the position the car would move to
            int position = (i + distance) % reading_street.size();
            if (reading_street[position] && reading_street[position] != reading_street[i])
            {
                writing_street[i] = reading_street[i]; // Move the car to the writing street
                writing_street[i]->speed = distance - 1; // Decelerate the car to the speed where it would collide
                break;
            }
            // if the car does not collide, just move it to the writing street
            if (distance == reading_street[i]->speed)
                writing_street[i] = reading_street[i];
        }
    }
    std::swap(reading_street, writing_street);
    std::fill(writing_street.begin(), writing_street.end(), nullptr);
}

/// @brief decelerate cars by 1 with a certain probability
/// @param reading_street Street to read the cars from
/// @param writing_street Street to write the updated cars to
/// @param start_index The start index of the street section to dawdle the cars at
/// @param end_index The end index of the street section to dawdle the cars at
/// @param dawdle_prob Probability to dawdle the car
/// @param rng Random number generator to generate the random numbers for the dawdle probability
void SimulatorPeriodic::dawdle_cars(std::vector<Car *> &reading_street, std::vector<Car *> &writing_street, int start_index, int end_index, float dawdle_prob, std::mt19937 &rng)
{
    if (start_index > end_index || end_index >= static_cast<int>(reading_street.size()) || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for deceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 105)");

    if (dawdle_prob < 0 || dawdle_prob > 1)
        throw std::runtime_error("Error: Invalid dawdle probability" + std::to_string(dawdle_prob) + " (Code: 106)");

    std::uniform_real_distribution<> dis(0, 1); // Generate a distribution from 0 to 1 to get a random number from
    for (int i = start_index; i <= end_index; i++)
    {
        if (!reading_street[i]) // check if the street is empty at this position, if yes -> continue
            continue;
        // Check if the probability dawdle_prob is bigger than a random number between 0 and 1
        if (dis(rng) < dawdle_prob && reading_street[i]->speed > 0) // if the car is not at speed 0, dawdle it
        {
            writing_street[i] = reading_street[i]; // Move the car to the writing street and delete it from the reading street
            writing_street[i]->speed--;            // Dawdle the car
        }
        else
        {
            writing_street[i] = reading_street[i]; // if the car does not dawdle, just move it to the writing street
        }
    }
    std::swap(reading_street, writing_street);
    std::fill(writing_street.begin(), writing_street.end(), nullptr);
}

/// @brief Move the cars to their new position
/// @param reading_street The street to read the cars from
/// @param writing_street The street to write the updated cars to
/// @param start_index The start index of the street section to move the cars at
/// @param end_index The end index of the street section to move the cars at
void SimulatorPeriodic::move_cars(std::vector<Car *> &reading_street, std::vector<Car *> &writing_street, int start_index, int end_index)
{
    // check if the start and end index are valid
    if (start_index > end_index || end_index >= static_cast<int>(reading_street.size()) || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for move (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 107)");

    // place the cars at their new position, make sure to move only cars that stay in the index range
    for (int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;
        // check if there was a computation error in the previous steps
        if (writing_street[(i + reading_street[i]->speed) % writing_street.size()] != nullptr) {
            int collision_index = (i + reading_street[i]->speed) % writing_street.size(); 
            throw std::runtime_error("Error: Car at position " + std::to_string(i) + "Speed: "+ std::to_string(reading_street[i]->speed) +" would collide with another car at: " + std::to_string(collision_index) + "(Code: 108)");
        }

        // move the car to the new position
        writing_street[(i + reading_street[i]->speed) % reading_street.size()] = reading_street[i];
    }
    std::swap(reading_street, writing_street);
    std::fill(writing_street.begin(), writing_street.end(), nullptr);
}

// ====================================================== //
// =================== Output-Methods =================== //
// ====================================================== //

/// @brief Method to print the parameters of the simulation to the file
void SimulatorPeriodic::print_parameters()
{
    // open the output file in append mode
    std::ofstream file(parameters.output_file_name, std::ios::app);

    // check if the file could be opened
    if (!file.is_open())
        throw std::runtime_error("Error: Could not open output file (Code: 110)");

    // write the parameters of the simulation to the file
    file << "Street Length: " << parameters.street_length << ", "
         << "Initial Cars: " << parameters.initial_cars << ", "
         << "Max Speed: " << parameters.vmax << ", "
         << "Iterations: " << parameters.iterations << ", "
         << "Dawdle Probability: " << parameters.dawdle_probability << ", "
         << "Unlimited Speed: " << (parameters.always_unlimited ? "Yes, " : "No, ")
         << "Cars start with speed 0:" << (parameters.start_velocity_zero ? "Yes" : "No") << "\n";

    // close the file
    file.close();
}

/// @brief Method to write the current state of the street to a file
/// @param street The street to write to the file
void SimulatorPeriodic::print_street(std::vector<Car *> &street)
{
    // open the output file in append mode
    std::ofstream file(parameters.output_file_name, std::ios::app);

    // check if the file could be opened
    if (!file.is_open())
        throw std::runtime_error("Error: Could not open output file (Code: 109)");

    // write the current state of the street to the file
    for (size_t i = 0; i < street.size(); i++)
    {
        if (!street[i])
            // Write a "-" if the cell is empty (nullptr)
            file << "-";
        else
            // Otherwise, write the car's speed
            file << street[i]->speed;

        // Separate the cars with a comma, except for the last car
        if (i != street.size() - 1)
            file << ",";
    }

    // Add a newline character at the end of the line
    file << std::endl;

    // close the file
    file.close();
}