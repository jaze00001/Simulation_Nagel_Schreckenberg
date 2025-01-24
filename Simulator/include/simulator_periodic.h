#ifndef SIMULATOR_PERIODIC_H
#define SIMULATOR_PERIODIC_H

#include "simulator_base.h"

// Struct to store the parameters of the simulation for periodic boundaries
struct PeriodicParameters
{
    int street_length,initial_cars, iterations;
    int vmax;
    float dawdle_probability;
    bool always_unlimited, start_velocity_zero, multicore;
    std::string output_file_name;
};

class SimulatorPeriodic : public SimulatorBase
{

// ##################################################################### //
// ############################# VARIABLES ############################# //
// ##################################################################### //

private:
    PeriodicParameters parameters;
    std::vector<Car*> reading_street;
    std::vector<Car*> writing_street;

// ##################################################################### //
// ###################### CONSTRUCTOR & DESTRUCTOR ##################### //
// ##################################################################### //

public:
    SimulatorPeriodic(int street_length, int initial_cars, int vmax, int iterations, float dawdle_probability, bool always_unlimited, bool start_velocity_zero, bool multicore);
    ~SimulatorPeriodic();
// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

public:
    void perform_simulation() override;
    void perform_simulation_singlecore() override;
    void perform_simulation_multicore() override;

private:
    // Methods to perform simulation steps and print results to file
    void accelerate_cars(std::vector<Car*> &reading_street, std::vector<Car*> &writing_street, int start_index, int end_index) override;
    void decelerate_cars(std::vector<Car*> &reading_street, std::vector<Car*> &writing_street, int start_index, int end_index) override;
    void dawdle_cars(std::vector<Car*> &reading_street, std::vector<Car*> &writing_street, int start_index, int end_index, float dawdle_prob, std::mt19937 &rng) override;
    void move_cars(std::vector<Car*> &reading_street, std::vector<Car*> &writing_street, int start_index, int end_index) override;
    void print_street(std::vector<Car*>& street) override;
    void print_parameters() override; 
    // Methods to initialize the street
    void initialize_street() override;
    void fill_street(std::vector<Car*> &street);
};

#endif