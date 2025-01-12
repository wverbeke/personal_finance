#include <iostream>
#include <fstream>
#include "gaussian.h"
#include "stock.h"

int main(){
    auto g = Gaussian(0.02, 0.1);
    for(unsigned i = 0; i < 100; ++i){
        g.draw_sample();
    }
   
    //Simulate 10 evolutions of the stock over a year.
    std::ofstream test_dump("test_dump.txt");
    for(unsigned i = 0; i < 10; ++i){
        auto s = Stock(100, 10, g);
        test_dump << s.current_worth() << "\t";
        for(unsigned j = 0; j < 12; ++j){
            s.monthly_update();
            test_dump << s.current_worth() << "\t";
        }
        test_dump << "\n";
    }
    test_dump.close();

    return 0;
}
