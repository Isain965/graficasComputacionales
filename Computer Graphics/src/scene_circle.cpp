#include "scene_circle.h"

#include "ifile.h"
#include "time.h"
#include "vec2.h"
#include "vec3.h"
#include <vector>


scene_circle::~scene_circle() {
	glDeleteProgram(shader_program);
}

void scene_circle::init() {

	std::vector<cgmath::vec2> circlePoints;

	int numLados = 40;
	float radius = 1.0f;
	float pi = 3.1416f;
	float circ = 2 * pi;

	// Este ciclo es utilizado para generar los puntos que conformaran el circulo
	// se utilizan senos y cosenos para poder calcular el angulo de apertura de los
	// puntos, El circulo se creara por medio de triangles fan, debemos asumir que 
	// esto funciona debido a que al declarar el vector "circle" ya contiene un punto
	// en las coordenadas (0,0), por eso ya solo debemos de agregar los puntos de la
	// circunferencia
	for (int i = 1; i < numLados; i++) {
		circlePoints.push_back(cgmath::vec2(radius * cos(i * circ / numLados), radius * sin(i * circ / numLados)));
	}

	sizeVao = circlePoints.size();

	// Este conjunto de instrucciones es para preparar el VAO del circulo y
	// posteriormente mandarlo a pintar, si solo se ejecuta asi tendremos 
	// un circulo blanco
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cgmath::vec2)*circlePoints.size(), circlePoints.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Su unico proposito en la vida es leer archivos de texto
	ifile shader_file;
	// El metodo read recibe la ruta al archivo de texto a leer
	// Si encuentra el archivo, intenta leerlo. En este caso,
	// estamos intentando leer un archivo llamado circle,
	// dentro de una carpeta shaders.
	shader_file.read("shaders/circle.vert");
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

	// Repetimos el mismo proceso, pero ahora para un
	// fragment shader contenido en un archivo llamado
	// circle.frag dentro de la carpeta shaders.
	shader_file.read("shaders/circle.frag");
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

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void scene_circle::awake() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glPointSize(10.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}


void scene_circle::sleep() {
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glPointSize(1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Para pintar los colores sobre el circulo
	glUseProgram(shader_program);
	GLuint resolution_location = glGetUniformLocation(shader_program, "iResolution");
	glUniform2f(resolution_location, 400, 400);

	// Para pintar el circulo blanco generado por los puntos
	// que generamos anteriormente
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0.0f, sizeVao);
	glBindVertexArray(0);
	// Aqui termina el pintado del circulo

	glUseProgram(0);
	// Aqui termina el pintado de colores
	// por medio del fragment shader
}

void scene_circle::resize(int width, int height) {

}
