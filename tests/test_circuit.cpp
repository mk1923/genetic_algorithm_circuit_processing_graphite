#include <iostream>
#include "CCircuit.h"
#include "CUnit.h"

bool all_tests_passed = true;

/**
 * @brief Tests the Circuit class constructor.
 *
 * This test checks whether the Circuit constructor correctly initializes
 * the number of units.
 */
void test_CircuitConstructor() {
    Circuit circuit(4);
    if (circuit.units.size() == 4) {
        std::cout << "test_CircuitConstructor passed\n";
    } else {
        std::cout << "test_CircuitConstructor failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the Check_Validity function.
 *
 * This test includes two cases:
 * - Valid circuit vector.
 * - Invalid circuit vector with a self-loop.
 */
void test_CheckValidity() {
    // Test case 1: Valid circuit
    int valid_vector[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5};
    Circuit circuit1(4);
    if (circuit1.Check_Validity(13, valid_vector)) {
        std::cout << "test_CheckValidity: valid_vector passed\n";
    } else {
        std::cout << "test_CheckValidity: valid_vector failed\n";
        all_tests_passed = false;
    }

    // Test case 2: Invalid circuit (self-loop)
    int invalid_vector[] = {0, 1, 0, 2};
    Circuit circuit2(2);
    if (!circuit2.Check_Validity(4, invalid_vector)) {
        std::cout << "test_CheckValidity: invalid_vector passed\n";
    } else {
        std::cout << "test_CheckValidity: invalid_vector failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the get_input_units function.
 *
 * This test verifies that the get_input_units function correctly identifies
 * the input units for a specific unit in the circuit.
 */
void test_get_input_units() {
    Circuit circuit(3);
    circuit.units[0].conc_num = 2;
    circuit.units[1].inter_num = 2;
    circuit.units[2].tails_num = 2;

    circuit.get_input_units(circuit.units, 2);

    if (circuit.units[2].input_info.size() == 3) {
        std::cout << "test_get_input_units passed\n";
    } else {
        std::cout << "test_get_input_units failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the get_all_input_units function.
 *
 * This test checks if get_all_input_units correctly sets the input_info for
 * each unit in the circuit.
 */
void test_get_all_input_units() {
    Circuit circuit(3);
    circuit.units[0].conc_num = 1;
    circuit.units[1].inter_num = 2;
    circuit.units[2].tails_num = 0;

    circuit.get_all_input_units();

    bool passed = true;
    if (circuit.units[1].input_info.size() != 1) passed = false;
    if (circuit.units[2].input_info.size() != 1) passed = false;
    if (circuit.units[0].input_info.size() != 1) passed = false;

    if (passed) {
        std::cout << "test_get_all_input_units passed\n";
    } else {
        std::cout << "test_get_all_input_units failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the initialize_units function.
 *
 * This test ensures that the initialize_units function correctly initializes
 * all units in the circuit with the provided circuit vector and feed rates.
 */
void test_InitializeUnits() {
    int circuit_vector[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5};
    Circuit circuit(4);
    circuit.initialize_units(circuit_vector, 1000.0, 500.0);

    if (circuit.units[0].conc_num == 1 &&
        circuit.units[1].inter_num == 2 &&
        circuit.units[2].tails_num == 1 &&
        circuit.units[3].tails_num == 5 &&
        circuit.units[0].mark == false &&
        circuit.units[1].mark == false &&
        circuit.units[2].mark == false &&
        circuit.units[3].mark == false &&
        circuit.units[0].self_num == 0 &&
        circuit.units[1].self_num == 1 &&
        circuit.units[2].self_num == 2 &&
        circuit.units[3].self_num == 3 &&
        circuit.initial_F_g == 1000.0 &&
        circuit.initial_F_w == 500.0 &&
        circuit.first_feed == 0) {
        std::cout << "test_InitializeUnits passed\n";
    } else {
        std::cout << "test_InitializeUnits failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the update_final_output function.
 *
 * This test manually sets input flow rates and checks if the final output
 * values are correctly calculated by the update_final_output function.
 */
void test_UpdateFinalOutput() {
    // Initialize circuit and units
    int circuit_vector[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5};
    Circuit circuit(4);
    circuit.initialize_units(circuit_vector, 1000.0, 500.0);
    std::vector<CUnit> units = circuit.units;
    std::array<CUnit, 2> final_output;

    // Manually set input flow rates
    units[0].C_g = 100.0;
    units[0].C_w = 50.0;
    units[1].I_g = 200.0;
    units[1].I_w = 100.0;
    units[2].T_g = 300.0;
    units[2].T_w = 150.0;

    bool passed = true;

    // Test concentrate stream for unit 0
    final_output[0].input_info.emplace_back(0, 0);
    circuit.update_final_output(final_output, units);
    if (final_output[0].current_F_g != 100.0 || final_output[0].current_F_w != 50.0) {
        std::cout << "Concentrate stream failed: Expected (100, 50), got ("
                  << final_output[0].current_F_g << ", " << final_output[0].current_F_w << ")\n";
        passed = false;
    }

    // Test intermediate stream for unit 1
    final_output[0].input_info = {{1, 1}};
    final_output[0].current_F_g = final_output[0].current_F_w = 0.0;
    circuit.update_final_output(final_output, units);
    if (final_output[0].current_F_g != 200.0 || final_output[0].current_F_w != 100.0) {
        std::cout << "Intermediate stream failed: Expected (200, 100), got ("
                  << final_output[0].current_F_g << ", " << final_output[0].current_F_w << ")\n";
        passed = false;
    }

    // Test tails stream for unit 2
    final_output[1].input_info.emplace_back(2, 2);
    circuit.update_final_output(final_output, units);
    if (final_output[1].current_F_g != 300.0 || final_output[1].current_F_w != 150.0) {
        std::cout << "Tailings stream failed: Expected (300, 150), got ("
                  << final_output[1].current_F_g << ", " << final_output[1].current_F_w << ")\n";
        passed = false;
    }

    if (passed) {
        std::cout << "test_UpdateFinalOutput passed\n";
    } else {
        std::cout << "test_UpdateFinalOutput failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the iterate_units function.
 *
 * This test checks whether the iterate_units function correctly processes
 * the circuit and achieves convergence.
 */
void test_IterateUnits() {
    int circuit_vector[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5};
    Circuit circuit(4);
    circuit.initialize_units(circuit_vector, 1000.0, 500.0);

    // Call iterate_units and check for convergence
    circuit.iterate_units(100, 1.0e-6);

    if (circuit.check_convergence(1.0e-6)) {
        std::cout << "test_IterateUnits convergence passed\n";
    } else {
        std::cout << "test_IterateUnits convergence failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the evaluate_performance function.
 *
 * This test sets the final output values manually and verifies if the
 * evaluate_performance function calculates the correct performance value.
 */
void test_EvaluatePerformance() {
    // Initialize circuit and units
    int circuit_vector[] = {0, 1, 3, 3, 2, 2, 0, 4, 1, 1, 1, 0, 5};
    Circuit circuit(4);
    circuit.initialize_units(circuit_vector, 1000.0, 500.0);
    std::vector<CUnit> units = circuit.units;
    std::array<CUnit, 2> final_output;

    // Manually set final output
    final_output[0].current_F_g = 150.0; // Gerardium
    final_output[0].current_F_w = 30.0;  // Waste
    circuit.final_output[0].current_F_g = final_output[0].current_F_g;
    circuit.final_output[0].current_F_w = final_output[0].current_F_w;

    // Compare the output with correct value
    double performance = circuit.evaluate_performance();
    double expected_performance = 100 * 150.0 - 750 * 30.0;

    if (std::abs(performance - expected_performance) < 1e-6) {
        std::cout << "test_EvaluatePerformance passed\n";
    } else {
        std::cout << "test_EvaluatePerformance failed: Expected "
                  << expected_performance << ", got " << performance << "\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the check_convergence function.
 *
 * This test verifies whether the check_convergence function correctly
 * identifies if the circuit has converged based on the tolerance value.
 */
void test_check_convergence() {
    Circuit circuit(2);
    circuit.units[0].current_F_g = 100;
    circuit.units[0].previous_F_g = 100;
    circuit.units[0].current_F_w = 50;
    circuit.units[0].previous_F_w = 50;

    circuit.units[1].current_F_g = 100;
    circuit.units[1].previous_F_g = 100.01;  // Slightly different
    circuit.units[1].current_F_w = 50;
    circuit.units[1].previous_F_w = 50;

    if (!circuit.check_convergence(0.01)) {
        std::cout << "test_check_convergence passed\n";
    } else {
        std::cout << "test_check_convergence failed\n";
        all_tests_passed = false;
    }
}

/**
 * @brief Tests the update_input function.
 *
 * This test sets up the input flow rates and input_info for each unit and
 * checks if the update_input function correctly updates the current flow rates.
 */
void test_update_input() {
    Circuit circuit(3);
    circuit.units[0].C_g = 100;
    circuit.units[0].C_w = 50;
    circuit.units[1].I_g = 200;
    circuit.units[1].I_w = 100;
    circuit.units[2].T_g = 300;
    circuit.units[2].T_w = 150;

    // Set input_info for each unit
    circuit.units[0].input_info = {{1, 1}, {2, 2}}; // Unit 0 gets I_g and I_w from unit 1 and T_g and T_w from unit 2
    circuit.units[1].input_info = {{0, 0}}; // Unit 1 gets C_g and C_w from unit 0
    circuit.units[2].input_info = {{1, 1}}; // Unit 2 gets I_g and I_w from unit 1

    circuit.update_input(circuit.units);

    // Expected results:
    // Unit 0: I_g from unit 1 (200) + T_g from unit 2 (300) => current_F_g = 500
    //         I_w from unit 1 (100) + T_w from unit 2 (150) => current_F_w = 250
    // Unit 1: C_g from unit 0 (100)                        => current_F_g = 100
    //         C_w from unit 0 (50)                         => current_F_w = 50
    // Unit 2: I_g from unit 1 (200)                        => current_F_g = 200
    //         I_w from unit 1 (100)                        => current_F_w = 100

    if (circuit.units[0].current_F_g == 500 &&
        circuit.units[0].current_F_w == 250 &&
        circuit.units[1].current_F_g == 100 &&
        circuit.units[1].current_F_w == 50 &&
        circuit.units[2].current_F_g == 200 &&
        circuit.units[2].current_F_w == 100) {
        std::cout << "test_update_input passed\n";
    } else {
        std::cout << "test_update_input failed\n";
        all_tests_passed = false;
    }
}

int main() {
    test_CircuitConstructor();
    test_CheckValidity();
    test_get_input_units();
    test_get_all_input_units();
    test_InitializeUnits();
    test_UpdateFinalOutput();
    test_IterateUnits();
    test_EvaluatePerformance();
    test_check_convergence();
    test_update_input();

    if (all_tests_passed) {
        std::cout << "All tests passed\n";
    } else {
        std::cout << "Some tests failed\n";
    }

    return all_tests_passed ? 0 : 1;
}
