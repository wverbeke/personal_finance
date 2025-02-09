#include <iostream>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <vector>
#include <string>
#include <stdexcept>
#include "interest_rates.h"

std::vector<double> read_rates(const std::string& path){
    std::vector<double> ret;
    std::ifstream file(path);
    if(!file){
        throw std::runtime_error("File " + path + " does not exist.");
    }
    std::string line;
    while(std::getline(file, line)){
        std::istringstream iss(line);
        std::string date;
        double value;
        if(iss >> date >> value){

            // All values are in % units.
            ret.push_back(value/100);
        }
    }
    file.close();
    return ret;
}

void write_samples(const std::string& path, const std::vector<double>& samples){
    std::ofstream file(path);
    for(const auto& s: samples){
        file << s << "\n";
    }
    file.close();
}

const std::string rate_path = "data/riksbanken_ranta_cleaned.txt";

int main(){
    //Try to fit interest rates.
    auto true_interest = read_rates(rate_path);
    auto fit_interest = InterestRatesOU(1.0, 0, 0, 0.1);
    auto delta = fit_interest.optimize_gradient_descent(true_interest, 1e-8, 5e-6, 10000000);
    std::cout << "Convergence delta = " << delta << std::endl;
    std::cout << fit_interest << std::endl;
    
    // Simulate 10 random 10 year interest rate evolutions starting from 2.25% interest.
    for(unsigned e=0; e < 10; ++e){
        auto interest = InterestRatesOU(0.0225, fit_interest.mu(), fit_interest.theta(), fit_interest.sigma());
        std::vector<double> sampled_values(120);
        for(unsigned i=0; i < 120; ++i){
           sampled_values[i] = interest.sample_and_update();
        }
        write_samples("dumps/samples_" + std::to_string(e) + ".txt", sampled_values);
    }
    return 0;
}
