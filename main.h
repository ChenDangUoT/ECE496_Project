#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <complex>
#include "channel.h"
#include "phasor.h"
#include <bitset>

#define S 2//BS array element
#define U 2//MS array element
#define N 6//multi-path component
#define M 20
#define f0 2000000000//carrier frequency
#define D 3000//LOS distance
#define Tm 0.1//max time
#define STEPS 100//total steps for the time interval


// our final destination is to input vector<bool> and output vector<pair<float,float>> which translates the bits to a priori probability

using std::vector;
using std::pair;

void channel_coefficients_generator(base_channel* current_channel, double t, phasor (&H)[S][U][N], double k);
double G_BS(double theta_AoD);//BS pattern
double G_MS(double theta_AoA);//MS pattern
phasor generate_phasor(double magnitude, double angle);




// our ultimate goal is to create a function with following prototype
// vector<pair,pair> channel(vector<bool>) or bitset for more robust/standarized behavior
// the input argument may increase as needed but do be careful when try to increase the argument
// the main will serve as a test shell and generate input for the channel, as well as output them to matlab for plotting.




