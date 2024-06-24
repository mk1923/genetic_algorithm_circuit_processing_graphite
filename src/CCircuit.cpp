#include <vector>
#include <stdio.h>

#include "../include/CUnit.h"
#include "../include/CCircuit.h"
#include <iostream>


Circuit::Circuit(int num_units) {
    this->units.resize(num_units);
}

bool Circuit::Check_Validity(int vector_size, int *circuit_vector, bool if_debug) {
    // initialize validation
    validation[0] = false;
    validation[1] = false;
    validation[2] = false;

    // check if there are no units, return false
    if (this->units.size() <= 0) {
        //cout why
        if (if_debug){
            std::cout << "No units in the circuit" << std::endl;
        }
        return false;
    }
    // check if vector_size is correct
    if (vector_size != 3 * this->units.size() + 1) {
        //print every element in the vector
        if (if_debug){
            std::cout << "Vector size is incorrect" << std::endl;
            for (int i = 0; i < vector_size; i++) {
                std::cout << circuit_vector[i] << " ";
            }
            std::cout << units.size() << std::endl;
            std::cout << vector_size << std::endl;
        }
        return false;
    }

    // get feed_num
    int feed_num = circuit_vector[0];
    // check if feed_num is valid: 0 <= feed_num < num_units
    if (feed_num < 0 || feed_num >= this->units.size()) {
        if (if_debug){
            std::cout << "Feed number is invalid" << std::endl;
        }
        return false;
    }

    // initialize connections for each unit
    for (int i = 0; i < this->units.size(); i++) {
        this->units[i].conc_num = circuit_vector[3 * i + 1];
        this->units[i].inter_num = circuit_vector[3 * i + 2];
        this->units[i].tails_num = circuit_vector[3 * i + 3];
        this->units[i].mark = false;
    }

    // mark units starting from feed_num
    mark_units(feed_num);

    // check if all units are marked
    for (int i = 0; i < units.size(); i++) {
        // if any unit is not marked, return false
        if (!this->units[i].mark) {
            if (if_debug){
                std::cout << "Not all units are connected" << std::endl;
            }
            return false;
        }
        // if any unit has self loop, return false
        if (this->units[i].conc_num == i || this->units[i].inter_num == i || this->units[i].tails_num == i) {
            if (if_debug){
                std::cout << "Unit " << i << " has self loop" << std::endl;
            }
            return false;
        }

        // if any unit has the same destination for all streams, return false
        if (this->units[i].conc_num == this->units[i].inter_num &&
            this->units[i].conc_num == this->units[i].tails_num &&
            this->units[i].inter_num == this->units[i].tails_num) {
            if (if_debug){
                std::cout << "Unit " << i << " has the same destination for all streams" << std::endl;
            }
            return false;
        }
        // ensure that the concentrate stream (conc_num) and tailings stream (tails_num) do not point to invalid units
        if (this->units[i].conc_num == this->units.size() + 1 || this->units[i].tails_num == this->units.size()) {
            if (if_debug){
                std::cout << "Unit " << i << " has invalid concentrate or tailings stream" << std::endl;
            }
            return false;
        }
        // bounds check
        if (this->units[i].conc_num < 0 || this->units[i].conc_num > this->units.size() + 1) {
            if (if_debug){
                std::cout << "Unit " << i << " has invalid concentrate stream range" << std::endl;
            }
            return false;
        }
        if (this->units[i].inter_num < 0 || this->units[i].inter_num > this->units.size() + 1) {
            if (if_debug){
                std::cout << "Unit " << i << " has invalid intermediate stream range" << std::endl;
            }
            return false;
        }
        if (this->units[i].tails_num < 0 || this->units[i].tails_num > this->units.size() + 1) {
            if (if_debug){
                std::cout << "Unit " << i << " has invalid tails stream range" << std::endl;
            }
            return false;
        }
        // check the validation results
        // if there is only one unit, ensure effective concentrated and tailing stream
        // if there are multiple units, should also ensure no intermediate flow directly to the product
        if (this->units.size() == 1) {
            if (!validation[0] || !validation[2])
            {
                if (if_debug){
                    std::cout << "Unit " << i << " has no ensure effective concentrated and tailing stream" << std::endl;
                }
                return false;
            }
        } else {
            if (!validation[0] || validation[1] || !validation[2])
            {
                if (if_debug){
                    std::cout << "Unit " << i << " has no ensure effective concentrated and tailing stream" << std::endl;
                }
                return false;
            }
        }
    }
    return true;
}

void Circuit::initialize_units(int* circuit_vector, double F_in_valuable, double F_in_waste)
{
    initial_F_g = F_in_valuable;
    initial_F_w = F_in_waste;
    auto vec = circuit_vector;
    first_feed = vec[0];
    for (int i = 0; i < units.size(); ++i) {
        units[i].self_num = i;
        units[i].conc_num = vec[i * 3 + 1];
        units[i].inter_num = vec[i * 3 + 2];
        units[i].tails_num = vec[i * 3 + 3];
        units[i].mark = false;
    }
}

void Circuit::get_input_units(std::vector<CUnit>& Units, int unit_num)
{
    for (int i = 0; i < Units.size(); ++i) {
        if (Units[i].conc_num == unit_num) {
            Units[unit_num].input_info.emplace_back(i, 0);  // 0 表示 concentrated
        }
        if (Units[i].inter_num == unit_num) {
            Units[unit_num].input_info.emplace_back(i, 1);  // 1 表示 intermediate
        }
        if (Units[i].tails_num == unit_num) {
            Units[unit_num].input_info.emplace_back(i, 2);  // 2 表示 tails
        }
    }
}

void Circuit::get_all_input_units()
{
    for (int i = 0; i < units.size(); ++i) {
        get_input_units(units, i);
    }
}

void Circuit::get_final_output_source(std::array<CUnit,2>& outputs, std::vector<CUnit>& Units) {
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < Units.size(); ++i) {
            if (Units[i].conc_num == Units.size() + j) {
                final_output[j].input_info.emplace_back(i, 0); // 0 represents concentrate stream
            }
            if (Units[i].inter_num == Units.size() + j) {
                final_output[j].input_info.emplace_back(i, 1); // 1 represents intermediate stream
            }
            if (Units[i].tails_num == Units.size() + j) {
                final_output[j].input_info.emplace_back(i, 2); // 2 represents tails stream
            }
        }
    }
}

void Circuit::update_final_output(std::array<CUnit,2>& fin_output, std::vector<CUnit>& cunits)
{
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < fin_output[j].input_info.size(); ++i) {
            int input_unit_num = fin_output[j].input_info[i].first;
            int input_type = fin_output[j].input_info[i].second;
            if (input_type == 0) {
                fin_output[j].current_F_g += cunits[input_unit_num].C_g;
                fin_output[j].current_F_w += cunits[input_unit_num].C_w;
            } else if (input_type == 1) {
                fin_output[j].current_F_g += cunits[input_unit_num].I_g;
                fin_output[j].current_F_w += cunits[input_unit_num].I_w;
            } else if (input_type == 2) {
                fin_output[j].current_F_g += cunits[input_unit_num].T_g;
                fin_output[j].current_F_w += cunits[input_unit_num].T_w;
            }
        }
    }
}

void Circuit::update_input(std::vector<CUnit>& units)
{
    for (int j = 0; j < units.size(); ++j) {
        int num_of_inputs = units[j].input_info.size();
        for (int i = 0; i < num_of_inputs; ++i) {
            int input_unit_num = units[j].input_info[i].first;
            int input_type = units[j].input_info[i].second;
            if (input_type == 0) {
                units[j].current_F_g += units[input_unit_num].C_g;
                units[j].current_F_w += units[input_unit_num].C_w;
            } else if (input_type == 1) {
                units[j].current_F_g += units[input_unit_num].I_g;
                units[j].current_F_w += units[input_unit_num].I_w;
            } else if (input_type == 2) {
                units[j].current_F_g += units[input_unit_num].T_g;
                units[j].current_F_w += units[input_unit_num].T_w;
            }
        }
    }
}

void Circuit::one_unit(CUnit& cunit)
{
    // calculate tau
    cunit.calculate_tau(cunit.current_F_g, cunit.current_F_w);

    // calculate recovery rates
    cunit.calculate_recovery_rates();


    // calculate flow out rates
    cunit.C_g = cunit.current_F_g* cunit.R_C_G;
    cunit.C_w = cunit.current_F_w* cunit.R_C_W;
    cunit.I_g = cunit.current_F_g* cunit.R_I_G;
    cunit.I_w = cunit.current_F_w* cunit.R_I_W;
    cunit.T_g = cunit.current_F_g* (1 - cunit.R_C_G - cunit.R_I_G);
    cunit.T_w = cunit.current_F_w* (1 - cunit.R_C_W - cunit.R_I_W);
}

void Circuit::iterate_units(int max_iterations, double tol)
{
    get_all_input_units();
    get_final_output_source(final_output, units);

    int iterations = 0;
    for (int i = 0; i < max_iterations; i++) {
        //std::cout << "Iteration: " << iterations << std::endl;
        for (auto& unit : units) {
            unit.save_current_input_flow();
            unit.reset_input_flow();
        }
        for (auto& output : final_output) {
            output.save_current_input_flow();
            output.reset_input_flow();
        }
        if (iterations == 0) {
            // use initial guess as the feed for every unit
            for (int i = 0; i < units.size(); i++) {
                units[i].initialise_flow(initial_F_g, initial_F_w);
            }
        }

        units[first_feed].initialise_flow(initial_F_g, initial_F_w);
        update_input(units);

        //print unit 0's input
        for (auto& unit : units) {
            if (unit.current_F_g + unit.current_F_w == 0) {
                continue;
            }
            one_unit(unit);
        }
        update_final_output(this->final_output, units);

        // visualise the units
        // for (auto& unit : units) {
        //     unit.visualize();
        // }
        converged = check_convergence(tol);
        if (converged) {
            break;
        }
        iterations++;
    }

}

void Circuit::mark_units(int unit_num) {
    if (this->units[unit_num].mark) return;
    this->units[unit_num].mark = true;

    // if conc_num points to another unit, recursively call mark_units
    if (this->units[unit_num].conc_num < this->units.size()) {
        mark_units(this->units[unit_num].conc_num);
    } else {
        validation[0] = true;
    }

    // if inter_num points to another unit, recursively call mark_units
    if (this->units[unit_num].inter_num < this->units.size()) {
        mark_units(this->units[unit_num].inter_num);
    } else {
        validation[1] = true;
    }

    // if tails_num points to another unit, recursively call mark_units
    if (this->units[unit_num].tails_num < this->units.size()) {
        mark_units(this->units[unit_num].tails_num);
    } else {
        validation[2] = true;
    }
}

bool Circuit::check_convergence(double tol) {
    for (const auto& unit : units) {
        if (std::abs(unit.current_F_g - unit.previous_F_g) > tol ||
            std::abs(unit.current_F_w- unit.previous_F_w) > tol) {
            return false;
        }
    }
    return true;
}

double Circuit::evaluate_performance(){

    double total_valuable = final_output[0].current_F_g;
    double total_waste = final_output[0].current_F_w;
//    double grade = total_valuable / (total_valuable + total_waste);

    return 100 * total_valuable - 750 * total_waste;
}
