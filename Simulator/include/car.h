#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include <random>

class Car {
    /* -----------------Variables-----------------*/
    private:
        // Probability Disitribution of max_speeds 
        static const std::vector<std::pair<double, int>> speed_distribution;
        // variable to store how fast the car will go at max, is computed in the constructor with the speed_distribution
        std::mt19937 rng;
    public:
        // Variable to store the current speed of the car. Is going to be modified by the simulator
        const unsigned int max_speed;
        // Variable to store the max speed of the car
        unsigned int speed;
    
    /* -----------------Constructors-----------------*/
        Car(bool start_velocity_zero, bool always_unlimited);
        Car(bool start_velocity_zero, int max_speed);
    
    /* -----------------Methods-----------------*/
    private:
        int compute_max_speed(bool always_unlimited);
        int compute_start_speed(bool start_velocity_zero);
    public:
        void accelerate();
        void decelerate();
        void set_speed(int speed);
};

#endif