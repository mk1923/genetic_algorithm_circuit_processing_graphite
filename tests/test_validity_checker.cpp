#include <iostream>
#include "CUnit.h"
#include "CCircuit.h"

/**
 * @brief Main function to run validity tests on the Circuit class.
 *
 * This function defines a set of valid and invalid test cases for circuit
 * vectors and checks the validity of each using the Circuit class.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return int Returns 0 if all tests pass, 1 otherwise.
 */
int main(int argc, char* argv[]) {
    // Flag to track overall test result
    int flag = 0;

    // Valid test cases
    int valid_1[] = {0, 1, 2, 2};
    int valid_2[] = {1, 2, 3, 5, 2, 3, 0, 4, 3, 3, 2, 2, 1};
    int valid_3[] = {0, 1, 3, 2, 4, 4, 3, 1, 3, 6, 1, 1, 0, 5, 1, 1};
    int valid_4[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 1, 0, 2, 6, 5, 0, 6};

    // Invalid test cases
    int invalid_1[] = {0};  // No unit
    int invalid_2[] = {0, 1, 0, 2};  // Self loop
    int invalid_3[] = {0, 1, 3, 2, 4, 4, 3, 1, 3, 4, 1, 1, 0, 5, 1, 1};  // Miss one product
    int invalid_4[] = {0, 1, 2, 2, 3, 3, 3};  // Flow to same destination
    int invalid_5[] = {0, 1, 2, 2, 3, 3, 3, 2};  // Wrong vector length
    int invalid_6[] = {1, 2, 3, 5, 2, 3, 0, 4, 3, 6, 2, 2, 1};  // Out of bounds
    int invalid_7[] = {1, 2, 3, 4, 2, 3, 0, 5, 3, 3, 2, 2, 1};  // Mix two destinations
    int invalid_8[] = {6, 2, 3, 5, 2, 3, 0, 4, 3, 3, 2, 2, 1};  // Feed num out of bounds
    int invalid_9[] = {0, 1, 1, 1, 1, 2, 2, 0, 3, 3, 3, 4, 4, 5, 5, 0, 6, 6, 6};  // Duplicate connections
    int invalid_10[] = {0, 1, 0, 2, 2, 2, 3, 3, 3, 4, 4, 4};  // More self loops
    int invalid_11[] = {0, 1, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12};  // Incorrect bounds check
    int invalid_12[] = {0, 1, 3, 2, 4, 4, 3, 1, 3, 6, 1, 1, 0, 5, 1, 1, 7, 8, 8, 7, 9, 3, 5};  // One product missing in larger circuit
    int invalid_13[] = {0, 1, 2, 3, 3, 1, 2, 0, 4, 4, 5, 5, 6, 6, 0, 7, 7, 8, 8, 9, 9};  // Cycle in graph
    int invalid_14[] = {0, 1, 2, 3, 3, 0, 2, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 0, 1};  // Feed num same as one of the outputs
    int invalid_15[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // All units pointing to same unit
    int invalid_16[] = {0, 1, 2, 3, 1, 4, 4, 0, 5, 1, 5, 6, 6, 7, 7, 0, 8, 2, 8, 3, 9, 1, 10};  // Invalid intermediate stream
    int invalid_17[] = {0, 1, 2, 2, 1, 3, 3, 0, 4, 1, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10};  // Multiple invalid connections
    int invalid_18[] = {0, 1, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10};  // Unreachable units
    int invalid_19[] = {0, 1, 2, 2, 1, 3, 3, 0, 4, 1, 4, 5, 5, 6, 6, 7, 7, 0, 8, 2, 8, 3, 9, 1, 11};  // Unit with no outputs
    int invalid_20[] = {0, 1, 2, 2, 1, 3, 3, 0, 4, 1, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10};  // Unit with invalid product connections

    /**
     * @brief Struct to hold test vector and expected result for validity tests.
     */
    struct Test_Validity {
        int* vector;  ///< Pointer to the test vector
        int size;  ///< Size of the test vector
        bool expected_result;  ///< Expected result of the validity test
    };

    // Array of test cases
    Test_Validity tests_validity[] = {
        {valid_1, sizeof(valid_1) / sizeof(valid_1[0]), true},
        {valid_2, sizeof(valid_2) / sizeof(valid_2[0]), true},
        {valid_3, sizeof(valid_3) / sizeof(valid_3[0]), true},
        {valid_4, sizeof(valid_4) / sizeof(valid_4[0]), true},

        {invalid_1, sizeof(invalid_1) / sizeof(invalid_1[0]), false},
        {invalid_2, sizeof(invalid_2) / sizeof(invalid_2[0]), false},
        {invalid_3, sizeof(invalid_3) / sizeof(invalid_3[0]), false},
        {invalid_4, sizeof(invalid_4) / sizeof(invalid_4[0]), false},
        {invalid_5, sizeof(invalid_5) / sizeof(invalid_5[0]), false},
        {invalid_6, sizeof(invalid_6) / sizeof(invalid_6[0]), false},
        {invalid_7, sizeof(invalid_7) / sizeof(invalid_7[0]), false},
        {invalid_8, sizeof(invalid_8) / sizeof(invalid_8[0]), false},
        {invalid_9, sizeof(invalid_9) / sizeof(invalid_9[0]), false},
        {invalid_10, sizeof(invalid_10) / sizeof(invalid_10[0]), false},
        {invalid_11, sizeof(invalid_11) / sizeof(invalid_11[0]), false},
        {invalid_12, sizeof(invalid_12) / sizeof(invalid_12[0]), false},
        {invalid_13, sizeof(invalid_13) / sizeof(invalid_13[0]), false},
        {invalid_14, sizeof(invalid_14) / sizeof(invalid_14[0]), false},
        {invalid_15, sizeof(invalid_15) / sizeof(invalid_15[0]), false},
        {invalid_16, sizeof(invalid_16) / sizeof(invalid_16[0]), false},
        {invalid_17, sizeof(invalid_17) / sizeof(invalid_17[0]), false},
        {invalid_18, sizeof(invalid_18) / sizeof(invalid_18[0]), false},
        {invalid_19, sizeof(invalid_19) / sizeof(invalid_19[0]), false},
        {invalid_20, sizeof(invalid_20) / sizeof(invalid_20[0]), false}
    };

    int num_tests = sizeof(tests_validity) / sizeof(tests_validity[0]);

    // Run each test case
    for (int i = 0; i < num_tests; i++) {
        Circuit circuit((tests_validity[i].size - 1) / 3);
        bool result = circuit.Check_Validity(tests_validity[i].size, tests_validity[i].vector);
        if (result == tests_validity[i].expected_result) {
            std::cout << "Test " << i + 1 << " passed\n";
        } else {
            std::cout << "Test " << i + 1 << " failed\n";
            flag = 1;
        }
    }

    // Output overall test result
    if (flag == 0) std::cout << "All tests passed\n";

    return flag; // Return 0 if all tests pass, 1 otherwise
}
