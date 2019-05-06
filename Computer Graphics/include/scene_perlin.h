#pragma once

#include "scene.h"
#include <IL/il.h>
#include <string>
#include <vector>
#include "vec3.h"
#include "vec2.h"
#include "mat4.h"

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
	void specialKeys(int key) { }
	void passiveMotion(int x, int y) { }
	
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
};