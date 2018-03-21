#include "base_channel.h"



//assuming uniform antenna gains

const double PI = std::atan(1) * 4;

bool abssmaller(double i, double j){return ((i*i)<(j*j));}


base_channel::base_channel(){
    //left blank
}

base_channel::base_channel(const base_channel& a){
    path_base=a.path_base;
    omega_BS=a.omega_BS;
    omega_MS=a.omega_MS;
    v=a.v;
    theta_v=a.theta_v;
    theta_MS=a.theta_MS;
    theta_BS=a.theta_BS;
    d=a.d;
    sigma_AS=a.sigma_AS;
    sigma_SF=a.sigma_SF;
    sigma_DS=a.sigma_DS;
}

base_channel::base_channel(double distance, double theta_MS_in, double theta_BS_in){

    //6 path components per channel
    path_base.reserve(6);

    //now set up the random engine here
    std::random_device channel_seed;
	std::default_random_engine generator(channel_seed());

	//then setup the random engine for angle and speed.
	//the speed distribution are to be determined, here we use uniform up to 50 m/s
	std::uniform_real_distribution<double> angle_generator(0.0, 2*PI);
	std::uniform_real_distribution<double> speed_generator(0.0, 50.0);

	//5.3.1 step 2 and 5.3.2 step 2 here
	//produce the omega_MS, v, theta_v, then calculate bulk loss
	this->omega_BS=PI/2;
	this->omega_MS=angle_generator(generator);
	this->v=speed_generator(generator);
	this->theta_v=angle_generator(generator);
	this->theta_MS=theta_MS_in;
	this->theta_BS=theta_BS_in;
	std::cout<<"theta_BS is "<<this->theta_BS<<std::endl;
	std::cout<<"theta_MS is "<<this->theta_MS<<std::endl;
	std::cout<<"v is "<<this->v<<std::endl;
	std::cout<<"theta_v is "<<this->theta_v<<std::endl;
	std::cout<<"omega_BS is "<<this->omega_BS<<std::endl;
	std::cout<<"omega_MS is "<<this->omega_MS<<std::endl;
}

base_channel::~base_channel(){std::cout<<"channel terminated"<<std::endl;}


void base_channel::operator =(const base_channel& a){
    path_base=a.path_base;
    omega_BS=a.omega_BS;
    omega_MS=a.omega_MS;
    v=a.v;
    theta_v=a.theta_v;
    theta_MS=a.theta_MS;
    theta_BS=a.theta_BS;
    d=a.d;
    sigma_AS=a.sigma_AS;
    sigma_SF=a.sigma_SF;
    sigma_DS=a.sigma_DS;
}

void base_channel::correlation_generator(std::default_random_engine& generator){}
void base_channel::set_d_n_m_AoD_from_table(path &this_path){}
void base_channel::set_d_n_m_AoA_from_table(path &this_path){}
