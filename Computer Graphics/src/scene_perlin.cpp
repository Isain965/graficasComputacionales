#include "scene_perlin.h"
#include <vector>
#include <random>
#include <iostream>
#include "time.h"
#include "vec2.h"
#include "vec3.h"

// Variables del Terreno
const int cols = 100;
const int rows = 100;


void scene_perlin::init() {
	std::vector<cgmath::vec3> positions;

	// Crean los vertices para el grid
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			positions.push_back(cgmath::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void scene_perlin::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_perlin::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_perlin::mainLoop() {

}

void scene_perlin::resize(int width, int height) {

}
