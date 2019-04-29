#include "scene_cube_texture.h"

#include "ifile.h"
#include "time.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "vec2.h"

#include <iostream>
#include <vector>

static cgmath::mat4 getModelMatrix(float thetaX, float thetaY, float thetaZ, float sx, float sy, float sz, float vx, float vy, float vz) {
	cgmath::mat4 rotationMatrixXY(cgmath::vec4(cos(thetaZ), sin(thetaZ), 0, 0), cgmath::vec4(-sin(thetaZ), cos(thetaZ), 0, 0), cgmath::vec4(0, 0, 1, 0), cgmath::vec4(0, 0, 0, 1));
	cgmath::mat4 rotationMatrixXZ(cgmath::vec4(cos(thetaY), 0, -sin(thetaY), 0), cgmath::vec4(0, 1, 0, 0), cgmath::vec4(sin(thetaY), 0, cos(thetaY), 0), cgmath::vec4(0, 0, 0, 1));
	cgmath::mat4 rotationMatrixYZ(cgmath::vec4(1, 0, 0, 0), cgmath::vec4(0, cos(thetaX), sin(thetaX), 0), cgmath::vec4(0, -sin(thetaX), cos(thetaX), 0), cgmath::vec4(0, 0, 0, 1));
	cgmath::mat4 scaleMatrix(cgmath::vec4(sx, 0, 0, 0), cgmath::vec4(0, sy, 0, 0), cgmath::vec4(0, 0, sz, 0), cgmath::vec4(0, 0, 0, 1));
	cgmath::mat4 translationMatrix(cgmath::vec4(sx, 0, 0, 0), cgmath::vec4(0, sy, 0, 0), cgmath::vec4(0, 0, sz, 0), cgmath::vec4(0, 0, 0, 1));
	return rotationMatrixXY * rotationMatrixYZ * rotationMatrixXZ * scaleMatrix * translationMatrix;
}

//perspective
static cgmath::mat4 getProjectionMatrix(float aspect, float fov, float far, float near) {
	return cgmath::mat4(
		cgmath::vec4(1 / (aspect*tan(fov / 2)), 0, 0, 0),
		cgmath::vec4(0, 1 / tan(fov / 2), 0, 0),
		cgmath::vec4(0, 0, -(far + near) / (far - near), -1),
		cgmath::vec4(0, 0, -(2 * far*near) / (far - near), 0));
}

scene_cube_texture::~scene_cube_texture()
{
	// Borramos la memoria del ejecutable cuando
	// la escena deja de existir.
	glDeleteProgram(shader_program);
	glDeleteTextures(1, &textureId);
}

void scene_cube_texture::init()
{
	//loading Texture
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilLoadImage("img/pig.png");

	//...some logic step 3
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

	ilBindImage(0);
	ilDeleteImages(1, &imageID);

	float length = 6.0f;

	std::vector<cgmath::vec3> positions;
	std::vector<cgmath::vec3> colors;
	std::vector<cgmath::vec2> texturePositions;

	//bottom
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, -length / 2));//0
	positions.push_back(cgmath::vec3(length / 2, -length / 2, -length / 2));//1
	positions.push_back(cgmath::vec3(length / 2, length / 2, -length / 2));//2
	positions.push_back(cgmath::vec3(-length / 2, length / 2, -length / 2));//3

	for (int i = 0; i < 6; i++) {
		texturePositions.push_back(cgmath::vec2(0.0f, 0.0f));//0
		texturePositions.push_back(cgmath::vec2(1.0f, 0.0f));//1
		texturePositions.push_back(cgmath::vec2(1.0f, 1.0f));//2
		texturePositions.push_back(cgmath::vec2(0.1f, 1.0f));//3
	}

	//up
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, length / 2));//4
	positions.push_back(cgmath::vec3(length / 2, -length / 2, length / 2));//5
	positions.push_back(cgmath::vec3(length / 2, length / 2, length / 2));//6
	positions.push_back(cgmath::vec3(-length / 2, length / 2, length / 2));//7

	//front
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, -length / 2));//8
	positions.push_back(cgmath::vec3(length / 2, -length / 2, -length / 2));//9
	positions.push_back(cgmath::vec3(length / 2, -length / 2, length / 2));//10
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, length / 2));//11

	//back
	positions.push_back(cgmath::vec3(length / 2, length / 2, -length / 2));//12
	positions.push_back(cgmath::vec3(-length / 2, length / 2, -length / 2));//13
	positions.push_back(cgmath::vec3(-length / 2, length / 2, length / 2));//14
	positions.push_back(cgmath::vec3(length / 2, length / 2, length / 2));//15

	//left
	positions.push_back(cgmath::vec3(-length / 2, length / 2, -length / 2));//16
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, -length / 2));//17
	positions.push_back(cgmath::vec3(-length / 2, -length / 2, length / 2));//18
	positions.push_back(cgmath::vec3(-length / 2, length / 2, length / 2));//19

	//right
	positions.push_back(cgmath::vec3(length / 2, -length / 2, -length / 2));//20
	positions.push_back(cgmath::vec3(length / 2, length / 2, -length / 2));//21
	positions.push_back(cgmath::vec3(length / 2, length / 2, length / 2));//22
	positions.push_back(cgmath::vec3(length / 2, -length / 2, length / 2));//23

	std::vector<unsigned int> indices{
		//up
		4,5,6,6,7,4,
		//down
		0,3,2,2,1,0,
		//front
		8,9,10,10,11,8,
		//right
		20,21,22,22,23,20,
		//back
		12,13,14,14,15,12,
		//left
		16,17,18,18,19,16,
	};

	colors.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f)); //red
	colors.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f)); //red
	colors.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f)); //red
	colors.push_back(cgmath::vec3(1.0f, 0.0f, 0.0f)); //red

	colors.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f)); //green
	colors.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f)); //green
	colors.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f)); //green
	colors.push_back(cgmath::vec3(0.0f, 1.0f, 0.0f)); //green

	colors.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f)); //blue
	colors.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f)); //blue
	colors.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f)); //blue
	colors.push_back(cgmath::vec3(0.0f, 0.0f, 1.0f)); //blue

	colors.push_back(cgmath::vec3(1.0f, 0.5f, 0.0f));//naranja
	colors.push_back(cgmath::vec3(1.0f, 0.5f, 0.0f));
	colors.push_back(cgmath::vec3(1.0f, 0.5f, 0.0f));
	colors.push_back(cgmath::vec3(1.0f, 0.5f, 0.0f));

	colors.push_back(cgmath::vec3(0.1f, 0.2f, 0.5f));
	colors.push_back(cgmath::vec3(0.1f, 0.2f, 0.5f));
	colors.push_back(cgmath::vec3(0.1f, 0.2f, 0.5f));
	colors.push_back(cgmath::vec3(0.1f, 0.2f, 0.5f));

	colors.push_back(cgmath::vec3(0.3f, 0.1f, 0.2f));
	colors.push_back(cgmath::vec3(0.3f, 0.1f, 0.2f));
	colors.push_back(cgmath::vec3(0.3f, 0.1f, 0.2f));
	colors.push_back(cgmath::vec3(0.3f, 0.1f, 0.2f));

	// Creacion y activacion del vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Creacion y configuracion del buffer del atributo de posicion
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//atributos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//jasd, vec2, tipo float ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Creacion y configuracion del buffer del atributo de color
	//glGenBuffers(1, &colorsVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);//atributos
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//jasd, vec2, tipo float ...
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//textures buffers
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2) * texturePositions.size(), texturePositions.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// A diferencia de los buffers de atributos, los buffers de indices deben permanecer activos. No hacemos unbind.
	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);//element array de indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindVertexArray(1);

	// ifile es parte del codigo que yo les doy
	// El codigo fuente se encuentra en el proyecto Util
	// Su unico proposito en la vida es leer archivos de texto
	ifile shader_file;
	// El metodo read recibe la ruta al archivo de texto a leer
	// Si encuentra el archivo, intenta leerlo. En este caso,
	// estamos intentando leer un archivo llamado grid,
	// dentro de una carpeta shaders.
	shader_file.read("shaders/cube_texture.vert");
	// Obtenemos los contenidos leidos en el paso anterior
	// utilizando el metodo get_contents. Regresa un string
	std::string vertex_source = shader_file.get_contents();
	// OpenGL es un API de C, por lo que no trabaja con
	// strings de C++. Tenemos que hacer un cast a un tipo de
	// dato que OpenGL entienda. Podemos usar strings de C (char*)
	// o utilizar el tipo de dato definido por OpenGL (GLchar*).
	// Preferimos lo segundo.
	const GLchar* vertex_source_c = (const GLchar*)vertex_source.c_str();
	// Creamos el identificador para un vertex shader,
	// utiliznado la funcion glCreateShader. La funcion
	// regresa el identificador y lo guardamos en la variable
	// vertex_shader.
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// Utilizando la funcion glShaderSource, indicamos que queremos
	// enviar el codigo fuente del shader. La funcion espera:
	// Identificador del shader (vertex_shader)
	// Cuantos codigos fuentes queremos manadar (1)
	// El c�digo fuente (vertex_source_c)
	// La longitud del codigo fuente. Si usamos nullptr, se
	// asume que debe continuar leyendo hasta encontrar un nullptr.
	glShaderSource(vertex_shader, 1, &vertex_source_c, nullptr);
	// Compilamos el codigo fuente contenido en el shader
	// con identificador vertex_shader.
	glCompileShader(vertex_shader);

	// Revision de errores de compilacion del vertex shader
	GLint vertex_compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE)
	{
		GLint log_length;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log;
		log.resize(log_length);
		glGetShaderInfoLog(vertex_shader, log_length, &log_length, &log[0]);
		std::cout << "Syntax errors in vertex shader: " << std::endl;
		for (auto& c : log) std::cout << c;
		std::cout << std::endl;
	}

	// Repetimos el mismo proceso, pero ahora para un
	// fragment shader contenido en un archivo llamado
	// solid_color.frag dentro de la carpeta shaders.
	shader_file.read("shaders/cube_texture.frag");
	std::string fragment_source = shader_file.get_contents();
	const GLchar* fragment_source_c = (const GLchar*)fragment_source.c_str();
	// El identificador del shader lo creamos pero para un 
	// shader de tipo fragment.
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_c, nullptr);
	glCompileShader(fragment_shader);

	// Revision de errores de compilacion del fragment shader
	GLint fragment_compiled;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE)
	{
		GLint log_length;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log;
		log.resize(log_length);
		glGetShaderInfoLog(fragment_shader, log_length, &log_length, &log[0]);
		std::cout << "Syntax errors in fragment shader: " << std::endl;
		for (auto& c : log) std::cout << c;
		std::cout << std::endl;
	}

	// Una vez que hemos creado los shaders necesarios,
	// creamos el manager utilizando la funcion glCreateProgram
	// que regresa el id.
	shader_program = glCreateProgram();
	// Utilizamos glAttachShader para asociar un shader con el manager
	// En este caso, shader_program es manager de vertex_shader
	glAttachShader(shader_program, vertex_shader);
	// En este caso, shader_program es manager de fragment_shader
	glAttachShader(shader_program, fragment_shader);
	//asignar al atributo 0 hacer antes del link program
	glBindAttribLocation(shader_program, 0, "VertexPosition");
	glBindAttribLocation(shader_program, 1, "VertexCoord");
	// Ejecutamos el proceso de linkeo. En esta etapa se busca
	// que los shaders puedan trabajar en conjunto y todo este
	// definido correctamente.
	glLinkProgram(shader_program);

	// Tambien deberiamos verificar que el proceso de linkeo
	// termine sin errores. Por tiempo, asumimos que el
	// resultado fue correcto.

	// Borramos los shaders, porque ya tenemos el ejecutable
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//Enviamos la resolucion de la ventana al inicio de la aplicacion, la variable uniform vec2 asignarle valor con 
	glUseProgram(shader_program);
	GLuint resolution_location = glGetUniformLocation(shader_program, "iResolution");
	glUniform2f(resolution_location, 400.0f, 400.0f);
	glUseProgram(0);
}

void scene_cube_texture::awake()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_cube_texture::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_cube_texture::mainLoop()
{
	//Building matrix
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

	float t = time::elapsed_time().count();

	float pi = 3.14159f;

	thetaX = t * (30.0f * (pi / 200.0f));
	thetaY = t * (60.0f * (pi / 200.0f));
	thetaZ = t * (30.0f * (pi / 200.0f));

	float sx = 1.0f;
	float sy = 1.0f;
	float sz = 1.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float vz = 0.0f;

	cgmath::mat4 modelCameraMatrix(1.0f);
	modelCameraMatrix[3] = cgmath::vec4(0.0f, 0.0f, 10.0f, 1.0f);

	//model matrix
	cgmath::mat4 modelMatrix = getModelMatrix(thetaX, thetaY, thetaZ, sx, sy, sz, vx, vy, vz);
	cgmath::mat4 viewMatrix = cgmath::mat4::inverse(modelCameraMatrix);

	float aspect = swidth / sheight;
	float fov = 60 * (pi / 200.0f);
	float far = 1000.0f;
	float near = 1.0f;
	cgmath::mat4 projectionMatrix = getProjectionMatrix(aspect, fov, far, near);

	//mvp matrix uniform to shaders
	cgmath::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	GLuint mvpm = glGetUniformLocation(shader_program, "mvp");
	glUniformMatrix4fv(mvpm, 1, GL_FALSE, &mvp[0][0]);

	GLuint resolution_location = glGetUniformLocation(shader_program, "resolution");
	glUniform2f(resolution_location, 400.0f, 400.0f);
	GLuint time_location = glGetUniformLocation(shader_program, "time");
	glUniform1f(time_location, time::elapsed_time().count());
	glBindVertexArray(vao);

	//textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	GLuint tex_location = glGetUniformLocation(shader_program, "textures");
	glUniform1i(tex_location, 0);

	//glDrawArrays(GL_POINTS, 0, 100);//cuando no hay indices
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	//textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void scene_cube_texture::resize(int width, int height)
{
	swidth = float(width);
	sheight = float(height);
	glViewport(0, 0, width, height);
	glUseProgram(shader_program);
	GLuint resolution_location = glGetUniformLocation(shader_program, "iResolution");
	glUniform2f(resolution_location, GLfloat(width), GLfloat(height));
	glUseProgram(0);
}