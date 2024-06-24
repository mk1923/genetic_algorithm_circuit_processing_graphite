#include <iostream>
#include "../include/Genetic_Algorithm.h"
#include "../include/CCircuit.h"
#include <omp.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <limits>
#include <functional>
#include <array>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "../include/CUnit.h"
#include "../include/CCircuit.h"
#include "../include/CSimulator.h"
#include "../include/Genetic_Algorithm.h"
#include "../include/hyper.h"

Algorithm_Parameters gridSearch::hyperParameterSearch(int numIterations, int* vector, int vectorSize) {
    double bestFitness = -std::numeric_limits<double>::infinity();
    int bestNumPopulation = 0;
    int bestNumGen = 0;
    double bestMutationRate = 0.0;
    double bestCrossoverProbability = 0.0;
    int bestNumCross = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> popDist(100, 2000); // Population size range
    std::uniform_int_distribution<> genDist(50, 500); // Generations range
    std::uniform_real_distribution<> mutDist(0.01, 0.3); // Mutation rate range
    std::uniform_real_distribution<> crossProbDist(0.1, 0.9); // Crossover probability range
    std::uniform_int_distribution<> crossDist(1, 10); // Number of crossover points

    for (int i = 0; i < numIterations; ++i) {
        Algorithm_Parameters params;
        params.numPopulation = popDist(gen);
        params.numParents = params.numPopulation*0.4;
        params.numOffspring = params.numPopulation*0.6;
        params.numGenerations = genDist(gen);
        params.mutationRate = mutDist(gen);
        params.crossoverProbability = crossProbDist(gen);
        params.num_cross = crossDist(gen);

        //print params
        std::cout << "Population: " << params.numPopulation
                  << ", Generations: " << params.numGenerations
                  << ", Mutation Rate: " << params.mutationRate
                  << ", Crossover Probability: " << params.crossoverProbability
                  << ", Num Cross: " << params.num_cross << std::endl;

        Circuit circuit((vectorSize - 1) / 3);

        double start = omp_get_wtime();
        optimize(vectorSize, vector, Evaluate_Circuit,
                 [&circuit](int size, int* vec) { return circuit.Check_Validity(size, vec); },
                 params);
        double finish = omp_get_wtime();

        double fitness = Evaluate_Circuit(vectorSize, vector);
        std::cout << "Iteration: " << i 
                  << ", Fitness: " << fitness 
                  << ", Population: " << params.numPopulation
                  << ", Generations: " << params.numGenerations
                  << ", Mutation Rate: " << params.mutationRate
                  << ", Crossover Probability: " << params.crossoverProbability
                  << ", Num Cross: " << params.num_cross
                  << ", Time: " << finish - start << " seconds" << std::endl;

        if (fitness > bestFitness) {
            bestFitness = fitness;
            bestNumPopulation = params.numPopulation;
            bestNumGen = params.numGenerations;
            bestMutationRate = params.mutationRate;
            bestCrossoverProbability = params.crossoverProbability;
            bestNumCross = params.num_cross;
        }
    }

    std::cout << "Best Parameters: " 
              << "Population: " << bestNumPopulation 
              << ", Generations: " << bestNumGen 
              << ", Mutation Rate: " << bestMutationRate 
              << ", Crossover Probability: " << bestCrossoverProbability 
              << ", Num Cross: " << bestNumCross 
              << ", Fitness: " << bestFitness << std::endl;

    Algorithm_Parameters bestParams{bestNumGen, bestNumPopulation, bestNumPopulation, bestNumPopulation, bestCrossoverProbability, bestMutationRate, bestNumCross};

    return bestParams;
}