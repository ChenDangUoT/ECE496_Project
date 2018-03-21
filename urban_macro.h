#ifndef URBAN_MACRO_H_INCLUDED
#define URBAN_MACRO_H_INCLUDED





#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include "base_channel.h"

class urban_macro:public base_channel
{
public:
    //default constructor, not used
    urban_macro();

    //use this constructor
    urban_macro(double distance, double theta_MS_in, double theta_BS_in);

    ~urban_macro();

    void correlation_generator(std::default_random_engine& generator);
    void set_d_n_m_AoD_from_table(path &this_path);
    void set_d_n_m_AoA_from_table(path &this_path);
};

#endif // URBAN_MACRO_H_INCLUDED
