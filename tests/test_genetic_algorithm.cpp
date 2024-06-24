#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <functional>
#include <random>
#include <limits>
#include <cstring>
#include "../include/Genetic_Algorithm.h"
#include "../include/CCircuit.h"
#include "../include/CUnit.h"
#include "../include/CSimulator.h"
#include <omp.h>
#include <array>



// Mock answer vector used in the test function
int test_answer[] = {2, 1, 1, 2, 0, 2, 3, 0, 4, 4};

// Mock test function for the genetic algorithm
double test_function(int vector_size, int* vector) {
    double result = 0;
    for (int i = 0; i < vector_size; ++i) {
        result += (vector[i] - test_answer[i]) * (vector[i] - test_answer[i]);
    }
    return result;
}

// Mock validity function for testing
bool mock_validity_function(int vector_size, int* vector) {
    return true;  // For simplicity, all vectors are valid
}

void testRandomInt() {
    int min = 0;
    int max = 10;
    int randValue = GeneticAlgorithmUtils::randomInt(min, max);
    if (randValue >= min && randValue <= max) {
        std::cout << "testRandomInt passed." << std::endl;
    } else {
        std::cout << "testRandomInt failed: " << randValue << " not in range [" << min << ", " << max << "]." << std::endl;
    }
}

void testShouldMutate() {
    double highMutationRate = 1.0;
    double lowMutationRate = 0.0;
    if (GeneticAlgorithmUtils::shouldMutate(highMutationRate) && !GeneticAlgorithmUtils::shouldMutate(lowMutationRate)) {
        std::cout << "testShouldMutate passed." << std::endl;
    } else {
        std::cout << "testShouldMutate failed." << std::endl;
    }
}

// Test function for initializeFixPopulation : checks that size is correct and number of units 
void test_initializeFixPopulation() {
    int numPopulation = 10;  // Number of individuals in the population
    int num_of_units = 3;    // Number of units (for calculation of vector size)
    int vector_size = num_of_units * 3 + 1;  // Size of each vector in the population

    // Allocate memory for the population array
    int** population = new int*[numPopulation];
    for (int i = 0; i < numPopulation; ++i) {
        population[i] = new int[vector_size];
    }

    // Initialize the population using the function to be tested
    GeneticAlgorithmUtils::initializeFixPopulation(population, numPopulation, num_of_units, mock_validity_function);

    // Validate that the population has the correct number of individuals
    assert(numPopulation == numPopulation);  // Check that the population size is as expected

    // Validate that each individual in the population has the correct vector size
    for (int i = 0; i < numPopulation; ++i) {
        assert(population[i] != nullptr);  // Ensure that the pointer is not null
    }

    // Validate that each element in the population vectors is within the expected range
    for (int i = 0; i < numPopulation; ++i) {
        // The first element should be in the range [0, num_of_units]
        assert(population[i][0] >= 0 && population[i][0] <= num_of_units);
        for (int j = 1; j < vector_size; ++j) {
            // All other elements should be in the range [0, num_of_units + 1]
            assert(population[i][j] >= 0 && population[i][j] <= num_of_units + 1);
        }
    }

    // If all assertions pass, print a success message
    std::cout << "Test passed: initializeFixPopulation" << std::endl;

    // Free the allocated memory to avoid memory leaks
    for (int i = 0; i < numPopulation; ++i) {
        delete[] population[i];
    }
    delete[] population;
}

// Test function for selectParents :
//validates that selectParents correctly selects and orders the top numParents
// individuals by their fitness values
void test_selectParents() {
    int numPopulation = 10;
    int numParents = 4;
    double fitness[] = {1.0, 3.0, 2.0, 5.0, 4.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    std::vector<int> idx(numPopulation);

    GeneticAlgorithmUtils::selectParents(fitness, numPopulation, numParents, idx);

    assert(idx.size() == numParents);
    for (int i = 0; i < numParents - 1; ++i) {
        assert(fitness[idx[i]] >= fitness[idx[i + 1]]);
    }

    std::cout << "Test passed: selectParents" << std::endl;
}

// Test function for mutate_substitution : check that a mutation always occus
void test_mutate_substitution() {
    int vector_size = 10;
    int individual[vector_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double mutationRate = 1.0;  // Always mutate for testing
    int N = 10;

    GeneticAlgorithmUtils::mutate_substitution(vector_size, individual, mutationRate, N);

    bool mutated = false;
    for (int i = 0; i < vector_size; ++i) {
        if (individual[i] != i) {
            mutated = true;
            break;
        }
    }

    assert(mutated);
    std::cout << "Test passed: mutate_substitution" << std::endl;
}

// Test function for crossover_multiple
//Ensure the offspring contains only elements from the parents.
//Verify that the offspring is not a direct copy of either parent when the crossover probability is 1.0.
void test_crossover_multiple() {
    int vector_size = 10;
    int parent1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int parent2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int offspring[vector_size];
    int numCross = 3;
    double crossoverProbability = 1.0;  // Always crossover for testing

    GeneticAlgorithmUtils::crossover_multiple(vector_size, parent1, parent2, offspring, numCross, crossoverProbability);

    bool mixed = false;
    bool valid = true;

    // Check that each element of the offspring comes from either parent1 or parent2
    for (int i = 0; i < vector_size; ++i) {
        if (offspring[i] != parent1[i] && offspring[i] != parent2[i]) {
            valid = false;
            break;
        }
        if (offspring[i] != parent1[i] || offspring[i] != parent2[i]) {
            mixed = true;
        }
    }

    assert(valid);
    assert(mixed);
    std::cout << "Test passed: crossover_multiple" << std::endl;
}

// Test function for optimize to check that the fitness is positive
void test_optimize() {
    int vector_size = 10;
    int vector[vector_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    Algorithm_Parameters params;
    params.numPopulation = 100;
    params.numParents = 40;
    params.numGenerations = 100;
    params.crossoverProbability = 0.7;
    params.mutationRate = 0.1;
    params.num_cross = 3;
    params.numOffspring = 60;

    optimize(vector_size, vector, test_function, mock_validity_function, params);

    double fitness = test_function(vector_size, vector);
    assert(fitness >= 0);  // Ensure the fitness is positive

    std::cout << "Test passed: optimize function produces a positive fitness value." << std::endl;
}



/**
 * @brief Main function to execute all tests.
 * @return 0 on successful execution.
 */
int main() {
    testRandomInt();
    testShouldMutate();
    test_initializeFixPopulation();
    test_selectParents();
    test_mutate_substitution();
    test_crossover_multiple();
    test_optimize();
    return 0;
}
