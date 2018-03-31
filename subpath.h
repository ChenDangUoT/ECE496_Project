#ifndef SUBPATH_H_INCLUDED
#define SUBPATH_H_INCLUDED




#include <cmath>
#include <vector>
#include <iostream>


class subpath
{
public:
    //delta_n_AoD is the AoD for the nth path with respect to the line of sight AoD theta_0

    //offste of the mth subpath of the nth path with respect to delta_n_AoD
    double d_n_m_AoD;

    //offset with respect to delta_n_AoA
    double d_n_m_AoA;

    //absolute AoD for the mth subpath of the nth path at BS with respect to BS broadside
    double theta_n_m_AoD;

    //at MS with respect to BS broadside
    double theta_n_m_AoA;

    //phase of the nth subpath of the mth path
    double phi_n_m;


    subpath();
    subpath(int pdd);
    subpath(const subpath& a);
    ~subpath();

    double get_d_AoA();
    double get_d_AoD();
    double get_theta_AoA();
    double get_theta_AoD();
    double get_phi();

    void set_d_AoA(double value);
    void set_d_AoD(double value);
    void set_theta_AoA(double value);
    void set_theta_AoD(double value);
    void set_phi(double value);

    void operator =(const subpath& a);


};

#endif // SUBPATH_H_INCLUDED
