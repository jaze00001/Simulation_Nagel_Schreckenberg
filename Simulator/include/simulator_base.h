#ifndef SIMULATOR_BASE_H
#define SIMULATOR_BASE_H

#define EMPTY -1

#include "car.h"
#include <memory>
#include <map>


class SimulatorBase
{

// ##################################################################### //
// ############################# VARIABLES ############################# //
// ##################################################################### //

std::map<unsigned int, std::unique_ptr<Car>> ghost_cells;

// ##################################################################### //
// ############################## METHODS ############################## //
// ##################################################################### //

public:
    // Method to perform the simulation
    virtual void perform_simulation() = 0;
private:
    // Pure virtual methods to be implemented by the derived classes
    virtual void accelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) = 0;
    virtual void decelerate_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) = 0;
    virtual void dawdle_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index, float dawdle_prob, std::mt19937 &rng) = 0;
    virtual void move_cars(std::vector<std::unique_ptr<Car>> &reading_street, std::vector<std::unique_ptr<Car>> &writing_street, int start_index, int end_index) = 0;
    virtual void print_street(std::vector<std::unique_ptr<Car>>& street) = 0;
    // Methods to initialize the street(s)
    virtual void initialize_street() = 0;
};
#endif