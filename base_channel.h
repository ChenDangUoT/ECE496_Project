#ifndef BASE_CHANNEL_H_INCLUDED
#define BASE_CHANNEL_H_INCLUDED





#include <random>
#include <cmath>
#include <vector>
#include <iostream>
#include "path.h"

bool abssmaller(double i, double j);

class base_channel
{
public:
    std::vector<path> path_base;

    //array orientation, defined as the difference between the broadside and the absolute north reference direction
    double omega_BS, omega_MS;

    //MS velocity vector
    double v, theta_v;

    //angle between broadside and LOS, LOS distance is d
    double theta_MS, theta_BS, d;

    //azimuth spread, delay spread and shadow fading distributions
    double sigma_AS, sigma_SF, sigma_DS;


    //default constructor, not used
    base_channel();

    //standard constructor
    base_channel(double distance, double theta_MS_in, double theta_BS_in);

    //copy constructor
    base_channel(const base_channel& a);

    virtual ~base_channel();

    void operator =(const base_channel& a);

    virtual void correlation_generator(std::default_random_engine& generator);
    virtual void set_d_n_m_AoD_from_table(path &this_path);
    virtual void set_d_n_m_AoA_from_table(path &this_path);





};

#endif // BASE_CHANNEL_H_INCLUDED
