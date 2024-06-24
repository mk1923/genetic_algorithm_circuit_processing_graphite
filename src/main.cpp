#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>

#include "../include/CUnit.h"
#include "../include/CCircuit.h"
#include "../include/CSimulator.h"
#include "../include/Genetic_Algorithm.h"
#include "../include/hyper.h"

#include <omp.h>

void writeVectorToFile(const std::string& filename, const int* vector, int size) {
    std::ofstream outfile;
    outfile.open(filename, std::ios::out | std::ios::trunc);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        outfile << vector[i];
        if (i != size - 1) { // don't print a comma after the last element
            outfile << ",";
        }
    }

    outfile.close();
    std::cout << "Vector written to " << filename << std::endl;
}

std::pair<int*, int> parseVectorInput(const std::string& input) {
    std::vector<int> vec;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        vec.push_back(std::stoi(item));
    }
    int vector_size = vec.size();
    int* vector = new int[vector_size];
    std::copy(vec.begin(), vec.end(), vector);
    return std::make_pair(vector, vector_size);
}


int main(int argc, char * argv[])
{

    // set things up
    int vector[] = {0, 1, 2, 2, 3, 3, 3, 2, 4, 1, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
                    10, 11, 12, 13, 14, 15, 16, 17, 18};
    int vector_size = 31;

//    // Uncomment this block to add user interface
//    int* vector = nullptr;
//    int vector_size = 0;
//    int unitCount;
//
//    std::cout << "Enter 1 to input the number of units, or 2 to input the initial vector: ";
//    int choice;
//    std::cin >> choice;
//
//    if (choice == 1) {
//        std::cout << "Enter the number of units: ";
//        std::cin >> unitCount;
//        vector_size = (unitCount * 3 + 1);
//        vector = new int[vector_size]();
//        std::cout << "Initialized vector with " << vector_size << " elements." << std::endl;
//    } else if (choice == 2) {
//        std::cout << "Enter the initial vector (comma separated values): ";
//        std::string input;
//        std::cin.ignore(); // Ignore the leftover newline character from previous input
//        std::getline(std::cin, input);
//        auto result = parseVectorInput(input);
//        vector = result.first;
//        vector_size = result.second;
//        // Check if vector size is valid
//        if ((vector_size - 1) % 3 != 0) {
//            std::cerr << "Invalid vector size. Size must be a positive integer * 3 + 1." << std::endl;
//            std::cerr << "Size entered: " << vector_size << std::endl;
//            delete[] vector; // clean up dynamic memory
//            return 1;
//        }
//
//        // Calculate the number of units
//        unitCount = (vector_size - 1) / 3;
//        std::cout << "Calculated number of units: " << unitCount << std::endl;
//    } else {
//        std::cerr << "Invalid choice." << std::endl;
//        return 1;
//    }
    // run your code
    // optimize(16, vector, Evaluate_Circuit);
    // or

    Circuit circuit((vector_size-1)/3);
    double start = omp_get_wtime();

//    // If you want to do grid search
//    Algorithm_Parameters parameters = gridSearch::hyperParameterSearch(10, vector, vector_size);
//    optimize(vector_size, vector, Evaluate_Circuit,
//             [&circuit](int size, int* vec) { return circuit.Check_Validity(size, vec);}, parameters);


    optimize(vector_size, vector, Evaluate_Circuit,
            [&circuit](int size, int* vec) { return circuit.Check_Validity(size, vec); });
    double finish = omp_get_wtime();
    std::cout << "Time: " << finish - start << std::endl;
    // generate final output, save to file, etc.
    std::cout << Evaluate_Circuit(vector_size, vector) << std::endl;

    for (int i = 0; i < vector_size; i++) {
        std::cout << vector[i] << " ";
    }

    // Write vector to file
    writeVectorToFile("../post_process/vector_data.txt", vector, vector_size);

    return 0;
}
