#include "path.h"

path::path(){

    for (int i=0; i<20; i++){
        subpath* wee=new subpath;
        subpath_base.push_back(*wee);
    }
    delta_n_AoD=0;
    delta_n_AoA=0;
    tao_n=0;
    P_n=0;

}
path::path(const path& temp){
    this->subpath_base=temp.subpath_base;
    this->delta_n_AoD=temp.delta_n_AoD;
    this->delta_n_AoA=temp.delta_n_AoA;
    this->tao_n=temp.tao_n;
    this->P_n=temp.P_n;

}
path::~path(){}

std::vector<subpath>& path::get_subpath_base(){return this->subpath_base;}
double path::get_delta_n_AoD(){return this->delta_n_AoD;}
double path::get_delta_n_AoA(){return this->delta_n_AoA;}
double path::get_tao_n(){return this->tao_n;}
double path::get_P_n(){return this->P_n;}

bool path::set_delta_n_AoD(double value){return this->delta_n_AoD=value;}
bool path::set_delta_n_AoA(double value){return this->delta_n_AoA=value;}
bool path::set_tao_n(double value){return this->tao_n=value;}
bool path::set_P_n(double value){return this->P_n=value;}

void path::operator =(const path& a){
    subpath_base=a.subpath_base;
    delta_n_AoD=a.delta_n_AoD;
    delta_n_AoA=a.delta_n_AoA;
    tao_n=a.tao_n;
    P_n=a.P_n;
}
