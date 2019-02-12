#include "complex.h"
#include <iostream>

cgmath::complex::complex(): real(0.0f), imag(0.0f) {

}

cgmath::complex::complex(float real, float imag) {
	this->real = real;
	this->imag = imag;
}

void cgmath::complex::print() const {
	std::cout << real << "+" << imag << "i" << std::endl;
}

void cgmath::complex::add(const complex & other) {
	real += other.real;
	imag += other.imag;
}

cgmath::complex& cgmath::complex::operator+=(const complex & other) {
	real += other.real;
	imag += other.imag;

	return *this;
}

cgmath::complex cgmath::complex::add(const cgmath::complex& a, const cgmath::complex& b) {
	return cgmath::complex(a.real + b.real, a.imag + b.imag);
}