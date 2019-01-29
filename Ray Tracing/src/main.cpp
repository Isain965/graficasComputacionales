#include <vector>

#include "image_writer.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"


float MAXFLOAT = 10.0;

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		float random1 = ((double)rand() / (RAND_MAX));
		float random2 = ((double)rand() / (RAND_MAX));
		float random3 = ((double)rand() / (RAND_MAX));
		p = 2.0*vec3(random1, random2, random3) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color( ray(rec.p, target-rec.p), world);
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
	int ns = 5;

	//Crar vector
	std::vector<unsigned char> pixels;

	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;

	
	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);

			for (int s = 0; s < ns; s++) {
				float random = ((double)rand() / (RAND_MAX));
				float u = float(i + random) / float(nx);
				random = ((double)rand() / (RAND_MAX));
				float v = float(j + random) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

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