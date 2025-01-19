#ifndef GAUSSIAN_H
#define GAUSSIAN_H
#include <random>
#include "distribution.h"

// TODO: Investigate how the random seeds work.
class Gaussian: public Distribution{
    public:
        Gaussian(const double mean, const double std):
            _dist(mean, std)
        {}

        double draw_sample(){
            return _dist(_random_engine);
        }

    private:
        // TODO Figure out what the best random generator here is.
        std::ranlux48 _random_engine;
        std::normal_distribution<double> _dist;
};
#endif
