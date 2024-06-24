#include "../include/CUnit.h"
#include "../include/CCircuit.h"
#include "../include/CSimulator.h"
 
#include <iostream>
#include <cmath>
 
struct Circuit_Parameters default_circuit_parameters = {1e-6, 1000};
 
double Evaluate_Circuit(int vector_size, int* circuit_vector) {
    // Initialize the circuit
    Circuit circuit(((vector_size-1)/3));
 
    // Given the initial guess
    double initial_feed_gerardium = 10.0; // initial guess for F_g
    double initial_feed_waste = 90.0; // initial guess for F_w
    circuit.initialize_units(circuit_vector, initial_feed_gerardium, initial_feed_waste);
 
    // Define the maximum number of iterations and the tolerance
    int max_iterations = default_circuit_parameters.max_iterations;
    double tolerance = default_circuit_parameters.tolerance;
 
    //  Iterate the units
    circuit.iterate_units(max_iterations, tolerance);
 
    // Evaluate the performance of the circuit
    double performance = circuit.evaluate_performance();

    return performance;
}
 
// Other functions and variables to evaluate a real circuit.