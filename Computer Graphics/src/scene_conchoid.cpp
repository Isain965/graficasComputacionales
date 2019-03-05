#include "scene_conchoid.h"

#include "vec2.h"

#include <vector>

// Se manda a llamar una vez
// cuando inicia la aplicacion
void scene_conchoid::init() {

	std::vector<cgmath::vec2> upLine;
	std::vector<cgmath::vec2> downLine;


	float k = 0.75;
	cgmath::vec2 o = cgmath::vec2(0, -0.5);

	for (float x = -2; x < 2; x += 0.1) {
		cgmath::vec2 p(x, 0);
		// std::cout << p << " este es p\n";
		cgmath::vec2 vecRest = (o - p);
		vecRest.normalize();
		cgmath::vec2 d = vecRest * k;
		cgmath::vec2 q1 = p - d;
		cgmath::vec2 q2 = p + d;
		//std::cout << q1 << " este es q1\n";
		std::cout << q2 << " este es q2\n";
		upLine.push_back(q1);
		downLine.push_back(q2);
	}

	sizeUpLine = upLine.size();

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
		sizeof(cgmath::vec2) * upLine.size(),
		upLine.data(),
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


	// *****************************************************
	// Crear un identificador para un
	// Vertex Array Object
	// Guarda el id en vao
	glGenVertexArrays(1, &vao2);
	// Quiero comenzar a trabajar con
	// el siguiente vao
	glBindVertexArray(vao2);

	// Crar un identificado para un
	// Vertex Buffer Object
	// Guardar el id en positionsVBO

	glGenBuffers(1, &positionsVBO2);

	// Quiero trabajar con el buffer positionsBVO
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO2);

	// Crear la memoria del buffer,
	// especifica los datos
	// y la manda al GPU
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cgmath::vec2) * upLine.size(),
		downLine.data(),
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
	// *****************************************************

	primitiveType = GL_LINE_STRIP;
}

void scene_conchoid::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glPointSize(20.0f);
}

void scene_conchoid::sleep() {
	glPointSize(1.0f);
}

void scene_conchoid::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind del vao que tiene todos los atributos
	glBindVertexArray(vao);
	// Llamada a dibujar
	// tipo de primitiva
	// Desde que vertice se comienza a dibujar y 
	// cuantos vertices
	glDrawArrays(primitiveType, 0, sizeUpLine);
	// Unbind del vao y todos los atributos
	glBindVertexArray(0);

	// **************************************

	// Bind del vao que tiene todos los atributos
	glBindVertexArray(vao2);
	// Llamada a dibujar
	// tipo de primitiva
	// Desde que vertice se comienza a dibujar y 
	// cuantos vertices
	glDrawArrays(primitiveType, 0, sizeUpLine);
	// Unbind del vao y todos los atributos
	glBindVertexArray(0);

	// **************************************

}

void scene_conchoid::normalKeysDown(unsigned char key) {
	/* if (key == '3') {
		primitiveType = GL_LINE_STRIP;
	}

	if (key == '2') {
		primitiveType = GL_LINE_STRIP;
	} */

	if (key == '1') {
		primitiveType = GL_LINE_STRIP;
	}
}

void scene_conchoid::normalKeysUp(unsigned char key) {

}

void scene_conchoid::resize(int width, int height) {

}