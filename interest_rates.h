#ifndef INTEREST_RATE_H
#define INTEREST_RATE_H

#include "gaussian.h"

// Model interest rates with a mean-reverting Ornstein-Uhlenbeck Process.
class InterestRatesOU{
    public:
        // TODO: esetimate theta, mu, and sigma from Riksbanken data.
        // mu: mean interest rate.
        // theta: Mean reversion time to the mean interest rate.
        // sigma: Local interest rate volatility.
        InterestRatesOU(double current_rate, double mu, double theta, double sigma):
            _current_rate(current_rate),
            _mu(mu),
            _theta(theta),
            // TODO: Is there a better way to sample things?
            _noise_sampler(Gaussian(0, sigma))
        {}

        double sample_and_update(){
            // Note that the equation is simplified here, with the delta_t and sqrt(delta_t) terms
            // removed. This can be done assuming we only ever increment things by one month at a
            // time.
            _current_rate += (_theta*(_mu - _current_rate) + _noise_sampler.draw_sample());
            return _current_rate;
        }

    private:
        double _current_rate;
        double _mu;
        double _theta;
        Gaussian _noise_sampler;

};

// TODO: Implement interest rates using autoregressive intergated moving average models.
// TODO: Implement interest rates using a hidden markov model with a transition matrix between
// different models.

#endif
