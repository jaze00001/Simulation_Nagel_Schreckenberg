#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include <random>

class Car {

// ##################################################################### //
// ############################# VARIABLES ############################# //
// ##################################################################### //
    
    private:
        // Probability Disitribution of max_speeds 
        static const std::vector<std::pair<double, int>> speed_distribution;
    public:
        // Variable to store the current speed of the car. Is going to be modified by the simulator
        const int max_speed;
        // Variable to store the max speed of the car
        int speed;
    
// ##################################################################### //
// ############################ CONSTRUCTORS ########################### //
// ##################################################################### //
    
    public:
        Car(bool start_velocity_zero, bool always_unlimited, std::mt19937 &rng);
        Car(bool start_velocity_zero, int max_speed, std::mt19937 &rng);
    
// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //
    
    private:
        int compute_max_speed(bool always_unlimited, std::mt19937 &rng);
        int compute_start_speed(bool start_velocity_zero, std::mt19937 &rng);
};

#endif