#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include "channel.h"
#include <bitset>


// our final destination is to input vector<bool> and output vector<pair<float,float>> which translates the bits to a priori probability

using std::vector;
using std::pair;



// our ultimate goal is to create a function with following prototype
// vector<pair,pair> channel(vector<bool>) or bitset for more robust/standarized behavior 
// the input argument may increase as needed but do be careful when try to increase the argument
// the main will serve as a test shell and generate input for the channel, as well as output them to matlab for plotting.




