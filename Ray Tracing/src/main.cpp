#include <vector>

#include "image_writer.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"


float MAXFLOAT = 1000.0;

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		} else {
			return vec3(0,0,0);
		}
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

	hitable *list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 1.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0.3));
	hitable *world = new hitable_list(list, 4);
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
				col += color(r, world, 0);
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