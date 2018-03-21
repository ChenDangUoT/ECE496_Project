#include "urban_micro.h"

const double PI = std::atan(1) * 4;



urban_micro::urban_micro():base_channel(){
    //left blank
}
urban_micro::urban_micro(const urban_micro& a):base_channel(a){
}
urban_micro::~urban_micro(){
    //left blank
}

urban_micro::urban_micro(double distance, double theta_MS_in, double theta_BS_in)
:base_channel(distance, theta_MS_in, theta_BS_in)
{

    std::random_device seed;
	std::default_random_engine generator(seed());

    //start step 3 of 5.3.1
    this->correlation_generator(generator);

    //step 4
    std::uniform_real_distribution<double> tao_prime_generator(0.0, 0.0000012);

    //compute tao_prime_n, n=0, 1...., 5
    std::vector<double> tao_prime;

    for(int i=0; i<6; i++){
        double temp=tao_prime_generator(generator);
        tao_prime.push_back(temp);
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


    //step 6
    //distribution for z_n
    std::normal_distribution<double> z_n_distribution(0.0,3.0);
    //calculate P_prime_n
    std::vector<double> P_prime_n;
    double sumP=0;
    for(int i=0; i<6; i++){
        double temp=z_n_distribution(generator);
        temp=tao_prime[i]-tao_prime[0]+temp/10.0;
        temp=pow(10, -temp);
        sumP=sumP+temp;
        P_prime_n.push_back(temp);
    }

    //calculate P_n
    std::vector<double> P_n;
    for (int i=0; i<6; i++)
        P_n.push_back(P_prime_n[i]/sumP);


    //step 7
    std::vector<double> delta_AoD;
    std::uniform_real_distribution<double> delta_distribution(-40.0/180.0*PI,40.0/180.0*PI);
    for (int i=0; i<6; i++)
        delta_AoD.push_back(delta_distribution(generator));



    //step 8
    for (int i=0; i<6; i++){
        this->path_base[i].set_tao_n(tao[i]);
        this->path_base[i].set_delta_n_AoD(delta_AoD[i]);
        this->path_base[i].set_P_n(P_n[i]);
    }

    //step 9, dig into each path
    std::uniform_real_distribution<double> phi_generator(0.0, 2*PI);
    for (int n=0; n<6; n++){
        double subpath_P=P_n[n]/20.0;
        this->path_base[n].subpath_base.reserve(20);

        for (int m=0; m<20; m++){
                double bbb=phi_generator(generator);
                this->path_base[n].subpath_base[m].set_phi(bbb);
        }

        //set d_n_m_AoD from table 5.2
        set_d_n_m_AoD_from_table(this->path_base[n]);
    }

    //step 10
    std::vector<double> delta_prime_AoA;
    for (int i=0; i<6; i++){
        double sigma_AoA=104.12*(1-exp(-0.265*abs(10*log10(P_n[i]))));
        std::normal_distribution<double> delta_prime_distribution(0.0,sigma_AoA*sigma_AoA);
        delta_prime_AoA.push_back(delta_prime_distribution(generator));
    }

    //step 11
    for (int n=0; n<6; n++){
        std::uniform_real_distribution<double> angle_generator(0.0, 2*PI);
        //set d_n_m_AoA from table 5.2
        set_d_n_m_AoA_from_table(this->path_base[n]);
    }

    //step 12
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


    //step 13 to get theta_AoD and theta_AoA
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

    //step 14: path loss from d, 34.53 + 38log10(d)



}


void urban_micro::set_d_n_m_AoA_from_table(path &this_path){

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

void urban_micro::set_d_n_m_AoD_from_table(path &this_path){
    std::vector<subpath> this_subpath_base=this_path.get_subpath_base();
    //5 degree offset, micro
    (this_path.subpath_base)[0].set_d_AoD(0.2236/180.0*PI);
    (this_path.subpath_base)[1].set_d_AoD(-0.2236/180.0*PI);
    (this_path.subpath_base)[2].set_d_AoD(0.7064/180.0*PI);
    (this_path.subpath_base)[3].set_d_AoD(-0.7064/180.0*PI);
    (this_path.subpath_base)[4].set_d_AoD(1.2461/180.0*PI);
    (this_path.subpath_base)[5].set_d_AoD(-1.2461/180.0*PI);
    (this_path.subpath_base)[6].set_d_AoD(1.8578/180.0*PI);
    (this_path.subpath_base)[7].set_d_AoD(-1.8578/180.0*PI);
    (this_path.subpath_base)[8].set_d_AoD(2.5642/180.0*PI);
    (this_path.subpath_base)[9].set_d_AoD(-2.5642/180.0*PI);
    (this_path.subpath_base)[10].set_d_AoD(3.3986/180.0*PI);
    (this_path.subpath_base)[11].set_d_AoD(-3.3986/180.0*PI);
    (this_path.subpath_base)[12].set_d_AoD(4.4220/180.0*PI);
    (this_path.subpath_base)[13].set_d_AoD(-4.4220/180.0*PI);
    (this_path.subpath_base)[14].set_d_AoD(5.7403/180.0*PI);
    (this_path.subpath_base)[15].set_d_AoD(-5.7403/180.0*PI);
    (this_path.subpath_base)[16].set_d_AoD(7.5974/180.0*PI);
    (this_path.subpath_base)[17].set_d_AoD(-7.5974/180.0*PI);
    (this_path.subpath_base)[18].set_d_AoD(10.7753/180.0*PI);
    (this_path.subpath_base)[19].set_d_AoD(-10.7753/180.0*PI);

}


//this is the operation for part 5.6
void urban_micro::correlation_generator(std::default_random_engine& generator){
    //sigma_AS in rad
    sigma_AS=0.0;

    //sigma_DS in second
    sigma_DS=0;

    //sigma_SF in dB
    sigma_SF=10.0;
}
