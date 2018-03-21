#include "subpath.h"

subpath::subpath(){
    d_n_m_AoD=0;
    d_n_m_AoA=0;
    theta_n_m_AoD=0;
    theta_n_m_AoA=0;
    phi_n_m=0;
    ;
}

subpath::subpath(int pdd){
    d_n_m_AoD=0;
    d_n_m_AoA=0;
    theta_n_m_AoD=0;
    theta_n_m_AoA=0;
    phi_n_m=0;

}

subpath::subpath(const subpath& a){

    this->d_n_m_AoD=a.d_n_m_AoD;
    this->d_n_m_AoA=a.d_n_m_AoA;
    this->theta_n_m_AoD=a.theta_n_m_AoD;
    this->theta_n_m_AoA=a.theta_n_m_AoA;
    this->phi_n_m=a.phi_n_m;

}

subpath::~subpath(){

}

double subpath::get_d_AoA(){return this->d_n_m_AoA;}
double subpath::get_d_AoD(){return this->d_n_m_AoD;}
double subpath::get_theta_AoA(){return this->theta_n_m_AoA;}
double subpath::get_theta_AoD(){return this->theta_n_m_AoD;}
double subpath::get_phi(){return this->phi_n_m;}

void subpath::set_d_AoA(double value){ this->d_n_m_AoA=value;}
void subpath::set_d_AoD(double value){ this->d_n_m_AoD=value;}
void subpath::set_theta_AoA(double value){ this->theta_n_m_AoA=value;}
void subpath::set_theta_AoD(double value){ this->theta_n_m_AoD=value;}
void subpath::set_phi(double value){ this->phi_n_m=value;}

void subpath::operator =(const subpath& a){
    d_n_m_AoD=a.d_n_m_AoD;
    d_n_m_AoA=a.d_n_m_AoA;
    theta_n_m_AoD=a.theta_n_m_AoD;
    theta_n_m_AoA=a.theta_n_m_AoA;
    phi_n_m=a.phi_n_m;
}
