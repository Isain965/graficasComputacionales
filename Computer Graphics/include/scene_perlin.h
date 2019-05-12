#pragma once

#include "scene.h"
#include <IL/il.h>
#include <string>
#include <vector>
#include "vec3.h"
#include "vec2.h"
#include "mat4.h"

#include <IK/irrKlang.h>

class scene_perlin : public scene {

public:
	~scene_perlin();

	void init();
	void awake();
	void sleep();
	void reset() { }
	void mainLoop();
	void resize(int width, int height);
	void normalKeysDown(unsigned char);
	void normalKeysUp(unsigned char) { }
	void specialKeys(int key);
	void passiveMotion(int x, int y) { }

	// start the sound engine with default parameters
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	// Para la rotacion manual
	cgmath::mat4 rotx;
	cgmath::mat4 roty;
	cgmath::mat4 rotz;

	cgmath::mat4 view_matrix;

	// Para la cercania
	float distance = 400;

	// Para la altura
	float altura_scene = 85;

	// Para el centro
	float centerPosition = 150;


	bool manual = false;


	
	
	private:
		std::vector<cgmath::vec3> positions;
		float size;
		GLuint vao;

		float aspect;

		int count;
		GLuint shader_program;
		GLuint positionsVBO;
		GLuint UVSBO;
		GLuint normalesVBO;
		GLuint indicesBuffer;

		// Para la textura
		GLuint textureId;

		// Para poder cambiar la primitiva
		GLenum primitiveType;

		int index;

		bool flag;
};