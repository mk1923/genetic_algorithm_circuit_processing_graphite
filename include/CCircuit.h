/** Header for the circuit class
 *
 * This header defines the circuit class and its associated functions
 *
*/

#pragma once

#include "CUnit.h"
#include <array>
#include <vector>

class Circuit {
  public:
    double initial_F_g; // Initial guess for the flow rate of valuable material in the initial feed
    double initial_F_w; // Initial guess for the flow rate of waste material in the initial feed
    int first_feed; // The first unit in the circuit that receives feed
    bool converged; // A boolean that is true if the circuit has converged

    /**
     * @brief Constructs a Circuit with a given number of units.
     * 
     * @param num_units The number of units in the circuit.
     */
    Circuit(int num_units);

    /**
     * @brief Checks the validity of the circuit configuration.
     * 
     * @param vector_size Size of the circuit vector.
     * @param circuit_vector Pointer to the circuit vector.
     * @param if_debug Boolean flag to print debug information.
     * @return true if the circuit is valid.
     * @return false if the circuit is invalid.
     * 
     * @note The function performs several checks to ensure the circuit configuration is valid.
     */
    bool Check_Validity(int vector_size, int *circuit_vector, bool if_debug = false);

    /**
     * @brief Initializes the units in the circuit.
     * 
     * @param circuit_vector Pointer to the circuit vector.
     * @param F_in_valuable Initial valuable flow rate.
     * @param F_in_waste Initial waste flow rate.
     * 
     * @note This function sets up the initial configuration of the circuit units.
     */
    void initialize_units(int *circuit_vector, double F_in_valuable, double F_in_waste);

    /**
     * @brief Updates the input flow rates for all units in the circuit.
     * 
     * @param units Vector of units in the circuit.
     * 
     * @note This function updates the current input flow rates for each unit based on its input units.
     */
    void update_input(std::vector<CUnit>& units);

    /**
     * @brief Calculates the flow rates for a single unit.
     * 
     * @param cunit Reference to the unit for which flow rates are calculated.
     * 
     * @note This function calculates the flow rates for concentrate, intermediate, and tails streams.
     */
    void one_unit(CUnit& cunit);

    /**
     * @brief Iterates through the units in the circuit until convergence or maximum iterations are reached.
     * 
     * @param max_iterations Maximum number of iterations.
     * @param tol Convergence tolerance.
     * 
     * @note This function simulates the circuit until the results converge or the maximum number of iterations is reached.
     */
    void iterate_units(int max_iterations, double tol);

    /**
     * @brief Checks if the circuit has converged based on the tolerance.
     * 
     * @param tol Convergence tolerance.
     * @return true if the circuit has converged.
     * @return false if the circuit has not converged.
     * 
     * @note This function checks the difference between the current and previous flow rates to determine convergence.
     */
    bool check_convergence(double tol);

    /**
     * @brief Retrieves all input units for a single unit in the circuit.
     * 
     * @param Units Vector of units in the circuit.
     * @param unit_num The unit number for which input units are retrieved.
     * 
     * @note This function populates the input_info of the specified unit with its input units.
     */
    void get_input_units(std::vector<CUnit>& Units, int unit_num);

    /**
     * @brief Retrieves all input units for all units in the circuit.
     * 
     * @note This function populates the input_info of all units in the circuit.
     */
    void get_all_input_units();

    /**
     * @brief Retrieves all input units for the final output in the circuit.
     * 
     * @param outputs Array of final output units.
     * @param Units Vector of circuit units.
     * 
     * @note This function populates the input_info of the final output units with the units that contribute to their input streams.
     */
    void get_final_output_source(std::array<CUnit,2>& outputs, std::vector<CUnit>& Units);

    /**
     * @brief Updates the final output with the geradium and waste received.
     * 
     * @param fin_output Array of final output units.
     * @param cunits Vector of circuit units.
     * 
     * @note This function updates the final output units with the flow rates from the input units.
     */
    void update_final_output(std::array<CUnit,2>& fin_output, std::vector<CUnit>& cunits);

    /**
     * @brief Evaluates the performance of the circuit.
     * 
     * @return double The performance value of the circuit.
     * 
     * @note This function calculates the performance of the circuit based on the flow rates of valuable and waste materials.
     */
    double evaluate_performance();

    std::vector<CUnit> units;
    std::vector<double> output;
    std::array<CUnit,2> final_output;

  private:
    /**
     * @brief Marks the units starting from a given unit number.
     * 
     * @param unit_num The starting unit number for marking.
     * 
     * @note This function recursively marks units connected to the given unit.
     */
    void mark_units(int unit_num);
//    std::vector<CUnit> units;
    bool validation[3]; // validation[0] for conc_num, validation[1] for inter_num, validation[2] for tails_num
};

