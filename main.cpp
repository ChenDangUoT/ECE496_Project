#include "main.h"

const double PI = std::atan(1) * 4;






//assume half wavelength array, c=3*10^8
int main(void) {
	// to begin with we generate random bits

	// seeding the random engine
	// note that this may cause an exception in those that do not support random device
	std::random_device seed;
	std::default_random_engine generator(seed());
	std::uniform_int_distribution<unsigned long long> bits_generator(0,ULLONG_MAX);

	// usage each time we need a value, we write down bits_generator(seed)
	// also note that due to the expensive construction and high entropy it is advised to stick to one generator via global variable or passing them reference
	//


	bool use_bit_set = true;
	// below is an example of generating 64 bits which is not very much for communication channel
	if(!use_bit_set)
	std::bitset<64> bits(bits_generator(generator));
	//do the channel here

	else {
		// unfortunately using vector<bool> requires a more complicated approach
		// masking
		vector<bool> bits;
		unsigned long long number = bits_generator(generator);
		for (unsigned int i = 0; i < 64; i++) {
			bool bit = number ^ (1 << i);
			bits.push_back(bit);
		}

		//do the channel here
		double lambda=300000000.0/f0;
		double k=2*PI/lambda;
		int input;

		base_channel* current_channel;
        std::cout<<"1 for urban macro, 2 for suburban macro, 3 for urban micro"<<std::endl;
        std::cin>>input;
        std::uniform_real_distribution<double> angle_generator(-0.125*PI, 0.125*PI);//theta_MS and theta_BS
        double theta_MS_in=angle_generator(generator);
        double theta_BS_in=angle_generator(generator);
        if (input==1)
            current_channel=new urban_macro(D, theta_MS_in, theta_BS_in);
            else if(input==2)
                current_channel=new suburban_macro(D, theta_MS_in, theta_BS_in);
            else if(input==3)
                current_channel=new urban_micro(D, theta_MS_in, theta_BS_in);
            else{

                return 0;
            }

        phasor H[U][S][N];
        double t=0;


        for(int tic=0; tic<STEPS; tic++){
            t=tic*(Tm/STEPS);

            channel_coefficients_generator(current_channel, t, H, k);



            //then do the operations


        }




	}

	// note that bitset requires a pre-determined(at compile time) size at its template which is not very convinient
	// whereas vector<bool> is not a very orthodox approach due to its handling.









	return 0;
}

//add sqrt to the gain functions in the future for 5.4-1
double G_BS(double theta_AoD){
    return 1;
}

double G_MS(double theta_AoA){
    return 1;
}

phasor generate_phasor(double magnitude, double angle){
    phasor result;
	result.real = magnitude*std::cos(angle);
	result.imaginary = magnitude*std::sin(angle);
	return result;
}


void channel_coefficients_generator(base_channel* current_channel, double t, phasor (&H)[S][U][N], double k){

    double ds, du;

    for (int s=0; s<S; s++){
        ds=s*PI/k;
        for (int u=0; u<U; u++){
            du=u*PI/k;
            for (int n=0; n<N; n++){
                phasor usnt(0, 0);

                for (int m=0; m<M; m++){
                    subpath temp=(current_channel->path_base[n].get_subpath_base())[m];
                    phasor first_part=generate_phasor(1, k*ds*sin(temp.get_theta_AoD())+temp.get_phi());

                    phasor second_part=generate_phasor(1, k*du*sin(temp.get_theta_AoA()));
                    phasor third_part=generate_phasor(1, k*current_channel->v*
                                                      cos(temp.get_theta_AoA()-current_channel->theta_v)*t);
                    usnt=usnt+(first_part*second_part*third_part);

                }

                std::cout<<(current_channel->path_base[n].get_P_n())<<std::endl;
                std::cout<<(current_channel->sigma_SF)<<std::endl;
                std::cout<<M<<std::endl;


                usnt=usnt*sqrt(current_channel->path_base[n].get_P_n()*
                               current_channel->sigma_SF/M);

                H[s][u][n]=usnt;
                std::cout<<usnt<<std::endl;
            }
        }
    }

}
