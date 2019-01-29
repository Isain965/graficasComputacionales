#include <vector>

#include "image_writer.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"


float MAXFLOAT = 2.0;

vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {

	int nx = 200;
	int ny = 100;

	//Crar vector
	std::vector<unsigned char> pixels;

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	//ho
	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	//ho
	
	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			//Crear degradado
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);

			vec3 p = r.point_at_parameter(2.0);
			vec3 col = color(r, world);

			//Imprimir todo rojo en la pantalla
			pixels.push_back(int(255.99 * col[0]));
			pixels.push_back(int(255.99 * col[1]));
			pixels.push_back(int(255.99 * col[2]));
		}
	}

	//Guardar imagen (El numero 3 es para indicar que son 3 canales de color), pixels.data es para regresar un apuntador
	image_writer::save_png("out.png", nx, ny, 3, pixels.data());


	return 0;
}