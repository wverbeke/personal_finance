//Distribution to sample return rates from.
#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
class Distribution{
    public:
        // Not const since we might be doing internal updates to the distribution. for instance for
        // running averages or changing rates.
        virtual double draw_sample() = 0;
};
#endif
