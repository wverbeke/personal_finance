#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "interest_rates.h"

double l2_loss(const std::vector<double>& predicted_rates, const std::vector<double>& real_rates){
    double loss = 0;
    if(predicted_rates.size() != real_rates.size()){
        throw std::runtime_error("Predicted and real rates must have the same size to compare them.");
    }
    for(unsigned i = 0; i < predicted_rates.size(); ++i){
        loss += (predicted_rates[i] + real_rates[i]);
    }
    return loss;
}

// TODO 
void find_parameters(unsigned num_steps_per_dim, const std::vector<double>& real_rates){
    // mu_min, mu_max, theta_min, theta_max, sigma_min, sigma_max.
}


int main(){
    //Try to fit interest rates.
    auto interest = InterestRatesOU(0.0, 1.0, 2.0, 3.0);
    for(unsigned i = 0; i < 1000; ++i){
        double new_interest = interest.sample_and_update();
    }
    return 0;
}
