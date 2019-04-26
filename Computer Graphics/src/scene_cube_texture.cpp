#include "scene_cube_texture.h"
#include "ifile.h"
#include "mat4.h"
#include "time.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include <vector>

#include <IL/il.h>

// Funciones para la creacion del cubo
static void cube_points_body(std::vector<cgmath::vec3> &positions);
static void cube_index(std::vector<unsigned int> &index);
static void cube_colores_body(std::vector<cgmath::vec3> &colores);

// Funciones para la rotacion del cubo
static void rotationX(cgmath::mat4 &mRotationX);
static void rotationY(cgmath::mat4 &mRotationY);
static void rotationZ(cgmath::mat4 &mRotationZ);


scene_cube_texture::~scene_cube_texture() {
	glDeleteProgram(shader_program);
}

void scene_cube_texture::init() {
	// Creacion de los puntos que formaran el cubo
	std::vector<cgmath::vec3> positions;
	cube_points_body(positions);

	// Asignacion de colores a cada vertice del cubo
	std::vector<cgmath::vec3> colores;
	cube_colores_body(colores);

	// Indicar el orden de los vertices
	std::vector<unsigned int> indices;
	cube_index(indices);

	// Para la textura
	std::vector<cgmath::vec2> texturePositions;

	for (int i = 0; i < 6; i++) {
		texturePositions.push_back(cgmath::vec2(0.0f, 0.0f));
		texturePositions.push_back(cgmath::vec2(1.0f, 0.0f));
		texturePositions.push_back(cgmath::vec2(1.0f, 1.0f));
		texturePositions.push_back(cgmath::vec2(0.0f, 1.0f));
	}


	// PARA LAS IMAGENES ****************
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilLoadImage("img/crate.png");

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

	ilBindImage(0);
	ilDeleteImages(1, &imageID);

	// Segunda Imagen
	/*ilGenImages(1, &imageID2);
	ilBindImage(imageID2);
	ilLoadImage("img/pig.png");

	glGenTextures(1, &textureId2);
	glBindTexture(GL_TEXTURE_2D, textureId2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

	ilBindImage(0);
	ilDeleteImages(1, &imageID2);*/
	// PARA LAS IMAGENES ^^^^^^^^^^^^^^^^


	// Conjunto de instrucciones para prepara el VAO
	// que utiliza el cubo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * colores.size(), colores.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * texturePositions.size(), texturePositions.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	ifile shader_file;
	shader_file.read("shaders/texture.vert");
	std::string vertex_source = shader_file.get_contents();
	const GLchar* vertex_source_c = (const GLchar*)vertex_source.c_str();
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source_c, nullptr);
	glCompileShader(vertex_shader);

	shader_file.read("shaders/texture.frag");
	std::string fragment_source = shader_file.get_contents();
	const GLchar* fragment_source_c = (const GLchar*)fragment_source.c_str();
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_c, nullptr);
	glCompileShader(fragment_shader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glBindAttribLocation(shader_program, 0, "VertexPosition");
	glBindAttribLocation(shader_program, 1, "VertexColor");
	glLinkProgram(shader_program);

	GLint vertex_compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log;
		log.resize(log_length);
		glGetShaderInfoLog(vertex_shader, log_length, &log_length,
			&log[0]);

		std::cout << ">>> Errors in Vertex shader\n";
		for (int i = 0; i < log.size(); i++) {
			std::cout << log[i];
		}
		std::cout << ">>> END Errors in Vertex shader";
	}

	GLint fragment_compiled;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE) {
		GLint log_length;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log_shader;
		log_shader.resize(log_length);
		glGetShaderInfoLog(fragment_shader, log_length, &log_length,
			&log_shader[0]);
		std::cout << ">>> Errors in Fragment shader\n";
		for (int i = 0; i < log_shader.size(); i++) {
			std::cout << log_shader[i];
		}
		std::cout << ">>> END Errors in Fragment shader";
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	aspect = 1.0f;
}

void scene_cube_texture::awake() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
}

void scene_cube_texture::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
}

void scene_cube_texture::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	// ***** Matriz de modelo *****
	// Matriz de rotacion en X
	cgmath::mat4 rotx;
	rotationX(rotx);

	// Matriz de rotacion en y
	cgmath::mat4 roty;
	rotationY(roty);

	// Matriz de rotacion en z
	cgmath::mat4 rotz;
	rotationZ(rotz);

	// Matriz con todas las rotaciones, Matriz de modelo
	cgmath::mat4 model_matrix = rotx * roty * rotz;

	// ****** Matriz de vista ******
	// Colocacion de la camara en las
	// coordenadas (0, 0, 10)
	cgmath::mat4 view_matrix(1.0f);
	view_matrix[3][2] = 10.0f;
	view_matrix = cgmath::mat4::inverse(view_matrix);

	// Matriz de proyeccion
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
	cgmath::mat4 mvp = proj_matrix * view_matrix * model_matrix;

	// Finalmente Pintar el cubo
	GLuint mvp_location = glGetUniformLocation(shader_program, "mvpMatrix");
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(vao);

	glUseProgram(0);
}

void scene_cube_texture::resize(int width, int height) {

}


static void cube_points_body(std::vector<cgmath::vec3> &positions) {

	// Estos valores tienen el valor de 3
	// ya que al poner 2 con signo diferente 
	// se forma una distancia de 6 entre estos
	// y asi es como se especifica en el documento
	const float x = 3.0f;
	const float y = 3.0f;
	const float z = 3.0f;

	// Cara frontal
	positions.push_back(cgmath::vec3(-x, -y, z));
	positions.push_back(cgmath::vec3(x, -y, z));
	positions.push_back(cgmath::vec3(x, y, z));
	positions.push_back(cgmath::vec3(-x, y, z));

	// Cara derecha
	positions.push_back(cgmath::vec3(x, -y, z));
	positions.push_back(cgmath::vec3(x, -y, -z));
	positions.push_back(cgmath::vec3(x, y, -z));
	positions.push_back(cgmath::vec3(x, y, z));

	// Cara trasera
	positions.push_back(cgmath::vec3(x, -y, -z));
	positions.push_back(cgmath::vec3(-x, -y, -z));
	positions.push_back(cgmath::vec3(-x, y, -z));
	positions.push_back(cgmath::vec3(x, y, -z));

	// Cara izquierda
	positions.push_back(cgmath::vec3(-x, -y, -z));
	positions.push_back(cgmath::vec3(-x, -y, z));
	positions.push_back(cgmath::vec3(-x, y, z));
	positions.push_back(cgmath::vec3(-x, y, -z));

	// Cara superior
	positions.push_back(cgmath::vec3(-x, y, z));
	positions.push_back(cgmath::vec3(x, y, z));
	positions.push_back(cgmath::vec3(x, y, -z));
	positions.push_back(cgmath::vec3(-x, y, -z));

	// Cara inferior
	positions.push_back(cgmath::vec3(-x, -y, -z));
	positions.push_back(cgmath::vec3(x, -y, -z));
	positions.push_back(cgmath::vec3(x, -y, z));
	positions.push_back(cgmath::vec3(-x, -y, z));
}

static void cube_index(std::vector<unsigned int> &index) {
	//Asginar a un vector el orden de los vertices del cubo
	index = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
}

static void cube_colores_body(std::vector<cgmath::vec3> &colores) {
	// Por cada lado del cubo se le agrega a cada uno de sus vertices
	// el mismo color, de no hacer esto tendriamos un hermoso difuminado

	// CARA DE COLOR ROJO
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f));

	// CARA DE COLOR VERDE
	colores.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f));

	// CARA DE COLOR AZUL
	colores.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f));

	// CARA DE COLOR ROSA
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 0.0f, 1.0f));

	// CARA DE COLOR AMARILLA
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 0.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 0.0f));

	// Cara de color balnca,
	// esta cara nunca se muestra
	// debido a los angulos de rotacion
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 1.0f));
	colores.push_back(cgmath::vec3(1.0f, 1.0f, 1.0f));
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