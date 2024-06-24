#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>
#include <limits>
#include <numeric>
#include <functional>


#include "../include/Genetic_Algorithm.h"
#include "../include/CCircuit.h"
#include <omp.h>

using namespace std;

void GeneticAlgorithmUtils::showProgress(double progress) {
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

void GeneticAlgorithmUtils::completeProgressBar() {
    std::cout << std::endl;
}

int GeneticAlgorithmUtils::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(1234);
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

bool GeneticAlgorithmUtils::shouldMutate(double mutationRate) {
    static std::random_device rd;
    static std::mt19937 gen(1234);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < mutationRate;
}

bool all_true(int vector_size, int* vec) {
    return true;
}

void GeneticAlgorithmUtils::evaluateFitness(int** population, int numPopulation, double* fitness, int vector_size, double(&func)(int, int*), std::function<bool(int, int*)> validity) {
//    #pragma omp parallel for
    for (int i = 0; i < numPopulation; ++i) {
        if (validity(vector_size, population[i])) {
            fitness[i] = func(vector_size, population[i]);
        } else {
            fitness[i] = -std::numeric_limits<double>::infinity();  // Set fitness to negative infinity for invalid solutions
        }
    }
}

void GeneticAlgorithmUtils::initializeFixPopulation(int** population, int numPopulation, int num_of_units, std::function<bool(int, int*)> validity) {
    int vector_size = num_of_units * 3 + 1;
//    #pragma omp parallel for
    for (int i = 0; i < numPopulation; ++i) {
        do {
            population[i][0] = GeneticAlgorithmUtils::randomInt(0, num_of_units);
            for (int j = 1; j < vector_size; ++j) {
                population[i][j] = GeneticAlgorithmUtils::randomInt(0, num_of_units + 1);
            }
        } while (!validity(vector_size, population[i]));
    }
}

void GeneticAlgorithmUtils::selectParents(const double* fitness, int numPopulation, int numParents, std::vector<int>& idx) {
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) { return fitness[a] > fitness[b]; });
    idx.resize(numParents);  // Select the top numParents individuals
}

void GeneticAlgorithmUtils::uniformCrossover(int vector_size, int* parent1, int* parent2, int* offspring ) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);  // Distribution to decide gene inheritance
    for (int i = 0; i < vector_size; ++i) {
        offspring[i] = (dis(gen) == 0) ? parent1[i] : parent2[i];
    }
}

void GeneticAlgorithmUtils::crossover_two_point(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    if (dis(gen) < crossoverProbability) {
        int crossoverPoint1 = GeneticAlgorithmUtils::randomInt(1, vector_size - 1);
        int crossoverPoint2 = GeneticAlgorithmUtils::randomInt(1, vector_size - 1);

        // Ensure crossoverPoint1 is less than crossoverPoint2
        if (crossoverPoint1 > crossoverPoint2) {
            std::swap(crossoverPoint1, crossoverPoint2);
        }

        for (int i = 0; i < crossoverPoint1; ++i) {
            offspring[i] = parent1[i];
        }
        for (int i = crossoverPoint1; i < crossoverPoint2; ++i) {
            offspring[i] = parent2[i];
        }
        for (int i = crossoverPoint2; i < vector_size; ++i) {
            offspring[i] = parent1[i];
        }
    } else {
        std::copy(parent1, parent1 + vector_size, offspring);
    }
}

void GeneticAlgorithmUtils::crossover_one_point(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    if (dis(gen) < crossoverProbability) {
        int crossoverPoint = GeneticAlgorithmUtils::randomInt(1, vector_size - 1);

        for (int i = 0; i < crossoverPoint; ++i) {
            offspring[i] = parent1[i];
        }
        for (int i = crossoverPoint; i < vector_size; ++i) {
            offspring[i] = parent2[i];
        }
    } else {
        std::copy(parent1, parent1 + vector_size, offspring);
    }
}

void GeneticAlgorithmUtils::crossover_multiple(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    if (dis(gen) < crossoverProbability) {
        vector<int> crossoverPoints(numCross);  // Custom number of crossover points
        for (int& point : crossoverPoints) {
            point = GeneticAlgorithmUtils::randomInt(1, vector_size - 1);
        }
        sort(crossoverPoints.begin(), crossoverPoints.end());

        bool fromParent1 = true;
        int lastCrossoverPoint = 0;
        for (int point : crossoverPoints) {
            for (int j = lastCrossoverPoint; j < point; ++j) {
                offspring[j] = fromParent1 ? parent1[j] : parent2[j];
            }
            fromParent1 = !fromParent1;
            lastCrossoverPoint = point;
        }
        for (int j = lastCrossoverPoint; j < vector_size; ++j) {
            offspring[j] = fromParent1 ? parent1[j] : parent2[j];
        }
    } else {
        copy(parent1, parent1 + vector_size, offspring);
    }
}

void GeneticAlgorithmUtils::mutate_substitution(int vector_size, int* individual, double mutationRate, int N) {
    if (GeneticAlgorithmUtils::shouldMutate(mutationRate)) {
        int pos = GeneticAlgorithmUtils::randomInt(0, vector_size - 1);
        individual[pos] = GeneticAlgorithmUtils::randomInt(0, N);
    }
}


void GeneticAlgorithmUtils::selectParentsTournament(const double* fitness, int numPopulation, std::vector<int>& idx, int tournament_size) {
    std::vector<int> population_indices(numPopulation);
    std::iota(population_indices.begin(), population_indices.end(), 0);

    for (int i = 0; i < idx.size(); ++i) {
        std::vector<int> tournament(tournament_size);
        for (int j = 0; j < tournament_size; ++j) {
            tournament[j] = population_indices[GeneticAlgorithmUtils::randomInt(0, numPopulation - 1)];
        }
        idx[i] = *std::max_element(tournament.begin(), tournament.end(), [&](int a, int b) {
            return fitness[a] < fitness[b];
        });
    }
}

void GeneticAlgorithmUtils::elitism(int** population, const double* fitness, int** newPopulation, int numPopulation, int numElites) {
    std::vector<int> eliteIndices(numPopulation);
    std::iota(eliteIndices.begin(), eliteIndices.end(), 0);
    std::partial_sort(eliteIndices.begin(), eliteIndices.begin() + numElites, eliteIndices.end(), [&](int a, int b) {
        return fitness[a] > fitness[b];
    });

    for (int i = 0; i < numElites; ++i) {
        std::copy(population[eliteIndices[i]], population[eliteIndices[i]] + numPopulation, newPopulation[i]);
    }
}

void GeneticAlgorithmUtils::mutate_inversion(int* individual, int vector_size, double mutationRate) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) < mutationRate) {
        int start = GeneticAlgorithmUtils::randomInt(0, vector_size - 2);
        int end = GeneticAlgorithmUtils::randomInt(start + 1, vector_size - 1);
        std::reverse(individual + start, individual + end + 1);
    }
}


void GeneticAlgorithmUtils::mutate_delete_and_insert(int vector_size, int* offspring, double mutationRate, int N) {
    if (GeneticAlgorithmUtils::shouldMutate(mutationRate)) {
        int deletePos = GeneticAlgorithmUtils::randomInt(0, vector_size - 1);
        std::copy(offspring + deletePos + 1, offspring + vector_size, offspring + deletePos);

        int insertPos = GeneticAlgorithmUtils::randomInt(0, vector_size - 2);
        int newValue = GeneticAlgorithmUtils::randomInt(0, N);

        std::copy_backward(offspring + insertPos, offspring + vector_size - 1, offspring + vector_size);
        offspring[insertPos] = newValue;
    }
}

int optimize(int vector_size, int* vec, double(&func)(int, int*), std::function<bool(int, int*)> validity, Algorithm_Parameters parameters) {
    int numPopulation = parameters.numPopulation;  // Total population size
    int numParents = parameters.numParents;  // Number of parents
    int numOffspring = parameters.numOffspring;  // Number of offspring per generation
    int numGen = parameters.numGenerations;  // Number of generations
    int numCross = parameters.num_cross;  // Number of crossover points
    double crossoverProbability = parameters.crossoverProbability;  // Crossover probability
    double mutationRate = parameters.mutationRate;  // Mutation rate
    int num_of_units = (vector_size - 1) / 3;
    std::cout<<"Parameters initialised"<<std::endl;

    // Allocate memory for the population
    int** population = new int*[numPopulation];
    for (int i = 0; i < numPopulation; ++i) {
        population[i] = new int[vector_size];
    }

    // Allocate memory for the fitness array
    auto* fitness = new double[numPopulation];

    // Initialize the population with valid individuals
    std::cout<<"Initialising population"<<std::endl;
    GeneticAlgorithmUtils::initializeFixPopulation(population, numPopulation, num_of_units, validity);

    // Evaluate the initial population's fitness
    GeneticAlgorithmUtils::evaluateFitness(population, numPopulation, fitness, vector_size, func, validity);

    std::cout<<"Running the genetic algorithm"<<std::endl;
    // Main loop of the genetic algorithm
    for (int generation = 0; generation < numGen; ++generation) {
        vector<int> idx(numPopulation);
        GeneticAlgorithmUtils::selectParents(fitness, numPopulation, numParents, idx);

        // Generate offspring
//        #pragma omp parallel for
        for (int i = 0; i < numOffspring; ++i) {
            int parent1 = idx[GeneticAlgorithmUtils::randomInt(0, numParents - 1)];
            int parent2 = idx[GeneticAlgorithmUtils::randomInt(0, numParents - 1)];
            GeneticAlgorithmUtils::crossover_multiple(vector_size, population[parent1], population[parent2], population[numPopulation - numOffspring + i], numCross, crossoverProbability);
//            GeneticAlgorithmUtils::uniformCrossover(vector_size, population[parent1], population[parent2], population[numPopulation - numOffspring + i]);
            GeneticAlgorithmUtils::mutate_substitution(vector_size, population[numPopulation - numOffspring + i], mutationRate, num_of_units + 1);
        }

        // Evaluate the new population's fitness
        GeneticAlgorithmUtils::evaluateFitness(population, numPopulation, fitness, vector_size, func, validity);

        // Sort the entire population based on fitness
        vector<int> sortedIdx(numPopulation);
        iota(sortedIdx.begin(), sortedIdx.end(), 0);
        sort(sortedIdx.begin(), sortedIdx.end(), [&](int a, int b) { return fitness[a] > fitness[b]; });

        // Keep the top numPopulation individuals
        int** newPopulation = new int*[numPopulation];
        auto* newFitness = new double[numPopulation];
//        #pragma omp parallel for
        for (int i = 0; i < numPopulation; ++i) {
            newPopulation[i] = population[sortedIdx[i]];
            newFitness[i] = fitness[sortedIdx[i]];
        }

        // Replace old population with the new one
        delete[] population;
        delete[] fitness;
        population = newPopulation;
        fitness = newFitness;

        GeneticAlgorithmUtils::showProgress((double)(generation + 1) / numGen);
    }
    GeneticAlgorithmUtils::completeProgressBar();
    // Find the best solution in the final population
    int bestIdx = distance(fitness, max_element(fitness, fitness + numPopulation));
    copy(population[bestIdx], population[bestIdx] + vector_size, vec);


    // Free the memory of the final population
    for (int i = 0; i < numPopulation; ++i) {
        delete[] population[i];
    }
    delete[] population;

    // Free the memory of the fitness array
    delete[] fitness;

    return 0;  // Success
}
