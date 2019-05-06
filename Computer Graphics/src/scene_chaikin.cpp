#include "scene_chaikin.h"

#include "vec2.h"

#include <vector>

static void ditto_points_body(std::vector<cgmath::vec2> &positions) {
	positions.push_back(cgmath::vec2(-0.52f, 0.50f));
	positions.push_back(cgmath::vec2(-1.0f, 0.25f));
	positions.push_back(cgmath::vec2(-0.77f, 0.0f));
	positions.push_back(cgmath::vec2(-0.62f, -0.12f));
	positions.push_back(cgmath::vec2(-0.78f, -0.25f));
	positions.push_back(cgmath::vec2(-0.9f, -0.41f));
	positions.push_back(cgmath::vec2(-0.923f, -0.7f));
	positions.push_back(cgmath::vec2(-0.792f, -0.868f));
	positions.push_back(cgmath::vec2(-0.55f, -0.79f));
	positions.push_back(cgmath::vec2(-0.36f, -0.69f));
	positions.push_back(cgmath::vec2(-0.13f, -0.79f));
	positions.push_back(cgmath::vec2(0.13f, -0.82f));
	positions.push_back(cgmath::vec2(0.25f, -0.74f));
	positions.push_back(cgmath::vec2(0.35f, -0.63f));
	positions.push_back(cgmath::vec2(0.59f, -0.76f));
	positions.push_back(cgmath::vec2(0.98f, -0.77f));
	positions.push_back(cgmath::vec2(0.91f, -0.46f));
	positions.push_back(cgmath::vec2(0.692f, -0.103f));
	positions.push_back(cgmath::vec2(0.907f, 0.098f));
	positions.push_back(cgmath::vec2(1.0f, 0.29f));
	positions.push_back(cgmath::vec2(0.842f, 0.414f));
	positions.push_back(cgmath::vec2(0.62f, 0.49f));
	positions.push_back(cgmath::vec2(0.54f, 0.72f));
	positions.push_back(cgmath::vec2(0.42f, 0.89f));
	positions.push_back(cgmath::vec2(0.22f, 0.82f));
	positions.push_back(cgmath::vec2(0.05f, 0.728f));
	positions.push_back(cgmath::vec2(-0.07f, 0.81f));
	positions.push_back(cgmath::vec2(-0.24f, 0.87f));
	positions.push_back(cgmath::vec2(-0.38f, 0.73f));
	positions.push_back(cgmath::vec2(-0.45f, 0.627f));
	positions.push_back(cgmath::vec2(-0.52f, 0.50f));
}

static void ditto_points_boca(std::vector<cgmath::vec2> &positions) { 
	positions.push_back(cgmath::vec2(-0.1f, 0.385f));
	positions.push_back(cgmath::vec2(0.09f, 0.378f));
	positions.push_back(cgmath::vec2(0.23f, 0.394f));
	positions.push_back(cgmath::vec2(0.42f, 0.438f));
}

static void ditto_points_eye_1(std::vector<cgmath::vec2> &positions) {
	positions.push_back(cgmath::vec2(0.015f, 0.59f));
	positions.push_back(cgmath::vec2(0.015f, 0.5f));
	positions.push_back(cgmath::vec2(0.045f, 0.5f));
	positions.push_back(cgmath::vec2(0.045f, 0.59f));
}

static void ditto_points_eye_2(std::vector<cgmath::vec2> &positions) {
	positions.push_back(cgmath::vec2(0.255f, 0.61f));
	positions.push_back(cgmath::vec2(0.255f, 0.52f));
	positions.push_back(cgmath::vec2(0.285f, 0.52f));
	positions.push_back(cgmath::vec2(0.285f, 0.61f));
}


std::vector<cgmath::vec2> calculate_chaikin(int refinamientos, std::vector<cgmath::vec2> positions, int boca) {
	std::vector<cgmath::vec2> newPos = positions;
	// Para los refinamientos
	for (int i = 0; i < refinamientos; i++) {

		std::vector<cgmath::vec2> newPoints;

		// Para recorrer el arreglo
		for (int x = 0; x < newPos.size(); x++) {
			// formula
			//Qi = (3.0 / 4.0 * Pi) + (1.0 / 4.0 * Pi+1)
			//Ri = (1.0 / 4.0 * Pi+1) + (3.0 / 4.0 * Pi)

			int next = x + 1;

			if ((x + 1) == newPos.size()) {
				next = 0;

				if (boca == 2) {
					next = x;
				}
			}
			

			float Qx = (3.0 / 4.0 * newPos[x].x) + (1.0 / 4.0 * newPos[next].x);
			float Qy = (3.0 / 4.0 * newPos[x].y) + (1.0 / 4.0 * newPos[next].y);

			float Rx = (3.0 / 4.0 * newPos[next].x) + (1.0 / 4.0 * newPos[x].x);
			float Ry = (3.0 / 4.0 * newPos[next].y) + (1.0 / 4.0 * newPos[x].y);

			cgmath::vec2 Q = cgmath::vec2(Qx, Qy);
			cgmath::vec2 R = cgmath::vec2(Rx, Ry);

			newPoints.push_back(Q);
			newPoints.push_back(R);
		}

		newPos = newPoints;
		newPoints.clear();
	}
	
	return newPos;

}

// Se manda a llamar una vez
// cuando inicia la aplicacion
void scene_chaikin::init() {

	/***********Para el cuerpo ***************/
	std::vector<cgmath::vec2> positions;

	ditto_points_body(positions);
	std::vector<cgmath::vec2> positions_r = positions;

	std::vector<cgmath::vec2> positionsChaikin = calculate_chaikin(10, positions, 1);

	sizeDitto = positionsChaikin.size();
	sizeDitto_r = positions_r.size();

	std::cout << sizeDitto << " --> sizeDitto\n";
	std::cout << sizeDitto_r << " --> sizeDitto sin chaikin\n";


	/***********Para la boca ****************/
	std::vector<cgmath::vec2> positions_boca;

	ditto_points_boca(positions_boca);

	std::vector<cgmath::vec2> positionsChaikin_boca = calculate_chaikin(10, positions_boca, 2);

	sizeBoca = positionsChaikin_boca.size();
	sizeBoca_r = positions_boca.size();

	std::cout << sizeBoca << " --> sizeBoca\n";
	std::cout << sizeBoca_r << " --> sizeBoca sin chaikin";


	/***********Para el ojo 1 ****************/
	std::vector<cgmath::vec2> positions_eye_1;

	ditto_points_eye_1(positions_eye_1);

	std::vector<cgmath::vec2> positionsChaikin_eye_1 = calculate_chaikin(10, positions_eye_1, 3);

	sizeEye1 = positionsChaikin_eye_1.size();
	sizeEye1_r = positions_eye_1.size();

	std::cout << sizeEye1 << " --> sizeOjo1\n";
	std::cout << sizeEye1_r << " --> sizeOjo1 sin chaikin\n";

	/***********Para el ojo 2 ****************/
	std::vector<cgmath::vec2> positions_eye_2;

	ditto_points_eye_2(positions_eye_2);

	std::vector<cgmath::vec2> positionsChaikin_eye_2 = calculate_chaikin(10, positions_eye_2, 3);

	sizeEye2 = positionsChaikin_eye_2.size();
	sizeEye2_r = positions_eye_2.size();

	std::cout << sizeEye2 << " --> sizeOjo2\n";
	std::cout << sizeEye2_r << " --> sizeOjo2 sin chaikin\n";



	/***********Para el cuerpo ***************/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positionsChaikin.size(), positionsChaikin.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_r);
	glBindVertexArray(vao_r);
	glGenBuffers(1, &positionsVBO_r);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_r);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positions_r.size(), positions_r.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	/***********Para la boca ****************/
	glGenVertexArrays(1, &vao_boca);
	glBindVertexArray(vao_boca);
	glGenBuffers(1, &positionsVBO_boca);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_boca);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positionsChaikin_boca.size(), positionsChaikin_boca.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_boca_r);
	glBindVertexArray(vao_boca_r);
	glGenBuffers(1, &positionsVBO_boca_r);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_boca_r);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positions_boca.size(), positions_boca.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/***********Para el ojo 1 ****************/
	glGenVertexArrays(1, &vao_eye1);
	glBindVertexArray(vao_eye1);
	glGenBuffers(1, &positionsVBO_eye1);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_eye1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positionsChaikin_eye_1.size(), positionsChaikin_eye_1.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_eye1_r);
	glBindVertexArray(vao_eye1_r);
	glGenBuffers(1, &positionsVBO_eye1_r);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_eye1_r);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positions_eye_1.size(), positions_eye_1.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/***********Para el ojo 2 ****************/
	glGenVertexArrays(1, &vao_eye2);
	glBindVertexArray(vao_eye2);
	glGenBuffers(1, &positionsVBO_eye2);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_eye2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positionsChaikin_eye_2.size(), positionsChaikin_eye_2.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vao_eye2_r);
	glBindVertexArray(vao_eye2_r);
	glGenBuffers(1, &positionsVBO_eye2_r);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO_eye2_r);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * positions_eye_2.size(), positions_eye_2.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	primitiveType = GL_LINE_STRIP;
	// primitiveType_r = GL_LINES;
}

void scene_chaikin::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPointSize(20.0f);
}

void scene_chaikin::sleep() {
	glPointSize(0.3f);
}

void scene_chaikin::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/***************Para el cuerpo****************/
	glBindVertexArray(vao);
	glDrawArrays(primitiveType, 0, sizeDitto);
	glBindVertexArray(0);

	glBindVertexArray(vao_r);
	glDrawArrays(primitiveType_r, 0, sizeDitto_r);
	glBindVertexArray(0);


	/***************Para la boca****************/
	glBindVertexArray(vao_boca);
	glDrawArrays(primitiveType, 0, sizeBoca);
	glBindVertexArray(0);

	glBindVertexArray(vao_boca_r);
	glDrawArrays(primitiveType_r, 0, sizeBoca_r);
	glBindVertexArray(0);


	/***************Para el ojo 1****************/
	glBindVertexArray(vao_eye1);
	glDrawArrays(primitiveType, 0, sizeEye1);
	glBindVertexArray(0);

	glBindVertexArray(vao_eye1_r);
	glDrawArrays(primitiveType_r, 0, sizeEye1_r);
	glBindVertexArray(0);

	/***************Para el ojo 2****************/
	glBindVertexArray(vao_eye2);
	glDrawArrays(primitiveType, 0, sizeEye2);
	glBindVertexArray(0);

	glBindVertexArray(vao_eye2_r);
	glDrawArrays(primitiveType_r, 0, sizeEye2_r);
	glBindVertexArray(0);
}

void scene_chaikin::normalKeysDown(unsigned char key) {
	// Para mostrar solo la figura bien
	if (key == '1') {
		primitiveType = GL_LINE_STRIP;
		primitiveType_r = GL_LINE_STIPPLE;
	}

	// Para resaltar las lineas originales que despues fueron deformadas
	if (key == '2') {
		primitiveType = GL_LINES;
		primitiveType_r = GL_LINE_STRIP;
	}

	// Para mostrar sobre puestas las lineas reales sobre las deformadas
	if (key == '3') {
		primitiveType = GL_LINE_STRIP;
		primitiveType_r = GL_LINE_STRIP;
	}
}

void scene_chaikin::normalKeysUp(unsigned char key) {

}



void scene_chaikin::resize(int width, int height) {

}
