#ifndef INTEREST_RATE_H
#define INTEREST_RATE_H

#include "gaussian.h"
#include <ostream>

// Model interest rates with a mean-reverting Ornstein-Uhlenbeck Process.
class InterestRatesOU{
    friend std::ostream& operator <<(std::ostream&, const InterestRatesOU&);
    public:
        // TODO: esetimate theta, mu, and sigma from Riksbanken data.
        // mu: mean interest rate.
        // theta: Mean reversion time to the mean interest rate.
        // sigma: Local interest rate volatility.
        InterestRatesOU(double current_rate, double mu, double theta, double sigma):
            _current_rate(current_rate),
            _mu(mu),
            _theta(theta),
            _sigma(sigma),
            // TODO: Is there a better way to sample things?
            _noise_sampler(Gaussian(0, 1))
        {}

        double sample_and_update(){
            // Note that the equation is simplified here, with the delta_t and sqrt(delta_t) terms
            // removed. This can be done assuming we only ever increment things by one month at a
            // time.
            _current_rate += (_theta*(_mu - _current_rate) + _noise_sampler.draw_sample()*_sigma);
            return _current_rate;
        }

        double optimize_gradient_descent(
            const std::vector<double>& measured_rates,
            double convergence_threshold,
            double learning_rate,
            unsigned max_iterations=1e4
        );

    private:
        double _current_rate;
        double _mu;
        double _theta;
        double _sigma; // We could include this one in the Gaussian, but then we can not udpate it to fit it to data.
        Gaussian _noise_sampler;

        double log_likelihood_gradient_theta(const std::vector<double>& measured_rates) const;
        double log_likelihood_gradient_mu(const std::vector<double>& measured_rates) const;
        double log_likelihood_gradient_sigma(const std::vector<double>& measured_rates) const;
};

// Computation of the gradients.
// The update equation is R_t+1 = R_t + theta * (mu - R_t) + Z_t * sigma, where Z_t is a sample drawn from a 0 mean, unit-variance Gaussian.
// We can rewrite this to see that for the update Delta_t = R_t+1 - R_t = theta * (mu - R_t) + Z_t * sigma.
// In other words each Delta_t is a sample from a Gaussian N(theta*(mu - R_t), sigma) given R_t is known.
// Now we can compute the likelihood and log-likelihood of the interest rate updates:
// L(mu, theta, sigma | Delta_t) = 1/sqrt(2*pi*sigma^2) * exp(-(Delta_t - theta*(mu - R_t))^2 / 2*sigma^2)
// ln(L) = 

// Avoid zero division with sigma.
double _zero_safe(double sigma){
    return std::abs(sigma) > 1e-7 ? sigma : 1e-7;
}

double InterestRatesOU::log_likelihood_gradient_theta(const std::vector<double>& measured_rates) const{
    // Sum over all the observed interest rates.
    double sum = 0;
    double s = _zero_safe(_sigma);

    // TODO: Check that more than 1 interest rate is passed;
    for(unsigned i = 0; i < measured_rates.size() - 1; ++i){

        // TODO Cache this since it is the same every time.
        double r = measured_rates[i];
        double delta_r = measured_rates[i + 1] - measured_rates[i];
        sum += (_theta*_mu*_mu + _theta*r*r - _mu*delta_r - 2*_theta*_mu*r + r*delta_r);
    }
    sum /= (s*s);
    return sum/measured_rates.size();
}


double InterestRatesOU::log_likelihood_gradient_mu(const std::vector<double>& measured_rates) const{
    double sum = 0;
    double s = _zero_safe(_sigma);
    for(unsigned i = 0; i < measured_rates.size() - 1; ++i){
        double r = measured_rates[i];
        double delta_r = measured_rates[i + 1] - measured_rates[i];

        //factor 2 in denominator and numerator removed.
        //_sigma*_sigma can also be cached and shared between this and theta's gradient computation.
        sum += (_mu*_theta - delta_r - _theta*r);
    }
    sum *= (_theta/(s*s));
    return sum/measured_rates.size();
}

double InterestRatesOU::log_likelihood_gradient_sigma(const std::vector<double>& measured_rates) const{
    double sum = 0;
    double s = _zero_safe(_sigma);
    for(unsigned i = 0; i < measured_rates.size() - 1; ++i){

        // TODO cache.
        double r = measured_rates[i];
        double delta_r = measured_rates[i + 1] - measured_rates[i];

        //_sigma*_sigma can also be cached and shared between this and theta's gradient computation.
        sum += (1.0 - 1.0/(s*s) * (delta_r*delta_r + _theta*_theta*_mu*_mu + _theta*_theta*r*r - 2*_theta*_mu*delta_r - 2*_theta*_theta*_mu*r + 2*_theta*r*delta_r));
    }
    sum /= (s);
    return sum/measured_rates.size();
}

double InterestRatesOU::optimize_gradient_descent(const std::vector<double>& measured_rates, double convergence_threshold, double learning_rate, unsigned max_iterations){
    double delta = 1e12;
    std::size_t iteration = 0;
    while((delta > convergence_threshold) && (iteration < max_iterations)){
        // Compute all gradients before updating any of the values.
        double d_theta = log_likelihood_gradient_theta(measured_rates);
        double d_mu = log_likelihood_gradient_mu(measured_rates);
        double d_sigma = log_likelihood_gradient_sigma(measured_rates);
        delta = (std::abs(d_theta) + std::abs(d_mu) + std::abs(d_sigma));
        _theta -= learning_rate*d_theta;
        _mu -= learning_rate*d_mu;
        _sigma -= learning_rate*d_sigma;
        ++iteration;
    }

    // See whether it converged or not.
    return delta;
}

// Warning: These equations should change if the noise is sampled in a nongaussian way!
// Our update equations look 


// TODO: Implement interest rates using autoregressive intergated moving average models.
// TODO: Implement interest rates using a hidden markov model with a transition matrix between
// different models.

std::ostream& operator<<(std::ostream& os, const InterestRatesOU& interest){
    os << "current rate = " << interest._current_rate << " | theta = " << interest._theta << " | mu = " << interest._mu << " | sigma = " << interest._sigma;
    return os;
}

#endif
