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
            ret.push_back(value);
        }
    }
    return ret;
}

const std::string rate_path = "data/riksbanken_ranta_cleaned.txt";

int main(){
    //Try to fit interest rates.
    auto true_interest = read_rates(rate_path);
    auto interest = InterestRatesOU(1.0, 1.0, 1.0, 1.0);
    auto delta = interest.optimize_gradient_descent(true_interest, 1e-8, 0.003, 10000000);
    std::cout << "Convergence delta = " << delta << std::endl;
    std::cout << interest << std::endl;
    return 0;
}
