#include "phasor.h"

const double PI = std::atan(1) * 4;

phasor::phasor()
{
	//default constructor for phasors
	// in default we create a phasor that has 0 magnitude(length) and 0 phase angle

	this->real = 0;
	this->imaginary = 0;
}

phasor::phasor(const double real_, const double imaginary_) {

	this->real = real_;
	this->imaginary = imaginary_;





}


phasor::~phasor()
{
}


phasor phasor::operator*(double a){
    phasor result;

	result.real = this->real*a;

	result.imaginary = this->imaginary*a;


	return result;
}

phasor phasor::operator*(phasor input) {

	// multiplication open up and add

	phasor result;

	result.real = this->real*input.real-this->imaginary*input.imaginary;

	result.imaginary = this->imaginary*input.real + input.imaginary*this->real;


	return result;


}

phasor phasor::operator/(phasor input) {

	//division, angle difference and length mulitplication

	// handling the zero case
	phasor result;

	{
		std::cerr << "Dividing by a zero, aborting division" << std::endl;
		return *this;
	}

	// if the denominator is 0 then return 0
	if (this->real == 0 && this->imaginary == 0)
		return result;

	// in this case we do a multiplication with its conjugate and divide the result by its magintude

	phasor conjugate(input.real, input.imaginary*-1);

	result = *this*conjugate;

	result.real = result.real / conjugate.get_magnitude();
	result.imaginary = result.imaginary / conjugate.get_magnitude();

	return result;



}


phasor phasor::operator+(phasor input) {

	// phasor addtion, algorithm here is to convert them into real/imaginary part and do the addtion and then move back

	phasor result;

	result.real = this->real + input.real;
	result.imaginary = this->imaginary + input.imaginary;

	return result;



}

phasor phasor::operator-(phasor input) {

	// phasor diduction, implemented by shifting the input with PI and addition

	phasor result;

	result.real = this->real - input.real;
	result.imaginary = this->imaginary - input.imaginary;

	return result;




}


bool phasor::operator==(phasor input) {

	bool is_equal = false;

	is_equal = this->real == input.real && this->imaginary == input.imaginary;

	return is_equal;



}


std::ostream& operator<< (std::ostream& output_stream , phasor object) {
// output stream overload

	if(object.imaginary>0)
	    output_stream <<object.real << "+" << object.imaginary<<"i";
	else
		output_stream << object.real << object.imaginary << "i";


	return output_stream;


}


double phasor::get_magnitude(void) {

	if (this->real == 0 && this->imaginary == 0) {

		return 0;
	}

	return std::sqrt(this->real*this->real + this->imaginary*this->imaginary);



}
