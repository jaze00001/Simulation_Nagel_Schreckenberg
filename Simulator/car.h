#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include <random>

class Car {
    /* -----------------Variables-----------------*/
    private:
        // Probability Disitribution of max_speeds 
        static const std::vector<std::pair<double, short>> speed_distribution;
        // variable to store how fast the car will go at max, is computed in the constructor with the speed_distribution
        std::mt19937 rng;
    public:
        // Variable to store the current speed of the car. Is going to be modified by the simulator
        const unsigned short max_speed;
        // Variable to store the max speed of the car
        unsigned short speed;
    
    /* -----------------Constructors-----------------*/
        Car(bool start_velocity_zero, bool always_unlimited);
        Car(bool start_velocity_zero, short max_speed);
    
    private:
    /* -----------------Methods-----------------*/
        short compute_max_speed(bool always_unlimited);
        short compute_start_speed(bool start_velocity_zero);
};

#endif // CAR_H