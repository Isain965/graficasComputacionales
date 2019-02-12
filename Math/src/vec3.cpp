#include "vec3.h"

cgmath::vec3::vec3(): x(0.0f), y(0.0f), z(0.0f) {

}

cgmath::vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float cgmath::vec3::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

void cgmath::vec3::normalize() {
	float m = magnitude();
	x /= m;
	y /= m;
	z /= m;
}

float cgmath::vec3::magnitude(const vec3& v) {
	return v.magnitude();
}

cgmath::vec3 cgmath::vec3::normalize(const vec3& v) {
	return v / v.magnitude();
}

float cgmath::vec3::dot(const vec3& a, const vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

cgmath::vec3 cgmath::vec3::cross(const vec3& a, const vec3& b) {
	return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float& cgmath::vec3::operator[](int i) {
	return (&x)[i];
}

const float& cgmath::vec3::operator[](int i) const {
	return (&x)[i];
}

cgmath::vec3& cgmath::vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

cgmath::vec3& cgmath::vec3::operator/=(float s) {
	x = x/s;
	y = y/s;
	z = z/s;
	return *this;
}

cgmath::vec3& cgmath::vec3::operator+=(const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

cgmath::vec3& cgmath::vec3::operator-=(const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

bool cgmath::vec3::operator==(const vec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}