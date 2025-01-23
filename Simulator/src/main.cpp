#include "simulator_periodic.h"
#include <iostream>
#include <chrono>

int main(int argc, char *argv[]) 
{
    // start the timer to measure the duration of the simulation
    auto start = std::chrono::high_resolution_clock::now();
    
    // check if the user provided the correct number of arguments
    if (argc == 9) // periodic boundary conditions
    {
        // parse the command line arguments and check their validity
        int street_length;
        int initial_cars;
        int vmax;
        int iterations;
        float dawdle_probability;
        bool always_unlimited;
        bool start_velocity_zero;
        bool multicore;

        try
        {
            street_length = std::stoi(argv[1]);
            initial_cars = std::stoi(argv[2]);
            vmax = std::stoi(argv[3]);
            iterations = std::stoi(argv[4]);
            dawdle_probability = std::stof(argv[5]);
            always_unlimited = (argv[6] == std::string("true"));
            start_velocity_zero = (argv[7] == std::string("true"));
            multicore = (argv[8] == std::string("true"));
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return 1;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "Argument out of range: " << e.what() << std::endl;
            return 1;
        }

        // check validity of the parameters
        if (street_length <= 0)
        {
            std::cerr << "Error: Street length must be greater than 0" << std::endl;
            return 1;
        }
        if (initial_cars < 0)
        {
            std::cerr << "Error: Number of initial cars must be greater than or equal to 0" << std::endl;
            return 1;
        }
        if (vmax != -1 && vmax < 0)
        {
            std::cerr << "Error: Maximum speed must be greater than 0 or equal to -1 (to mark unlimited speed limit)" << std::endl;
            return 1;
        }
        if (initial_cars > street_length)
        {
            std::cerr << "Error: Number of initial cars must be less than or equal to the street length" << std::endl;
            return 1;
        }
        if (iterations <= 0)
        {
            std::cerr << "Error: Number of iterations must be greater than 0" << std::endl;
            return 1;
        }
        if (dawdle_probability < 0 || dawdle_probability > 1)
        {
            std::cerr << "Error: Dawdle probability must be between 0 and 1" << std::endl;
            return 1;
        }

        // Create a new simulator object
        SimulatorPeriodic simulator(street_length, initial_cars, vmax, iterations, dawdle_probability, always_unlimited, start_velocity_zero, multicore);

        // Perform the simulation
        simulator.perform_simulation();
    }
     
    /*else if (argc != 12) // open boundary conditions
    {
        int street_length;
        int initial_cars;
        int vmax;
        int iterations;
        float dawdle_probability;
        float remove_probability;
        float insert_probability;
        int remove_space;
        bool always_unlimited;
        bool start_velocity_zero;
        bool multicore;

        try
        {
            street_length = std::stoi(argv[1]);
            initial_cars = std::stoi(argv[2]);
            vmax = std::stoi(argv[3]);
            iterations = std::stoi(argv[4]);
            dawdle_probability = std::stof(argv[5]);
            remove_probability = std::stof(argv[6]);
            insert_probability = std::stof(argv[7]);
            remove_space = std::stoi(argv[8]);
            always_unlimited = (argv[9] == std::string("true"));
            start_velocity_zero = (argv[10] == std::string("true"));
            multicore = (argv[11] == std::string("true"));
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return 1;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "Argument out of range: " << e.what() << std::endl;
            return 1;
        }

        // check validity of the parameters
        if (street_length <= 0)
        {
            std::cerr << "Error: Street length must be greater than 0" << std::endl;
            return 1;
        }
        if (initial_cars < 0)
        {
            std::cerr << "Error: Number of initial cars must be greater than or equal to 0" << std::endl;
            return 1;
        }
        if (vmax != -1 && vmax < 0)
        {
            std::cerr << "Error: Maximum speed must be greater than 0 or equal to -1 (to mark unlimited speed limit)" << std::endl;
            return 1;
        }
        if (initial_cars > street_length)
        {
            std::cerr << "Error: Number of initial cars must be less than or equal to the street length" << std::endl;
            return 1;
        }
        if (iterations <= 0)
        {
            std::cerr << "Error: Number of iterations must be greater than 0" << std::endl;
            return 1;
        }
        if (dawdle_probability < 0 || dawdle_probability > 1)
        {
            std::cerr << "Error: Dawdle probability must be between 0 and 1" << std::endl;
            return 1;
        }
        if (remove_probability < 0 || remove_probability > 1)
        {
            std::cerr << "Error: Remove probability must be between 0 and 1" << std::endl;
            return 1;
        }
        if (insert_probability < 0 || insert_probability > 1)
        {
            std::cerr << "Error: Insert probability must be between 0 and 1" << std::endl;
            return 1;
        }
        if (remove_space < 0 || remove_space >= street_length)
        {
            std::cerr << "Error: 0 < remove_space < street_length" << std::endl;
            return 1;
        }

        // TO BE IMPLEMENTED
    } */
    
    else // invalid number of arguments
    {
        std::cerr << "Usage for periodic boundary conditions: " << argv[0] 
              << " <street_length> <initial_cars> <vmax> <iterations> <dawdle_probability> <always_unlimited> <start_velocity_zero> <multicore>" 
              << std::endl;
        std::cerr << "Usage for open boundary conditions: " << argv[0] 
              << " <street_length> <initial_cars> <vmax> <iterations> <dawdle_probability> <remove_probability> <insert_probability> <remove_space> <always_unlimited> <start_velocity_zero> <multicore>" 
              << std::endl;
        return 1;
    }

    // if successful, print running time and return 0
    std::cout << "Simulation successful" << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Duration: " << duration << " ms" << std::endl;
    
    return 0;
}