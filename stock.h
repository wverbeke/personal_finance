#ifndef STOCK_H
#define STOCK_H
#include <cstddef>
#include <stdexcept>
#include "distribution.h"

//One can have many stocks in the same company. All stocks in the same company follow the same
//trend. Stocks have a certain value, and rise and fall in worth. It's important that this
//correlation is modeled since it makes the whole portfolio one has more volatile.

// All purchases are assumed to be very small in scale so they do not affect the stock price.
class Stock{
    public:
        Stock(double unit_value, std::size_t number_owned, Distribution& update_distribution):
            _unit_value(unit_value),
            _num_owned(number_owned),
            _update_distribution(update_distribution)
        {}
        double buy_price(std::size_t num_to_buy){
            return num_to_buy * _unit_value;
        }
        void buy_additional(std::size_t num_to_buy){
            _num_owned += num_to_buy;
        }

        // Returns selling price.
        double sell(std::size_t num_to_sell){
            if(num_to_sell > _num_owned){
                throw std::runtime_error("One can not sell more of a stock than one owns.");
            }
            _num_owned -= num_to_sell;
            return num_to_sell*_unit_value;
        }

        double value() const{
            return _unit_value * _num_owned;
        }

        void monthly_update(){
            auto rate = _update_distribution.draw_sample();
            _unit_value = _unit_value*(1.0 + rate);
            if(_unit_value < 0){
                _unit_value = 0;
            }
        }

        double current_worth(){
            return _unit_value*_num_owned;
        }
            
    private:
        double _unit_value;
        std::size_t _num_owned;

        // This should become more sophisticated over time than just a guassian. Something with an
        // underlying trend that evolves over time would be good.
        Distribution& _update_distribution;
};
#endif
