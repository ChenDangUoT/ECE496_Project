#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED





#include <cmath>
#include <vector>
#include "subpath.h"


class path
{
public:
    //vector of subpath for nth path
    std::vector<subpath> subpath_base;

    //delta_n_AoD is the AoD for the nth path with respect to the line of sight AoD theta_BS
    double delta_n_AoD;

    //AoA for the nth path with respect to LOS AoA theta_MS
    double delta_n_AoA;

    //delay for nth path, generated from tao_prime_n-tao_prime_1
    double tao_n;

    //normalized power for nth path, generated from P_prime_n
    double P_n;


    path();
    path(const path& temp);
    ~path();

    std::vector<subpath>& get_subpath_base();
    double get_delta_n_AoD();
    double get_delta_n_AoA();
    double get_tao_n();
    double get_P_n();

    bool set_delta_n_AoD(double value);
    bool set_delta_n_AoA(double value);
    bool set_tao_n(double value);
    bool set_P_n(double value);

    void operator =(const path& a);


};

#endif // PATH_H_INCLUDED
