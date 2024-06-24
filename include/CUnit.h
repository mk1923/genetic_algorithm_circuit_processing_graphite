/** Header for the unit class
 * 
 * 
 */

#pragma once

#include <vector>
#include <utility>


class CUnit {
    public:
    //index of the unit to which this unit’s concentrate stream is connected
    int conc_num;
    //index of the unit to which this unit’s intermediate stream is connected
    int inter_num;
    //index of the unit to which this unit’s tailings stream is connected
    int tails_num;
    //A Boolean that is changed to true if the unit has been seen
    bool mark;

    // Only for final output
    double concentrate = 0.0;
    double tailing = 0.0;

    // index of the unit itself
    int self_num;

    // Recovery rates
    double R_C_G; // Recovery rate for high grade concentrate Gerardium
    double R_C_W; // Recovery rate for high grade concentrate Waste
    double R_I_G; // Recovery rate for intermediate grade concentrate Gerardium
    double R_I_W; // Recovery rate for intermediate grade concentrate Waste

    // Flow out rates
    double C_g; // Concentrate Gerardium
    double C_w; // Concentrate Waste
    double I_g; // Intermediate Gerardium
    double I_w; // Intermediate Waste
    double T_g; // Tails Gerardium
    double T_w; // Tails Waste

    // The source of feed in data
    std::vector<std::pair<int, int> > input_info;

    // The feed in rate
    double current_F_g;
    double current_F_w;
    double previous_F_g;
    double previous_F_w;

    // The residence time
    double tau;

    // The rate constants
    const double k_c_g = 0.004; // Gerardium's rate constant for high grade concentrate
    const double k_i_g = 0.001; // Gerardium's rate constant for intermediate grade concentrate
    const double k_c_w = 0.0002; // Waste's rate constant for high grade concentrate
    const double k_i_w = 0.0003; // Waste's rate constant for intermediate grade concentrate
    double V = 10; // The volume of the cell
    double rho = 3000; // The density of the solids
    double phi = 0.1; // The volume fraction solids

    CUnit() : self_num(-1), conc_num(-1), inter_num(-1), tails_num(-1), mark(false), R_C_G(0.0), R_C_W(0.0),
        R_I_G(0.0), R_I_W(0.0), current_F_g(0.0), current_F_w(0.0), previous_F_g(0.0), previous_F_w(0.0),
        C_g(0.0), C_w(0.0), I_g(0.0), I_w(0.0), T_g(0.0), T_w(0.0), tau(0.0) {}

    /**
     * @brief Calculates the recovery rates for the CUnit.
     *
     * The recovery rates are calculated using the formula:
     * R = (k_c * tau) / (1 + (k_c + k_i) * tau)
     * Where:
     * - k_c is the capture rate
     * - k_i is the interference rate
     * - tau is the residence time
     */
    void calculate_recovery_rates();

    /**
     * @brief Calculates the residence time (tau) for the CUnit.
     *
     * Tau is calculated using the formula:
     * tau = (phi * V) / ((F_g + F_w) / rho)
     * Where:
     * - phi is the porosity
     * - V is the volume
     * - F_g is the gas flow rate
     * - F_w is the water flow rate
     * - rho is the density
     *
     * @param F_g Gas flow rate
     * @param F_w Water flow rate
     */
    void calculate_tau(double F_g, double F_w);

    /**
     * @brief Initializes the flow rates for the CUnit.
     *
     * @param initial_F_g Initial gas flow rate
     * @param initial_F_w Initial water flow rate
     */
    void initialise_flow(double initial_feed_gerardium, double initial_feed_waste);

    /**
     * @brief Saves the current input flow rates for the CUnit.
     *
     * This method stores the current flow rates in previous flow rates variables.
     */
    void save_current_input_flow();

    /**
     * @brief Resets the input flow rates for the CUnit to zero.
     */
    void reset_input_flow();

    /**
     * @brief Visualizes the current state of the CUnit.
     *
     * This method prints the following information:
     * - Unit number
     * - Recovery rates
     * - Flow rates
     * - Input information
     * - Current and previous flow rates
     * - Tau (residence time)
     */
    void visualize() const;
};

