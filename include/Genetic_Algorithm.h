/** Header for the Genetic Algorithm library
 *
*/

#pragma once
#include <functional>
#include <vector>
#include <array>
#include <vector>


struct Algorithm_Parameters {
    int numPopulation;           // Maximum number of iterations
    int numParents;              // Number of parents selected for crossover
    int numOffspring;             // Number of offspring per generation
    int numGenerations;           // Number of generations the algorithm runs
    double crossoverProbability;  // Probability of crossover occurring
    double mutationRate;          // Mutation rate
    int num_cross;                // Number of crossover points
};

// Default algorithm parameters with default number of crossover points set to 4
#define DEFAULT_ALGORITHM_PARAMETERS Algorithm_Parameters{500, 200, 300, 1500, 0.8, 0.03, 3}

class GeneticAlgorithmUtils {
public:
    /**
     * @brief Displays a progress bar on the console.
     *
     * The progress bar visually represents the progress of an operation.
     *
     * @param progress A double value between 0.0 and 1.0 representing the completion percentage.
     */
    static void showProgress(double progress);

    /**
     * @brief Completes the progress bar by moving to a new line.
     *
     * This method is typically called after the progress has reached 100%.
     */
    static void completeProgressBar();

    /**
     * @brief Generates a random integer between min and max (inclusive).
     *
     * This method uses a Mersenne Twister pseudo-random generator seeded with a fixed value.
     *
     * @param min The minimum value of the random integer.
     * @param max The maximum value of the random integer.
     * @return A random integer between min and max.
     */
    static int randomInt(int min, int max);

    /**
     * @brief Determines whether a mutation should occur based on the mutation rate.
     *
     * This method generates a random double between 0.0 and 1.0 and compares it to the mutation rate.
     *
     * @param mutationRate A double value representing the probability of mutation (between 0.0 and 1.0).
     * @return True if a mutation should occur, otherwise false.
     */
    static bool shouldMutate(double mutationRate);

    /**
     * @brief Evaluate the fitness of the population.
     * 
     * @param population Pointer to the population array.
     * @param numPopulation Number of individuals in the population.
     * @param fitness Pointer to the fitness array.
     * @param vector_size Size of each individual vector.
     * @param func Function to evaluate the fitness.
     * @param validity Function to check the validity of an individual.
     */
    static void evaluateFitness(int** population, int numPopulation, double* fitness, int vector_size, double(&func)(int, int*), std::function<bool(int, int*)> validity);
    
    /**
     * @brief Initialize the population with only valid individuals.
     * 
     * @param population Pointer to the population array.
     * @param numPopulation Number of individuals in the population.
     * @param num_of_units Number of units in the circuit.
     * @param validity Function to check the validity of an individual.
     */
    static void initializeFixPopulation(int** population, int numPopulation, int num_of_units, std::function<bool(int, int*)> validity);
    
    /**
     * @brief Select parents for the next generation based on their fitness.
     * 
     * @param fitness Pointer to the fitness array.
     * @param numPopulation Number of individuals in the population.
     * @param numParents Number of parents to select.
     * @param idx Vector to store the indices of the selected parents.
     */
    static void selectParents(const double* fitness, int numPopulation, int numParents, std::vector<int>& idx);

    /**
     * @brief Selects parents using tournament selection.
     * @param fitness Fitness values of the population.
     * @param numPopulation Number of individuals in the population.
     * @param idx Indices of selected parents.
     * @param tournament_size Size of the tournament.
     */
    static void selectParentsTournament(const double* fitness, int numPopulation, std::vector<int>& idx, int tournament_size);

    /**
     * @brief Applies elitism to retain the best individuals.
     * @param population Current population.
     * @param fitness Fitness values of the population.
     * @param newPopulation New population after elitism.
     * @param numPopulation Number of individuals in the population.
     * @param numElites Number of elites to retain.
     */
    static void elitism(int** population, const double* fitness, int** newPopulation, int numPopulation, int numElites);

    /**
     * @brief Applies inversion mutation to an individual.
     * @param individual Individual to be mutated.
     * @param vector_size Size of the circuit vector.
     * @param mutationRate Mutation rate.
     */
    static void mutate_inversion(int* individual, int vector_size, double mutationRate);

    /**
     * @brief Applies delete and insert mutation to an individual.
     * @param vector_size Size of the circuit vector.
     * @param offspring Offspring to be mutated.
     * @param mutationRate Mutation rate.
     * @param N Range of random values.
     */
    static void mutate_delete_and_insert(int vector_size, int* offspring, double mutationRate, int N);

    /**
     * @brief Perform crossover to produce offspring from two parents.
     *
     * @param vector_size Size of each individual vector.
     * @param parent1 Pointer to the first parent.
     * @param parent2 Pointer to the second parent.
     * @param offspring Pointer to the offspring.
     */
    static void uniformCrossover(int vector_size, int* parent1, int* parent2, int* offspring );

    /**
    * @brief Perform crossover to produce offspring from two parents.
    *
    * @param vector_size Size of each individual vector.
    * @param parent1 Pointer to the first parent.
    * @param parent2 Pointer to the second parent.
    * @param offspring Pointer to the offspring.
    * @param numCross Number of crossover points.
    * @param crossoverProbability Probability of crossover occurring.
    */
    static void crossover_one_point(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability);

    /**
    * @brief Perform crossover to produce offspring from two parents.
    *
    * @param vector_size Size of each individual vector.
    * @param parent1 Pointer to the first parent.
    * @param parent2 Pointer to the second parent.
    * @param offspring Pointer to the offspring.
    * @param numCross Number of crossover points.
    * @param crossoverProbability Probability of crossover occurring.
    */
    static void crossover_two_point(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability);

    /**
     * @brief Perform crossover to produce offspring from two parents.
     * 
     * @param vector_size Size of each individual vector.
     * @param parent1 Pointer to the first parent.
     * @param parent2 Pointer to the second parent.
     * @param offspring Pointer to the offspring.
     * @param numCross Number of crossover points.
     * @param crossoverProbability Probability of crossover occurring.
     */
    static void crossover_multiple(int vector_size, int* parent1, int* parent2, int* offspring, int numCross, double crossoverProbability);
    
    /**
     * @brief Mutate an individual by randomly substituting one of its elements.
     * 
     * @param vector_size Size of the individual vector.
     * @param individual Pointer to the individual.
     * @param mutationRate Probability of mutation occurring.
     * @param N Range for random substitution.
     */
    static void mutate_substitution(int vector_size, int* individual, double mutationRate, int N);
};

/**
 * @brief Check if all elements in a vector are true.
 * 
 * @param vector_size Size of the vector.
 * @param vec Pointer to the vector.
 * @return true Always returns true.
 */
bool all_true(int vector_size, int* vector);

/**
 * @brief Optimization function using a genetic algorithm.
 * 
 * @param vector_size Size of the individual vector.
 * @param vec Pointer to the vector.
 * @param func Function to evaluate the fitness.
 * @param validity Function to check the validity of an individual.
 * @param parameters Parameters for the genetic algorithm.
 * @return int Returns 0 on success.
 */
int optimize(int vector_size, int* vector,
             double(&func) (int, int*),
             std::function<bool(int, int*)> validity = all_true,
             struct Algorithm_Parameters parameters = DEFAULT_ALGORITHM_PARAMETERS);