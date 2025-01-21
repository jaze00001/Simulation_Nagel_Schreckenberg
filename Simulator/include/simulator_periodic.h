#ifndef SIMULATOR_PERIODIC_H
#define SIMULATOR_PERIODIC_H

#include "simulator_base.h"

// Struct to store the parameters of the simulation for periodic boundaries
struct PeriodicParameters
{
    unsigned int street_length, initial_cars, vmax, iterations;
    float dawdle_probability;
    bool always_unlimited, start_velocity_zero;
};

class SimulatorPeriodic : public SimulatorBase
{

// ##################################################################### //
// ############################# VARIABLES ############################# //
// ##################################################################### //

private:
    PeriodicParameters parameters;
    std::vector<std::unique_ptr<Car>> reading_street;
    std::vector<std::unique_ptr<Car>> writing_street;

// ##################################################################### //
// ############################ CONSTRUCTORS ########################### //
// ##################################################################### //

public:
    SimulatorPeriodic(unsigned int street_length, unsigned int initial_cars, int vmax, unsigned int iterations, float dawdle_probability, bool always_unlimited, bool start_velocity_zero);

// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

public:
    void perform_simulation() override;

private:
    // Methods to perform simulation steps and print results to file
    void accelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) override;
    void decelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) override;
    void dawdle_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index, float dawdle_prob, std::mt19937 &rng) override;
    void move_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) override;
    void print_street(std::vector<std::unique_ptr<Car>>& street) = 0;
    // Perform simulation methods for multicore and single core
    void perform_simulation() override;
    // Methods to initialize the street
    void initialize_street() override;
    void fill_street(std::vector<std::unique_ptr<Car>> &street, unsigned int initial_cars);
};

#endif