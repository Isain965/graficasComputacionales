#include "mat3.h"

cgmath::mat3::mat3() {
	n[0][0] = 0.0f;	n[1][0] = 0.0f; n[2][0] = 0.0f;
	n[0][1] = 0.0f; n[1][1] = 0.0f; n[2][1] = 0.0f;
	n[0][2] = 0.0f; n[1][2] = 0.0f; n[2][2] = 0.0f;
}

cgmath::mat3::mat3(float diagonal) {
	n[0][0] = diagonal;	n[1][0] = 0.0f;		n[2][0] = 0.0f;
	n[0][1] = 0.0f;		n[1][1] = diagonal;	n[2][1] = 0.0f;
	n[0][2] = 0.0f;		n[1][2] = 0.0f;		n[2][2] = diagonal;
}

cgmath::mat3::mat3(const vec3& a, const vec3& b, const vec3& c) {
	n[0][0] = a.x; n[1][0] = b.x; n[2][0] = c.x;
	n[0][1] = a.y; n[1][1] = b.y; n[2][1] = c.y;
	n[0][2] = a.z; n[1][2] = b.z; n[2][2] = c.z;
}

cgmath::vec3& cgmath::mat3::operator[](int column) {
	return *reinterpret_cast<vec3*>(n[column]);
}

const cgmath::vec3& cgmath::mat3::operator[](int column) const {
	return *reinterpret_cast<const vec3*>(n[column]);
}

bool cgmath::mat3::operator==(const mat3& m) const {
	return n[0][0] == m[0][0] && n[0][1] == m[0][1] && n[0][2] == m[0][2] &&
		n[1][0] == m[1][0] && n[1][1] == m[1][1] && n[1][2] == m[1][2] &&
		n[2][0] == m[2][0] && n[2][1] == m[2][1] && n[2][2] == m[2][2];
}

float cgmath::mat3::determinant(const mat3& m) {
	return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

cgmath::mat3 cgmath::mat3::inverse(const mat3& m) {
	const vec3& a = m[0];
	const vec3& b = m[1];
	const vec3& c = m[2];

	vec3 r0 = vec3::cross(b, c);
	vec3 r1 = vec3::cross(c, a);
	vec3 r2 = vec3::cross(a, b);

	float invDeter = 1.0f / vec3::dot(a, r0);

	r0 *= invDeter;
	r1 *= invDeter;
	r2 *= invDeter;

	mat3 inv;
	inv[0][0] = r0.x; inv[1][0] = r0.y; inv[2][0] = r0.z;
	inv[0][1] = r1.x; inv[1][1] = r1.y; inv[2][1] = r1.z;
	inv[0][2] = r2.x; inv[1][2] = r2.y; inv[2][2] = r2.z;

	return inv;
}

cgmath::mat3 cgmath::mat3::transpose(const mat3& m) {
	const vec3& a = m[0];
	const vec3& b = m[1];
	const vec3& c = m[2];

	return mat3(vec3(a.x, b.x, c.x), vec3(a.y, b.y, c.y), vec3(a.z, b.z, c.z));
}
