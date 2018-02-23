#include "main.h"

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
	if(use_bit_set)
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
	}

	// note that bitset requires a pre-determined(at compile time) size at its template which is not very convinient
	// whereas vector<bool> is not a very orthodox approach due to its handling.






<<<<<<< HEAD
	std::cout << "This is a test file" << std::endl;
	std::cout<<"test"<<std::endl;
	std::cout<<"test on desktop"<<std::endl;
=======
>>>>>>> 3b23fbb460c9c41162f7f5f32f08b8f01542e3c8


	return 0;
}
