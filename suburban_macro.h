#ifndef SUBURBAN_MACRO_H_INCLUDED
#define SUBURBAN_MACRO_H_INCLUDED




#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include "base_channel.h"

class suburban_macro:public base_channel
{
public:
    //default constructor, not used
    suburban_macro();

    //use this constructor
    suburban_macro(double distance, double theta_MS_in, double theta_BS_in);
    suburban_macro(const suburban_macro& a);

    ~suburban_macro();

    void correlation_generator(std::default_random_engine& generator);
    void set_d_n_m_AoD_from_table(path &this_path);
    void set_d_n_m_AoA_from_table(path &this_path);
};

#endif // SUBURBAN_MACRO_H_INCLUDED
