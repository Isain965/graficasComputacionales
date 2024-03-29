﻿#include "scene_sphere.h"

#include "ifile.h"
#include "time.h"
#include <vector>
#include <string>
#include <iostream>

scene_sphere::~scene_sphere() {
	glDeleteProgram(shader_program);
}

void scene_sphere::init() {
	// ifile es parte del codigo que yo les doy
	// El codigo fuente se encuentra en el proyecto Util
	// Su unico proposito en la vida es leer archivos de texto
	ifile shader_file;
	// El metodo read recibe la ruta al archivo de texto a leer
	// Si encuentra el archivo, intenta leerlo. En este caso,
	// estamos intentando leer un archivo llamado grid,
	// dentro de una carpeta shaders.
	shader_file.read("shaders/sphere.vert");
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

	// Adicionalmente, habria que verificar que el proceso de
	// compilacion ha sido completado de manera exitosa.
	// Por tiempo, simplemente asumimos que el codigo no
	// tenia errores.

	// Repetimos el mismo proceso, pero ahora para un
	// fragment shader contenido en un archivo llamado
	// solid_color.frag dentro de la carpeta shaders.
	shader_file.read("shaders/solid_color.frag");
	std::string fragment_source = shader_file.get_contents();
	const GLchar* fragment_source_c = (const GLchar*)fragment_source.c_str();
	// El identificador del shader lo creamos pero para un 
	// shader de tipo fragment.
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source_c, nullptr);
	glCompileShader(fragment_shader);

	// Una vez que hemos creado los shaders necesarios,
	// creamos el manager utilizando la funcion glCreateProgram
	// que regresa el id.
	shader_program = glCreateProgram();
	// Utilizamos glAttachShader para asociar un shader con el manager
	// En este caso, shader_program es manager de vertex_shader
	glAttachShader(shader_program, vertex_shader);
	// En este caso, shader_program es manager de fragment_shader
	glAttachShader(shader_program, fragment_shader);
	// Ejecutamos el proceso de linkeo. En esta etapa se busca
	// que los shaders puedan trabajar en conjunto y todo este
	// definido correctamente.
	glLinkProgram(shader_program);

	// Tambien deberiamos verificar que el proceso de linkeo
	// termine sin errores. Por tiempo, asumimos que el
	// resultado fue correcto.
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


	// Borramos los shaders, porque ya tenemos el ejecutable 
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void scene_sphere::awake() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);
	GLuint time_location = glGetUniformLocation(shader_program, "time");
	glUniform1f(time_location, time::elapsed_time().count());
	glDrawArrays(GL_POINTS, 0, 20000);
	glUseProgram(0);

	glUseProgram(shader_program);
	GLuint resolution_location = glGetUniformLocation(shader_program, "iResolution");
	glUniform2f(resolution_location, 400.0, 400.0);
	glUseProgram(0);

}

void scene_sphere::resize(int width, int height) {
	
}