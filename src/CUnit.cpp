#include "../include/CUnit.h"
#include <iostream>

void CUnit::calculate_recovery_rates() {
    auto calc_recovery = [this](double k_c, double k_i) { return (k_c * tau) / (1 + (k_c + k_i) * tau); };
    R_C_G = calc_recovery(k_c_g, k_i_g);
    R_I_G = calc_recovery(k_i_g, k_c_g);
    R_C_W = calc_recovery(k_c_w, k_i_w);
    R_I_W = calc_recovery(k_i_w, k_c_w);
}

void CUnit::calculate_tau(double F_g, double F_w) {
    this->tau = this->phi * this->V / ((F_g + F_w) / this->rho);
}

void CUnit::initialise_flow(double initial_F_g, double initial_F_w) {
    current_F_g = initial_F_g;
    current_F_w = initial_F_w;
}

void CUnit::save_current_input_flow() {
    previous_F_g = current_F_g;
    previous_F_w = current_F_w;
}

void CUnit::reset_input_flow() {
    current_F_g = 0.0;
    current_F_w = 0.0;
}

void CUnit::visualize() const {
    std::cout << "CUnit:" << self_num << std::endl;
    std::cout << "Recovery rates: " << R_C_G << " " << R_I_G << " " << R_C_W << " " << R_I_W << std::endl;
    std::cout << "Flow rates: " << C_g << " " << C_w << " " << I_g << " " << I_w << " " << T_g << " " << T_w << std::endl;
    std::cout << "Input info: ";
    for (auto& info : input_info) {
        std::cout << info.first << " " << info.second << " ";
    }
    std::cout << std::endl;
    std::cout << "Current flow rates: " << current_F_g << " " << current_F_w << std::endl;
    std::cout << "Previous flow rates: " << previous_F_g << " " << previous_F_w << std::endl;
    std::cout << "Tau: " << tau << std::endl;
}
