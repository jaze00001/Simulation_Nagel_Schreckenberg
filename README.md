# Simulation_Nagel_Schreckenberg
A project implementing the Nagel-Schreckenberg traffic flow model in C++, integrated with a Python-based GUI for user interaction and configuration. The GUI collects user inputs, executes the simulation, and visualizes the results.
The goal is to simulate and observe traffic flow dynamics on a road section of configurable length, divided into discrete cells. Each cell represents a length of approximately 7.5 meters, corresponding to the average space a car occupies, including the necessary distance to the next car.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Key Features:

Traffic Simulation: A robust implementation of the Nagel-Schreckenberg traffic flow model in C++.

Interactive GUI: Developed in Python to allow users to configure parameters like road length, car density, maximum speed, and dawdling probability.

Visualization: Track the evolution of traffic throughout the simulation with automatically generated graphs.

Dynamic Speed Updates: Cars adjust their speed every second according to defined rules of acceleration, deceleration, and dawdling.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Traffic Model Overview
The simulation operates on a road section divided into cells. Each cell can either be empty or occupied by one car. The cars' positions and speeds are updated in discrete time steps based on the following rules:

Acceleration:
If the car's current speed is less than its maximum speed, increase the speed by 1 (up to the maximum).

Deceleration:
If a car’s current speed would cause it to crash into the car ahead, reduce the speed to match the number of free cells between the two cars.

Dawdling:
With a probability p, reduce the car's speed by 1 to account for random behavior (e.g., hesitation).

Movement:
After applying the above rules, the car is moved forward by the number of cells equal to its speed.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Acknowledgments
This project is inspired by the original Nagel-Schreckenberg model, a well-known cellular automaton for traffic flow simulation, and serves as an educational tool for understanding traffic dynamics.