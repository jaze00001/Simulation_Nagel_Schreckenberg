#include "../include/simulator_periodic.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip> 
#include <ctime>
#include <algorithm>

// ##################################################################### //
// ############################ CONSTRUCTORS ########################### //
// ##################################################################### //

SimulatorPeriodic::SimulatorPeriodic(unsigned int street_length, unsigned int initial_cars, int vmax, unsigned int iterations, float dawdle_probability, bool always_unlimited, bool start_velocity_zero)
{
    parameters.street_length = street_length;
    parameters.initial_cars = initial_cars;
    parameters.vmax = vmax; // os set to -1 if the max speed is unlimited
    parameters.iterations = iterations;
    parameters.dawdle_probability = dawdle_probability;
    parameters.always_unlimited = always_unlimited;
    parameters.start_velocity_zero = start_velocity_zero;
}

// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

// ====================================================== //
// ================ Simulation-Run-Method =============== //
// ====================================================== //

/// @brief Method to perform the simulation
void SimulatorPeriodic::perform_simulation()
{
    // TO BE IMPLEMENTED
}


// ====================================================== //
// ================= Initializer-Methods ================ //
// ====================================================== //

/// @brief Method to initialize the street 
void SimulatorPeriodic::initialize_street()
{
    this->reading_street = std::vector<std::unique_ptr<Car>>(parameters.street_length, nullptr);
    this->writing_street = std::vector<std::unique_ptr<Car>>(parameters.street_length, nullptr);
}

/// @brief Method to fill the street with the given number of initial cars
/// @param street The street to fill with cars 
/// @param initial_cars The number of initial cars to place on the street
void SimulatorPeriodic::fill_street(std::vector<std::unique_ptr<Car>> &street, unsigned int initial_cars) 
{
    if (initial_cars > street.size())
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
    for (int i = 0; i < initial_cars; i++)
    {
        // check if the street is empty at the index
        if (street[indices[i]])
            throw std::runtime_error("Error: Car already exists at position " + std::to_string(indices[i]) + " (Code: 112)");       // RENOVE LATER (for testing purposes)
        // create a new car and place it at the index
        if (parameters.always_unlimited)
            street[indices[i]] = std::make_unique<Car>(parameters.start_velocity_zero, true, rng);

        else if (parameters.always_unlimited == false && parameters.vmax == -1)
            street[indices[i]] = std::make_unique<Car>(parameters.start_velocity_zero, false, rng);
        else
            street[indices[i]] = std::make_unique<Car>(parameters.start_velocity_zero, parameters.vmax, rng);
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
void SimulatorPeriodic::accelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index)
{
    if (start_index > end_index || end_index >= reading_street.size() || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for acceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 101)");

    for (unsigned int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;

        if (reading_street[i]->speed < reading_street[i]->max_speed)
        {
            // move the car p to the writing street
            writing_street[i] = std::move(reading_street[i]); // Move the car to the writing street and delete it from the reading street
            writing_street[i]->speed++;                       // Accelerate the car
        }
        else if (reading_street[i]->speed == reading_street[i]->max_speed)
        {
            writing_street[i] = std::move(reading_street[i]); // if the car is already at max speed, just move it to the writing street
        }
        else
        {
            throw std::runtime_error("Error: Speed of car is above max speed (Code: 102)");
        }
    }
    // If the computation was successful, swap the reading and writing street adresses
    std::swap(reading_street, writing_street);
}

/// @brief Decelerate the cars if they would collide with another car
/// @param reading_street The street to read the cars from
/// @param writing_street The street to write the updated cars to
/// @param start_index The start index of the street section to decelerate the cars at
/// @param end_index The end index of the street section to decelerate the cars at
void SimulatorPeriodic::decelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index)
{
    // iterate over the street and decelerate the cars by 1
    if (start_index > end_index || end_index >= reading_street.size() || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for deceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 104)");
    for (int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;
        // check the car would collide with another car if it would drive at its current speed
        unsigned int distance = 1;
        while (distance <= reading_street[i]->speed)
        {
            if (reading_street[(i + distance) % reading_street.size()])
            {
                writing_street[i] = std::move(reading_street[i]); // Move the car to the writing street and delete it from the reading street
                writing_street[i]->speed = distance - 1;          // Decelerate the car
                break;
            }
            distance++;
        }
        // If the car can drive at its current speed without colliding with another car, just move it to the writing street
        if (reading_street[i]) // check if the car was not moved in the loop before
            writing_street[i] = std::move(reading_street[i]);
    }
    std::swap(reading_street, writing_street);
}

/// @brief decelerate cars by 1 with a certain probability
/// @param reading_street Street to read the cars from
/// @param writing_street Street to write the updated cars to
/// @param start_index The start index of the street section to dawdle the cars at
/// @param end_index The end index of the street section to dawdle the cars at
/// @param dawdle_prob Probability to dawdle the car
/// @param rng Random number generator to generate the random numbers for the dawdle probability
void SimulatorPeriodic::dawdle_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index, float dawdle_prob, std::mt19937 &rng)
{
    if (start_index > end_index || end_index >= reading_street.size() || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for deceleration (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 105)");

    if (dawdle_prob < 0 || dawdle_prob > 1)
        throw std::runtime_error("Error: Invalid dawdle probability" + std::to_string(dawdle_prob) + " (Code: 106)");

    std::uniform_real_distribution<> dis(0, 1); // Generate a distribution from 0 to 1 to get a random number from
    for (int i = start_index; i <= end_index; i++)
    {
        if (!reading_street[i]) // check if the street is empty at this position, if yes -> continue
            continue;
        // Check if the probability dawdle_prob is bigger than a random number between 0 and 1
        if (dis(rng) < dawdle_prob)
        {
            writing_street[i] = std::move(reading_street[i]); // Move the car to the writing street and delete it from the reading street
            writing_street[i]->speed--;                       // Dawdle the car
        }
        else
        {
            writing_street[i] = std::move(reading_street[i]); // if the car does not dawdle, just move it to the writing street
        }
    }
    std::swap(reading_street, writing_street);
}

/// @brief Move the cars to their new position
/// @param reading_street The street to read the cars from
/// @param writing_street The street to write the updated cars to
/// @param start_index The start index of the street section to move the cars at
/// @param end_index The end index of the street section to move the cars at
void SimulatorPeriodic::move_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index)
{
    // check if the start and end index are valid
    if (start_index > end_index || end_index >= reading_street.size() || start_index < 0)
        throw std::runtime_error("Error: Invalid start or end index for move (from " + std::to_string(start_index) + " to " + std::to_string(end_index) + ") (Code: 107)");

    // place the cars at their new position, make sure to move only cars that stay in the index range
    for (int i = start_index; i <= end_index; i++)
    {
        // check if the street is empty at this position
        if (!reading_street[i])
            continue;
        // check if there was a computation error in the previous steps
        if (writing_street[(i + reading_street[i]->speed) % writing_street.size()] != nullptr)
            throw std::runtime_error("Error: Car at position " + std::to_string(i) + " would collide with another car (Code: 108)");

        // move the car to the new position
        writing_street[(i + reading_street[i]->speed) % reading_street.size()] = std::move(reading_street[i]);
    }
    std::swap(reading_street, writing_street);
}

// ====================================================== //
// =================== Output-Methods =================== //
// ====================================================== //

/// @brief Method to write the current state of the street to a file
/// @param street The street to write to the file
void SimulatorPeriodic::print_street(std::vector<std::unique_ptr<Car>> &street)
{
    // set variable to the directory the program is stored in
    std::filesystem::path program_path = std::filesystem::current_path(); // get the current path of the program
    std::filesystem::path output_dir = program_path / "output";           // create a new path for the output directory

    // check if the output directory exists, if not create it
    if (!std::filesystem::exists(output_dir))
    {
        if (!std::filesystem::create_directory(output_dir))
            throw std::runtime_error("Error: Could not create output directory (Code: 109)");
        
    }

    // Get current time for the filename
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);

    // Create a stringstream to format the time
    std::ostringstream time_stream;
    time_stream << std::put_time(&local_tm, "%Y-%m-%d_%H-%M-%S"); // Format: YYYY-MM-DD_HH-MM-SS

    // Create the filename with date and time
    std::string filename = (output_dir / ("Simulation_Output_" + time_stream.str() + ".csv")).string();

    // open the file in write mode
    std::ofstream file(filename);

    // check if the file was opened successfully
    if (!file.is_open())
        throw std::runtime_error("Error: Could not open file " + filename + " (Code: 110)");

    // write the used simulation parameters to the first line of the file
    file << "Street Length: " << parameters.street_length << ", "
         << "Initial Cars: " << parameters.initial_cars << ", "
         << "Max Speed: " << parameters.vmax << ", "
         << "Iterations: " << parameters.iterations << ", "
         << "Dawdle Probability: " << parameters.dawdle_probability << ", "
         << "Unlimited Speed: " << (parameters.always_unlimited ? "Yes" : "No") << "\n";

    // write the street to the file
    for (size_t i = 0; i < street.size(); i++)
    {
        if (!street[i]) 
            file << "-"; // empty cell
        else 
            file << street[i]->speed; // car speed
        
        // separate the cars with a comma, except for the last car
        if (i != street.size() - 1)
            file << ",";
    }

    // set a newline character at the end of the line (end of street)
    file << "\n";

    // close the file
    file.close();
}