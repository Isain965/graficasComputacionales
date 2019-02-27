#include <iostream>

// Siempre que quieran utilizar OpenGL en
// algun archivo, tienen que incluirlos
// en este orden
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "time.h"

float angle = 0;


void render_loop() {
	time::tick();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// glClearColor(sin(time::elapsed_time().count()), 1.0f, 1.0f, 0.5f);

	// WARNING: Esto es OpenGL viejo y solo lovamos a usar en
	// esta clase.

	glPushMatrix();

	//glRotatef(180.0f * time::delta_time().count(), 1.0f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);

	glEnd();


	glPopMatrix();
	angle += 1.0f;
	// WARNING

	glutSwapBuffers();

	//OpenGL entra en un estado de espera.
}

void idle() {
	// Cuando OpenGL esta en un estado de espera,
	// cuelve a dibujar.
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {

	time::tick();

	// Inicializar Freeglut
	// Freeglut es la libreria para abrir ventanas
	// y configurar el contexto de OpenGL
	glutInit(&argc, argv);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Hello World!");
	glutDisplayFunc(render_loop);
	glutIdleFunc(idle);

	// Inicializar GLEW
	// GLEW busca el API de OpenGL directamente en la tarjeta de
	// video. Microsoft no lo expone directamente.
	glewInit();

	// Configurar OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	// Mostramos la version de OpenGL con la que trabajamos
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Inicializar la libreria de tiempo
	time::init();

	// Ejecutar el ciclo infinito (render loop)
	// se pausa la ejecucion del main
	glutMainLoop();


	return 0;
}