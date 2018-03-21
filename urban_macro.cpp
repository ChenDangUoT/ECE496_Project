#include "urban_macro.h"

const double PI = std::atan(1) * 4;



urban_macro::urban_macro():base_channel(){
    std::cout<<"then default called"<<std::endl;
}

urban_macro::urban_macro(const urban_macro& a):base_channel(a){
    std::cout<<"then copy called"<<std::endl;
}

urban_macro::~urban_macro(){
    //left blank
}

urban_macro::urban_macro(double distance, double theta_MS_in, double theta_BS_in)
:base_channel(distance, theta_MS_in, theta_BS_in)
{



    std::random_device seed;
	std::default_random_engine generator(seed());

    //start step 3 of 5.3.1
    this->correlation_generator(generator);

    //step 4
    std::uniform_real_distribution<double> z_n_generator(0.0, 1.0);

    //compute tao_prime_n, n=0, 1...., 5
    std::vector<double> tao_prime;
    double r_DS=1.7;
    for(int i=0; i<6; i++){
        double temp=z_n_generator(generator);
        tao_prime.push_back(0-r_DS*sigma_DS*log(temp));
    }

    //sort the vector tao_prime
    std::sort (tao_prime.begin(), tao_prime.end());

    //extract tao_n
    std::vector<double> tao;
    //assume 3GPP for now
    double  Tc=0.0000002604;
    for (int i=0; i<6; i++){
        double diff=tao_prime[i]-tao_prime[0];
        double temp =floor(diff*16/Tc+0.5);
        temp=Tc/16*temp;
        tao.push_back(temp);
    }


    //step 5
    //distribution for xi_n, could be different
    std::normal_distribution<double> xi_distribution(0.0,3.0);
    //calculate P_prime_n
    std::vector<double> P_prime_n;
    double sumP=0;
    for(int i=0; i<6; i++){
        double temp=xi_distribution(generator);
        temp=0.0-temp/10.0;
        temp=pow(10, temp);
        temp=temp*exp((1.0-r_DS)*(tao_prime[i]-tao_prime[0])/(r_DS*sigma_DS));
        sumP=sumP+temp;
        P_prime_n.push_back(temp);
    }

    //calculate P_n
    std::vector<double> P_n;
    for (int i=0; i<6; i++){
        P_n.push_back(P_prime_n[i]/sumP);

    }



    //step 6
    std::vector<double> delta_prime_AoD;
    double r_AS=1.3;
    double sigma_AoD=r_AS*sigma_AS;
    std::normal_distribution<double> delta_prime_distribution(0.0,sigma_AoD*sigma_AoD);
    for (int i=0; i<6; i++)
        delta_prime_AoD.push_back(delta_prime_distribution(generator));

    //sort by absolute value
    std::sort (delta_prime_AoD.begin(), delta_prime_AoD.end(), abssmaller);

    //step 7
    for (int i=0; i<6; i++){
        this->path_base[i].set_tao_n(tao[i]);
        this->path_base[i].set_delta_n_AoD(delta_prime_AoD[i]);
        this->path_base[i].set_P_n(P_n[i]);
    }

    //step 8, dig into each path
    std::uniform_real_distribution<double> phi_generator(0.0, 2*PI);
    for (int n=0; n<6; n++){
        double subpath_P=P_n[n]/20.0;

        //set d_n_m_AoD from table 5.2

        this->path_base[n].subpath_base.reserve(20);

        for (int m=0; m<20; m++){

                double bbb=phi_generator(generator);

                this->path_base[n].subpath_base[m].set_phi(bbb);

        }
        set_d_n_m_AoD_from_table(path_base[n]);
    }

    //step 9
    std::vector<double> delta_prime_AoA;
    for (int i=0; i<6; i++){
        double sigma_AoA=104.12*(1-exp(-0.2175*abs(10*log10(P_n[i]))));
        std::normal_distribution<double> delta_prime_distribution(0.0,sigma_AoA*sigma_AoA);
        delta_prime_AoA.push_back(delta_prime_distribution(generator));
    }

    //step 10
    for (int n=0; n<6; n++){
        std::uniform_real_distribution<double> angle_generator(0.0, 2*PI);
        //set d_n_m_AoA from table 5.2
        set_d_n_m_AoA_from_table(this->path_base[n]);
    }

    //step 11
    //firstly, associate the BS path with MS path
    for (int n=0; n<6; n++)
        this->path_base[n].set_delta_n_AoA(delta_prime_AoA[n]);

    //then randomly pair d_n_m_AoD with d_n_m_AoA for the same nth path
    for (int n=0; n<6; n++){
        std::vector<double> temp_d_n_m_AoA;
        for (int m=0; m<20; m++){
            temp_d_n_m_AoA.push_back((path_base[n].get_subpath_base())[m].get_d_AoA());
        }

        for (int m=0; m<20; m++){
            std::uniform_int_distribution<int> pair_generator(0, 19-m);
            int to_be_paired=pair_generator(generator);
            (path_base[n].get_subpath_base())[m].set_d_AoA(temp_d_n_m_AoA[to_be_paired]);
            temp_d_n_m_AoA.erase(temp_d_n_m_AoA.begin()+to_be_paired);
        }

    }


    //step 12
    for (int n=0; n<6; n++){
        for (int m=0; m<20; m++){
            (this->path_base[n].get_subpath_base())[m].set_theta_AoD(theta_BS
                                                                     +this->path_base[n].get_delta_n_AoD()
                                                                     +(this->path_base[n].get_subpath_base())[m].get_d_AoD());

            (this->path_base[n].get_subpath_base())[m].set_theta_AoA(theta_MS
                                                                     +this->path_base[n].get_delta_n_AoA()
                                                                     +(this->path_base[n].get_subpath_base())[m].get_d_AoA());

        }
    }

    //step 13: path loss from d, 34.5 + 35log10(d)



}


void urban_macro::set_d_n_m_AoA_from_table(path &this_path){

    //35 degree offset
    (this_path.subpath_base)[0].set_d_AoA(1.5649/180.0*PI);
    (this_path.subpath_base)[1].set_d_AoA(-1.5649/180.0*PI);
    (this_path.subpath_base)[2].set_d_AoA(4.9447/180.0*PI);
    (this_path.subpath_base)[3].set_d_AoA(-4.9447/180.0*PI);
    (this_path.subpath_base)[4].set_d_AoA(8.7224/180.0*PI);
    (this_path.subpath_base)[5].set_d_AoA(-8.7224/180.0*PI);
    (this_path.subpath_base)[6].set_d_AoA(13.0045/180.0*PI);
    (this_path.subpath_base)[7].set_d_AoA(-13.0045/180.0*PI);
    (this_path.subpath_base)[8].set_d_AoA(17.9492/180.0*PI);
    (this_path.subpath_base)[9].set_d_AoA(-17.9492/180.0*PI);
    (this_path.subpath_base)[10].set_d_AoA(23.7899/180.0*PI);
    (this_path.subpath_base)[11].set_d_AoA(-23.7899/180.0*PI);
    (this_path.subpath_base)[12].set_d_AoA(30.9538/180.0*PI);
    (this_path.subpath_base)[13].set_d_AoA(-30.9538/180.0*PI);
    (this_path.subpath_base)[14].set_d_AoA(40.1824/180.0*PI);
    (this_path.subpath_base)[15].set_d_AoA(-40.1824/180.0*PI);
    (this_path.subpath_base)[16].set_d_AoA(53.1816/180.0*PI);
    (this_path.subpath_base)[17].set_d_AoA(-53.1816/180.0*PI);
    (this_path.subpath_base)[18].set_d_AoA(75.4274/180.0*PI);
    (this_path.subpath_base)[19].set_d_AoA(-75.4274/180.0*PI);

}

void urban_macro::set_d_n_m_AoD_from_table(path &this_path){

    //2 degree offset, macro
    (this_path.subpath_base)[0].set_d_AoD(0.0894/180.0*PI);
    (this_path.subpath_base)[1].set_d_AoD(-0.0894/180.0*PI);
    (this_path.subpath_base)[2].set_d_AoD(0.2826/180.0*PI);
    (this_path.subpath_base)[3].set_d_AoD(-0.2826/180.0*PI);
    (this_path.subpath_base)[4].set_d_AoD(0.4987/180.0*PI);
    (this_path.subpath_base)[5].set_d_AoD(-0.4987/180.0*PI);
    (this_path.subpath_base)[6].set_d_AoD(0.7431/180.0*PI);
    (this_path.subpath_base)[7].set_d_AoD(-0.7431/180.0*PI);
    (this_path.subpath_base)[8].set_d_AoD(1.0257/180.0*PI);
    (this_path.subpath_base)[9].set_d_AoD(-1.0257/180.0*PI);
    (this_path.subpath_base)[10].set_d_AoD(1.3594/180.0*PI);
    (this_path.subpath_base)[11].set_d_AoD(-1.3594/180.0*PI);
    (this_path.subpath_base)[12].set_d_AoD(1.7688/180.0*PI);
    (this_path.subpath_base)[13].set_d_AoD(-1.7688/180.0*PI);
    (this_path.subpath_base)[14].set_d_AoD(2.2961/180.0*PI);
    (this_path.subpath_base)[15].set_d_AoD(-2.2961/180.0*PI);
    (this_path.subpath_base)[16].set_d_AoD(3.0389/180.0*PI);
    (this_path.subpath_base)[17].set_d_AoD(-3.0389/180.0*PI);
    (this_path.subpath_base)[18].set_d_AoD(4.3101/180.0*PI);
    (this_path.subpath_base)[19].set_d_AoD(-4.3101/180.0*PI);

}


//this is the operation for part 5.6
void urban_macro::correlation_generator(std::default_random_engine& generator){

    std::normal_distribution<double> x_distribution(0.0,1.0);
    double epsilon_AS, mu_AS, epsilon_DS=0.18, mu_DS=-6.18;
    double mean_AS;

    std::cout<<"Want mean AS at BS be 8 or 15?"<<std::endl;
    std::cin>>mean_AS;

    if (mean_AS==8){
        mu_AS=0.810;
        epsilon_AS=0.34;
    }
    else if (mean_AS==15){
        mu_AS=1.18;
        epsilon_AS=0.210;
    }
    else {
        std::cout<<"Invalid input, simulation terminated. "<<std::endl;
        return;
    }

    //sigma_AS in rad
    double temp=x_distribution(generator);
    temp=temp*epsilon_AS+mu_AS;
    sigma_AS=pow(10,temp);

    //sigma_DS in second
    temp=x_distribution(generator);
    temp=temp*epsilon_DS+mu_DS;
    sigma_DS=pow(10,temp);

    //sigma_SF in dB
    sigma_SF=8.0;
}
