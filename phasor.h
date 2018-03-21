#pragma once

#include <iostream>
#include <cmath>

class phasor
{


public:

	// the angle is measured in terms of radiance

	double real;
	double imaginary;


	phasor();
	// Note that by default the phasor does not accept negative length and transform them into positive ones without any phase shift
	phasor(const double, const double);
	~phasor();

	double get_magnitude(void);


	phasor operator*(phasor);

	phasor operator*(double a);

	phasor operator+(phasor);

	phasor operator-(phasor);

	phasor operator/(phasor);

	bool operator==(phasor);




	friend std::ostream& operator<< (std::ostream&,phasor);

};

std::ostream& operator<< (std::ostream&,phasor);
