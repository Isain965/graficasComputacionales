#pragma once
#include <iostream>

namespace cgmath {
	
	class complex {
		public:
			// Estas dos variables se almacenan en memoria de manera contigua.
			// Solo las variables se almacena, los metodos NO.
			float real;
			float imag;

			complex();

			complex(float real, float image);

			// Manera tradicional de imprimir una representacion
			// de esta clase. Muy comun en otros lenguajes
			void print() const;

			//Manera de hacerlo en c++, utilizando sobrecarga de operadores
			friend inline std::ostream& operator<< (std::ostream& os, const complex& c) {
				os << c.real << " + " << c.imag << "i";
				return os;
			}

			void add(const complex& other);
			complex& operator+=(const complex& other);

			static complex add(const complex& a, const complex& b);
	};

	inline complex operator+(const complex& a, const complex& b) {
		return complex(a.real + b.real, a.imag + b.imag);
	}
}