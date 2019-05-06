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
void calculate_chaikin(int refinamientos, std::vector<cgmath::vec3> positions);
// Calculo de las normales
void calculateNormal(std::vector<cgmath::vec3>& normales, std::vector<cgmath::vec3>& vertices, std::vector<unsigned int>& indices);



scene_perlin::~scene_perlin() {
	glDeleteProgram(shader_program);
}

void scene_perlin::init() {

	primitiveType = GL_LINE_STRIP;

	aspect = 1.0f;
	int size = 180;
	int nx = 100; //Width
	int ny = 100; //Heigh
	int cells = 20; // Densidad de vertices, dado que queremos generar cosas rocosas le pondremos 20

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
			pixels.push_back(int(225 * n));
			pixels.push_back(int(200 * n));
		}
	}

	// Los puntos de Perlin generados
	// anteriormete ahora los guardaremos como
	// Unsigned binary integer
	std::vector<GLubyte> perlinPoints;
	perlinPoints.insert(perlinPoints.end(), pixels.data(), pixels.data() + (nx*ny));
	

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
	glUniform3f(light_color, 1.0f, 1.0f, 1.0f);

	GLuint light_position = glGetUniformLocation(shader_program, "LightPosition");
	glUniform3f(light_position, -10.0f, 10.0f, 10.0f);

	GLuint camera_position = glGetUniformLocation(shader_program, "CameraPosition");
	glUniform3f(camera_position, 140.0f, 45.0f, 150.0f);
}

void scene_perlin::awake() {
	glClearColor(0.1f, 0.3f, 0.0f, 1.0f);
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
	// Matriz de rotacion en X
	cgmath::mat4 rotx(1);
	// Matriz de rotacion en y
	cgmath::mat4 roty;
	rotationY(roty);
	// Matriz de rotacion en z
	cgmath::mat4 rotz(1);
	//rotationZ(rotz);
	// Matriz con todas las rotaciones, Matriz de modelo
	cgmath::mat4 model = rotx * roty * rotz;

	// ***** Matriz de Vista *****
	cgmath::mat4 view_matrix(1.0f);
	view_matrix[3][2] = 200.0f; // Que tan lenjos
	view_matrix[3][1] = 100.0f; // Altura
	view_matrix[3][0] = 100.0f; // Centrado
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

	float angleY = (t * 60.0f) * (3.1416 / 180);

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

void calculate_chaikin(int refinamientos, std::vector<cgmath::vec3> positions) {
	// Funcion para calcular chaikin
	std::vector<cgmath::vec3> p;

	for (unsigned int i = 0; i < (positions.size() - 1); ++i) {

		const cgmath::vec3& p0 = positions[i];
		const cgmath::vec3& p1 = positions[i + 1];
		cgmath::vec3 Q;
		cgmath::vec3 R;


		Q.x = 0.75f*p0.x + 0.25f*p1.x;
		Q.y = 0.75f*p0.y + 0.25f*p1.y;
		Q.z = 0.75f*p0.z + 0.25f*p1.z;

		R.x = 0.25f*p0.x + 0.75f*p1.x;
		R.y = 0.25f*p0.y + 0.75f*p1.y;
		R.z = 0.25f*p0.z + 0.75f*p1.z;

		p.push_back(Q);
		p.push_back(R);
	}
	p.push_back(positions[positions.size() - 1]);

	positions = p;
}




//Referencias :
// https://github.com/teodorplop/OpenGL-Procedural-Terrain/
// Alfonso Alquicer