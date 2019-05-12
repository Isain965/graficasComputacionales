#include "scene_perlin.h"
#include "ifile.h"
#include "time.h"
#include "cgmath.h"
#include "perlin.h"
#include <iostream>
#include "mat3.h"


// Declaracion de funciones
// Rotacion de camara
static void rotationX(cgmath::mat4 &mRotationX);
static void rotationY(cgmath::mat4 &mRotationY);
static void rotationZ(cgmath::mat4 &mRotationZ);
//Chaikin
std::vector<cgmath::vec3> calculate_chaikin(int refinamientos, std::vector<cgmath::vec3> positions, int boca);
// Calculo de las normales
void calculateNormal(std::vector<cgmath::vec3>& normales, std::vector<cgmath::vec3>& vertices, std::vector<unsigned int>& indices);



scene_perlin::~scene_perlin() {
	glDeleteProgram(shader_program);
	engine->drop(); // delete engine
}

void scene_perlin::init() {

	// Generacion de puntos que utilizara la camara
	std::vector< int> camPosX;
	for (int i = 0; i < 1; i++) {
		for (float y = 400.0f; y > 200.0f; y--) {
			camPosX.push_back(y);
		}
		for (float y = 200.0f; y < 400.0f; y++) {
			camPosX.push_back(y);
		}
	}
	std::vector< int> camPosY;
	for (int i = 0; i < 10; i++) {
		for (float y = 80.0f; y < 85.0f; y++) {
			camPosY.push_back(y);
			camPosY.push_back(y);
			camPosY.push_back(y);
			camPosY.push_back(y);
		}
		for (float y = 85.0f; y > 80.0f; y--) {
			camPosY.push_back(y);
			camPosY.push_back(y);
			camPosY.push_back(y);
			camPosY.push_back(y);
		}
	}
	std::vector< int> camPosZ;
	for (int i = 0; i < 2; i++) {
		for (float y = 150.0f; y > 50.0f; y--) {
			camPosZ.push_back(y);
		}
		for (float y = 50.0f; y < 150.0f; y++) {
			camPosZ.push_back(y);
		}
	}
	
	for (int x = 0; x < camPosX.size(); x++) {
		positions.push_back(cgmath::vec3(camPosX[x], camPosY[x], camPosZ[x]));
	}
	positions = calculate_chaikin(5, positions, 1);
	std::cout << positions.size() << std::endl;



	primitiveType = GL_LINE_STRIP;

	aspect = 1.0f;
	int size = 200;
	int nx = 100; //Width
	int ny = 100; //Heigh
	int cells = 100; // Densidad de vertices, dado que queremos generar cosas rocosas le pondremos 20

	std::vector<unsigned char> pixels;

	// Creear el objeto de Ruido de Perlin
	PerlinNoise pNoise;

	// Recorrer el terreno de 100 por 100 que se
	// definio en la parte de arriba en las variables
	// ny y nx
	for (int i = 0; i < ny; ++i) {
		for (int j = 0; j < nx; ++j) {

			double x = (double)j / (double)nx;
			double y = (double)i / (double)ny;

			// Generamos y guardamos el punto que se genera
			// por medio de ruido de Perlin
			double n = pNoise.noise(10 * x, 10 * y, 0.5f);
			n = n - floor(n);

			// Aqui guardamos el valor generado por
			// perlin multiplicado por un numero constante
			// con la finalidad de agregar más puntos a partir
			// de uno
			pixels.push_back(int(250 * n));
			pixels.push_back(int(250 * n));
			pixels.push_back(int(250 * n));
		}
	}

	// Parametros correspondientes a la textura
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nx, ny, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	std::vector<cgmath::vec3> vertices;
	std::vector<cgmath::vec2> uvs;
	std::vector<unsigned int> indices;

	float cell = size / (cells - 1);
	cgmath::vec2 position(100.0f, 0.0f);
	for (int i = 0; i < cells; ++i) {
		for (int j = 0; j < cells; ++j) {
			cgmath::vec3 vertex(position.x, 1.0f, position.y);
			cgmath::vec2 uv(position.x / size, position.y / size);

			vertices.push_back(vertex);
			uvs.push_back(uv);
			position.x += cell;
		}
		position.x = 0.0f;
		position.y += cell;
	}

	for (int i = 0; i < cells - 1; ++i) {
		for (int j = 0; j < cells - 1; ++j) {
			int bottomLeft = i * cells + j;
			int bottomRight = bottomLeft + 1;
			int topLeft = bottomLeft + cells;
			int topRight = topLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomRight);

			indices.push_back(bottomRight);
			indices.push_back(bottomLeft);
			indices.push_back(topLeft);
		}
	}
	count = indices.size();

	std::vector<cgmath::vec3> arrNormals(cells*cells);
	calculateNormal(arrNormals, vertices, indices);


	// Lineas de codigo para la comunicacion
	// de OpenGL, esto siempre se hace
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec4) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &UVSBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVSBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * arrNormals.size(), arrNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	ifile shader_file;
	shader_file.read("shaders/terrain.vert");
	std::string vertex_source = shader_file.get_contents();
	const GLchar* vertex_source_c = (const GLchar*)
		vertex_source.c_str();
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source_c,
		nullptr);
	glCompileShader(vertex_shader);

	shader_file.read("shaders/terrain.frag");
	std::string fragment_source = shader_file.get_contents();
	const GLchar* fragment_source_c = (const GLchar*)
		fragment_source.c_str();
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_c,
		nullptr);
	glCompileShader(fragment_shader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glBindAttribLocation(shader_program, 0, "VertexPosition");
	glBindAttribLocation(shader_program, 1, "VertexColor");
	glBindAttribLocation(shader_program, 2, "VertexNormal");
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(shader_program);
	GLuint resolution_location = glGetUniformLocation(shader_program, "iResolution");
	glUniform2f(resolution_location, 400, 400);

	GLuint light_color = glGetUniformLocation(shader_program, "LightColor");
	glUniform3f(light_color, 0.9f, 1.0f, 0.2f);

	GLuint light_position = glGetUniformLocation(shader_program, "LightPosition");
	glUniform3f(light_position, 100.0f, 10.0f, 50.0f);

	GLuint camera_position = glGetUniformLocation(shader_program, "CameraPosition");
	glUniform3f(camera_position, 2000.0f, 180.0f, 360.0f);

	flag = false;
	index = 0;

	glPointSize(20);


	/*Para la musica*/
	if (!engine) {
		printf("Could not startup engine\n");
		// error starting up the engine
	}

	// play some sound stream, looped
	engine->play2D("media/COTD.mp3", true);
}

void scene_perlin::awake() {
	glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_perlin::sleep() {
	glClearColor(0.2f, 0.3f, 0.15f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_perlin::mainLoop() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	// ***** Matriz de modelo *****
	if (!manual) {
		// Matriz de rotacion en X
		rotx = 1;

		// Matriz de rotacion en y
		rotationY(roty);

		// Matriz de rotacion en z
		rotz = 1;
	} else {
		// Matriz de rotacion en X
		rotx = 1;

		// Matriz de rotacion en y
		rotationY(roty);

		// Matriz de rotacion en z
		rotz = 1;
	}
	

	// Matriz con todas las rotaciones, Matriz de modelo
	cgmath::mat4 model = rotx * roty * rotz;


	if (index <= positions.size() && !flag) {
		flag = false;
		index++;
	}
	if (index == positions.size() || flag) {
		flag = true;
		index--;
	}
	if (index < 0) {
		index = 0;
		flag = false;
	}

	// ***** Matriz de Vista *****
	if (manual) {
		view_matrix[3][2] = distance; // Distancia
		view_matrix[3][1] = altura_scene * 1.5; // Altura
		view_matrix[3][0] = centerPosition; // Centrado
	} else {
		view_matrix = 1.0f;
		view_matrix[3][2] = positions[index].x;//200.0; // Que tan lenjos
		view_matrix[3][1] = positions[index].y * 1.5; // Altura
		view_matrix[3][0] = positions[index].z; // Centrado
	}
	
	view_matrix = cgmath::mat4::inverse(view_matrix);



	// ***** Matriz de proyeccion *****
	// Distancia al plano de corte lejano
	const float far = 1000.0f;
	// Distancia al plano de corte cercano
	const float near = 1.0f;
	const float half_fov = (30.0f) * (3.1416 / 180);

	cgmath::mat4 proj_matrix;
	proj_matrix[0][0] = 1.0f / (aspect * tan(half_fov));
	proj_matrix[1][1] = 1.0f / tan(half_fov);
	proj_matrix[2][2] = -((far + near) / (far - near));
	proj_matrix[2][3] = -1.0f;
	proj_matrix[3][2] = -((2.0f * far * near) / (far - near));

	// MVP Matrix
	cgmath::mat4 mvp = proj_matrix * view_matrix * model;

	GLuint mvp_location = glGetUniformLocation(shader_program, "mvpMatrix");
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

	//Model
	GLuint model_location = glGetUniformLocation(shader_program, "modelMatrix");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

	//Normal Matrix
	cgmath::mat3 normal_matrix3(model);
	cgmath::mat3 normal_matrix = cgmath::mat3::transpose(cgmath::mat3::inverse(normal_matrix3));

	GLuint normal_location = glGetUniformLocation(shader_program, "normalMatrix");
	glUniformMatrix3fv(normal_location, 1, GL_FALSE, &normal_matrix[0][0]);

	glBindVertexArray(vao);
	glDrawElements(primitiveType, count, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(vao);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);


	glUseProgram(0);
}

void scene_perlin::resize(int width, int height) {
	glViewport(0, 0, width, height);
	aspect = static_cast<float>(width) / static_cast<float>(height);
}

void scene_perlin::specialKeys(int key) {
	// std::cout << key << std::endl;

	// Para modificar el centrado
	if (manual && key == 100) {
		centerPosition = centerPosition - 5;
	}
	if (manual && key == 102) {
		centerPosition = centerPosition + 5;
	}

	// Para modificar la altura
	if (manual && key == 101) {
		altura_scene = altura_scene + 5;
	}
	if (manual && key == 103) {
		altura_scene = altura_scene - 5;
	}
}
void scene_perlin::normalKeysDown(unsigned char key) {
	// Para mostrar solo la figura bien
	if (key == '1') {
		primitiveType = GL_LINE_STRIP;
	}

	// Para resaltar las lineas originales que despues fueron deformadas
	if (key == '2') {
		primitiveType = GL_POINTS;
	}

	// Para mostrar sobre puestas las lineas reales sobre las deformadas
	if (key == '3') {
		primitiveType = GL_TRIANGLES;
	}


	// Para cambiar entre Vista Manual y Predetarminada
	if (key == 'm') {
		distance = 400;
		altura_scene = 85;
		centerPosition = 150;
		manual = !manual;

		char state = manual ? 'M': 'P';
		std::cout << "El estado actual de la camara: " << state << std::endl;
	}

	// Para modificar la distancia
	if (manual && key == 'z') {
		distance = distance + 5;
	}
	if (manual && key == 'x') {
		distance = distance - 5;
	}
}

static void rotationX(cgmath::mat4 &mRotationX) {
	// Funcion para calcular el movimiento de rotacion
	// en el eje x
	// del cubo por cada segun de ejecucion
	float t = time::elapsed_time().count();

	float angleX = (t * 30.0f) * (3.1416 / 180);

	cgmath::vec4 rotxx(1.0f, 0.0f, 0.0f, 0.0f);
	cgmath::vec4 rotxy(0.0f, cos(angleX), sin(angleX), 0.0f);
	cgmath::vec4 rotxz(0.0f, -sin(angleX), cos(angleX), 0.0f);
	cgmath::vec4 rotxw(0.0f, 0.0f, 0.0f, 1.0f);
	mRotationX = cgmath::mat4(rotxx, rotxy, rotxz, rotxw);
}

static void rotationY(cgmath::mat4 &mRotationY) {
	// Funcion para calcular el movimiento de rotacion
	// en el eje y
	// del cubo por cada segun de ejecucion
	float t = time::elapsed_time().count();

	float angleY = (t * -10.0f) * (3.1416 / 180);

	cgmath::vec4 rotyx(cos(angleY), 0.0f, -sin(angleY), 0.0f);
	cgmath::vec4 rotyy(0.0f, 1.0f, 0.0f, 0.0f);
	cgmath::vec4 rotyz(sin(angleY), 0.0f, cos(angleY), 0.0f);
	cgmath::vec4 rotyw(0.0f, 0.0f, 0.0f, 1.0f);
	mRotationY = cgmath::mat4(rotyx, rotyy, rotyz, rotyw);
}

static void rotationZ(cgmath::mat4 &mRotationZ) {
	// Funcion para calcular el movimiento de rotacion
	// en el eje z
	// del cubo por cada segun de ejecucion
	float t = time::elapsed_time().count();

	float angleZ = (t * 30.0f) * (3.1416 / 180);

	cgmath::vec4 rotzx(cos(angleZ), sin(angleZ), 0.0f, 0.0f);
	cgmath::vec4 rotzy(-sin(angleZ), cos(angleZ), 0.0f, 0.0f);
	cgmath::vec4 rotzz(0.0f, 0.0f, 1.0f, 0.0f);
	cgmath::vec4 rotzw(0.0f, 0.0f, 0.0f, 1.0f);
	mRotationZ = cgmath::mat4(rotzx, rotzy, rotzz, rotzw);
}

void calculateNormal(std::vector<cgmath::vec3>& normales, std::vector<cgmath::vec3>& vertices, std::vector<unsigned int>& indices) {
	// Funcion para calcular la norma de los puntos,
	// esto es para generar la luz
	for (int i = 0; i < indices.size(); i += 3) {
		cgmath::vec3 p0 = vertices[indices[i + 0]];
		cgmath::vec3 p1 = vertices[indices[i + 1]];
		cgmath::vec3 p2 = vertices[indices[i + 2]];

		cgmath::vec3 e1 = p0 - p1;
		cgmath::vec3 e2 = p0 - p2;
		cgmath::vec3 normal = cgmath::vec3::normalize(cgmath::vec3::cross(e1, e2));

		normales[indices[i + 0]] += normal;
		normales[indices[i + 1]] += normal;
		normales[indices[i + 2]] += normal;
	}
}

std::vector<cgmath::vec3> calculate_chaikin(int refinamientos, std::vector<cgmath::vec3> positions, int boca) {
	std::vector<cgmath::vec3> newPos = positions;
	// Para los refinamientos
	for (int i = 0; i < refinamientos; i++) {

		std::vector<cgmath::vec3> newPoints;

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
			float Qz = (3.0 / 4.0 * newPos[x].z) + (1.0 / 4.0 * newPos[next].z);

			float Rx = (3.0 / 4.0 * newPos[next].x) + (1.0 / 4.0 * newPos[x].x);
			float Ry = (3.0 / 4.0 * newPos[next].y) + (1.0 / 4.0 * newPos[x].y);
			float Rz = (3.0 / 4.0 * newPos[next].z) + (1.0 / 4.0 * newPos[x].z);

			cgmath::vec3 Q = cgmath::vec3(Qx, Qy, Qz);
			cgmath::vec3 R = cgmath::vec3(Rx, Ry, Rz);

			newPoints.push_back(Q);
			newPoints.push_back(R);
		}

		newPos = newPoints;
		newPoints.clear();
	}

	return newPos;

}


//Referencias:
// https://github.com/teodorplop/OpenGL-Procedural-Terrain/
// Alfonso Alquicer
// https://www.ambiera.com/irrklang/

