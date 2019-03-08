#include "scene_chaikin.h"

#include "vec2.h"

#include <vector>

static void ditto_points(std::vector<cgmath::vec2> &positions) {
	positions.push_back(cgmath::vec2(-0.52f, 0.50f));
	positions.push_back(cgmath::vec2(-1.0f, 0.25f));
	positions.push_back(cgmath::vec2(-0.77f, 0.0f));
	positions.push_back(cgmath::vec2(-0.62f, -0.12f));
	positions.push_back(cgmath::vec2(-0.78f, -0.25f));
	positions.push_back(cgmath::vec2(-0.9f, -0.41f));
	positions.push_back(cgmath::vec2(-0.85f, -0.64f));
	positions.push_back(cgmath::vec2(-0.81f, -0.85f));
	positions.push_back(cgmath::vec2(-0.55f, -0.79f));
	positions.push_back(cgmath::vec2(-0.36f, -0.69f));
	positions.push_back(cgmath::vec2(-0.13f, -0.79f));
	positions.push_back(cgmath::vec2(0.13f, -0.82f));
	positions.push_back(cgmath::vec2(0.25f, -0.74f));
	positions.push_back(cgmath::vec2(0.35f, -0.63f));
	positions.push_back(cgmath::vec2(0.59f, -0.76f));
	positions.push_back(cgmath::vec2(0.98f, -0.77f));
	positions.push_back(cgmath::vec2(0.91f, -0.46f));
	positions.push_back(cgmath::vec2(0.65f, -0.10f));
	positions.push_back(cgmath::vec2(0.90f, 0.15f));
	positions.push_back(cgmath::vec2(1.0f, 0.29f));
	positions.push_back(cgmath::vec2(0.84f, 0.39f));
	positions.push_back(cgmath::vec2(0.62f, 0.49f));
	positions.push_back(cgmath::vec2(0.54f, 0.72f));
	positions.push_back(cgmath::vec2(0.42f, 0.89f));
	positions.push_back(cgmath::vec2(0.22f, 0.82f));
	positions.push_back(cgmath::vec2(0.02f, 0.72f));
	positions.push_back(cgmath::vec2(-0.07f, 0.81f));
	positions.push_back(cgmath::vec2(-0.24f, 0.87f));
	positions.push_back(cgmath::vec2(-0.38f, 0.73f));
	positions.push_back(cgmath::vec2(-0.49f, 0.62f));
	positions.push_back(cgmath::vec2(-0.52f, 0.50f));
}

std::vector<cgmath::vec2> calculate_chaikin(std::vector<cgmath::vec2> positions) {
	std::vector<cgmath::vec2> newPos = positions;
	// Para los refinamientos
	for (int i = 0; i < 5; i++) {

		std::vector<cgmath::vec2> newPoints;

		// Para recorrer el arreglo
		for (int x = 0; x < newPos.size(); x++) {
			// formula
			//Qi = (3.0 / 4.0 * Pi) + (1.0 / 4.0 * Pi+1)
			//Ri = (1.0 / 4.0 * Pi) + (3.0 / 4.0 * Pi+1)

			int next = x + 1;

			if ((x + 1) == newPos.size()) {
				next = 0;
			}

			//std::cout << "next: " << next << "x: " << x << "\n";
			

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

	std::vector<cgmath::vec2> positions;

	ditto_points(positions);

	std::vector<cgmath::vec2> positionsChaikin = calculate_chaikin(positions);

	sizeDitto = positionsChaikin.size();
	std::cout << sizeDitto << " --> sizeDitto\n";

	// Crear un identificador para un
	// Vertex Array Object
	// Guarda el id en vao
	glGenVertexArrays(1, &vao);
	// Quiero comenzar a trabajar con
	// el siguiente vao
	glBindVertexArray(vao);

	// Crar un identificado para un
	// Vertex Buffer Object
	// Guardar el id en positionsVBO
	glGenBuffers(1, &positionsVBO);
	// Quiero trabajar con el buffer positionsBVO
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	// Crear la memoria del buffer,
	// especifica los datos
	// y la manda al GPU
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cgmath::vec2) * positionsChaikin.size(),
		positionsChaikin.data(),
		GL_DYNAMIC_DRAW);

	// Prendo el atributo 0
	glEnableVertexAttribArray(0);

	// Voy a configurar el atributo 0
	// Numero de components
	// Tipo de dato de cada componente
	// Normalizamos los datos??
	// Desfazamiento entre los atributos en la lista
	// Apuntador a los datos si no los hemos mandado
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Cuando hacemos un bind con 0
	// -> unbind
	// Unbind de positionsVBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Unbind del vao
	glBindVertexArray(0);

	primitiveType = GL_POINTS;
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

	// Bind del vao que tiene todos los atributos
	glBindVertexArray(vao);
	// Llamada a dibujar
	// tipo de primitiva
	// Desde que vertice se comienza a dibujar y 
	// cuantos vertices
	glDrawArrays(primitiveType, 0, sizeDitto);
	// Unbind del vao y todos los atributos
	glBindVertexArray(0);

}

void scene_chaikin::normalKeysDown(unsigned char key) {
	if (key == '1') {
		primitiveType = GL_POINTS;
	}

	if (key == '2') {
		primitiveType = GL_LINE_STRIP;
	}

	if (key == '3') {
		primitiveType = GL_TRIANGLES;
	}
}

void scene_chaikin::normalKeysUp(unsigned char key) {

}



void scene_chaikin::resize(int width, int height) {

}
