#include "../include/car.h"
#include <random>
#include <iostream>

// ##################################################################### //
// ############################# VARIABLES ############################# //
// ##################################################################### //

/* Definition of the speed distribution on the german highways for unlimited speed sections (Data from https://www.iwkoeln.de/studien/thomas-puls-jan-marten-wendt-schneller-als-130-regel-oder-ausnahme.html, 18.01.2025)
By simplifying the data, we can say that 77% of the cars drive 130 km/h at max, 18% 160 km/h at max and 5% even faster */

const std::vector<std::pair<double, int>> Car::speed_distribution = {
    {0.05, 10}, // 5% chance of unlimited speed, faster than 270 km/h is rare and therefore not considered for the sake of simplicity
    {0.23, 6},  // 23% chance of 160 km/h
    {1.00, 5}   // 77% chance of 130 km/h
};

// ##################################################################### //
// ############################ CONSTRUCTORS ########################### //
// ##################################################################### //

Car::Car(bool start_velocity_zero, bool always_unlimited, std::mt19937 &rng) : max_speed(compute_max_speed(always_unlimited, rng)),
                                                            speed(compute_start_speed(start_velocity_zero, rng)) {} // Constructor for unlimited speed limit
Car::Car(bool start_velocity_zero, int max_speed, std::mt19937 &rng) :   max_speed(max_speed),
                                                      speed(compute_start_speed(start_velocity_zero, rng)) {} // Constructor for limited speed limit

// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

/// @brief computes the max speed of the car based on the speed distribution on unlimited speed sections :)
/// @param always_unlimited if true, the max speed will be 10 for all cars, otherwise a random number between 5 and 10 based on the speed distribution
/// @return the max speed of the car
int Car::compute_max_speed(bool always_unlimited, std::mt19937 &rng)
{
    if (always_unlimited)
        return 10;
    // Generate a distribution from 0 to 1 to get a random number from
    std::uniform_real_distribution<> dis(0, 1);
    double random_number = dis(rng);

    // Iterate over the speed distribution and set the max speed according to the random number
    for (auto &pair : speed_distribution)
    {
        if (random_number <= pair.first)
            return pair.second;
    }
    // If no matching speed is found, throw an exception since this should not happen
    throw std::runtime_error("Error: No matching speed found for random number " + std::to_string(random_number) + "(Code: 201)");
}

/// @brief computes the start speed of the car based on the max speed
/// @param start_velocity_zero if true, the start speed will be 0, otherwise a random number between 0 and the max speed
/// @return the start speed of the car
int Car::compute_start_speed(bool start_velocity_zero, std::mt19937 &rng)
{
    if (start_velocity_zero)
    {
        return 0;
    }
    std::uniform_int_distribution<> dis(0, max_speed);
return dis(rng);
}

