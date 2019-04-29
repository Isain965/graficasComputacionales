#pragma once

#include "scene.h"
#include <IL/il.h>

class scene_cube_texture : public scene {
	public:
		//destructores
		~scene_cube_texture();

		void init();
		void awake();
		void sleep();
		void reset() {}
		void mainLoop();
		void resize(int width, int height);
		void normalKeysDown(unsigned char key) {}
		void normalKeysUp(unsigned char key) {}
		void specialKeys(int key) {}
		void passiveMotion(int x, int y) {}

	private:
		GLuint shader_program;
		GLuint vao;
		GLuint positionsVBO;
		GLuint colorsVBO;
		GLuint indicesBuffer;

		//textures
		ILuint imageID;
		ILuint textureId;

		GLuint textureBuffer;

		//helpers
		float swidth = 400.0f;
		float sheight = 400.0f;
};