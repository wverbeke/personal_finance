// Class implementing a loan.
// This should be mostly relevant for one of more house/real estate mortgages one has ongoing.
#ifndef LOAN_H
#define LOAN_H

#include <stdexcept>
#include <cmath>

// TODO Make a class to model interest rates. The interest rate should vary over time, but
// variations should be relatively slow and strongly correlated over time.
//class InterestRate{
//
//};

class Loan{
    public:
        Loan(double total_amount):
            _remaining_amount(total_amount),
            _initial_amount(total_amount)
        {}

        bool fully_paid() const{
            return _remaining_amount <= 0.0;
        }

        double monthly_interest(double current_interest){
            return (_remaining_amount * current_interest / 12.0);
        }

        void pay_monthly(double amount, double current_interest){
            if(fully_paid()){
                throw std::runtime_error("The loan has been fully paid off.");
            }
            double interest = monthly_interest(current_interest);
            if(interest > amount){
                throw std::runtime_error("A loan payment must always be at least as large as the required interest payment.");
            }
            double principal_payment = amount - interest;
            if(_remaining_amount > principal_payment){
                _remaining_amount -= principal_payment;
            } else if(std::abs(_remaining_amount - principal_payment) < 1e-5){
                _remaining_amount = 0.0;

            // Money should not disappear.
            } else {
                throw std::runtime_error("You paid more than the remaining amount of debt in the loan.");
            }
        }

        double remaining() const{
            return _remaining_amount;
        }

        //It is common to have loan payments with a certain percentage of the original amount
        //needing to be paid off every year. For instance it could be 2%, i.e. 0.02 that has to be
        //paid of every year.
        void pay_monthly_fraction(double yearly_fraction, double current_interest){
            double principal_payment = (yearly_fraction * _initial_amount / 12);
            pay_monthly(principal_payment + monthly_interest(current_interest), current_interest);
        }

        //TODO: Make functionality that incorporates the required payoff percentages on Swedish
        //loans.


    private:
        double _remaining_amount;
        double _initial_amount;
};


#endif
