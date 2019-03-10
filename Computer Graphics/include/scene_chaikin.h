#pragma once

#include "scene.h"

class scene_chaikin : public scene {

	public:
		void init();
		void awake();
		void sleep();
		void reset() { }
		void mainLoop();
		void resize(int width, int height);
		void normalKeysDown(unsigned char key);
		void normalKeysUp(unsigned char key);
		void specialKeys(int key) { }
		void passiveMotion(int x, int y) { }

	private:
		// Este es el manager de memoria
		GLuint vao;
		GLuint vao_boca;
		GLuint vao_eye1;
		GLuint vao_eye2;

		GLuint vao_r;
		GLuint vao_boca_r;
		GLuint vao_eye1_r;
		GLuint vao_eye2_r;

		// Este es el buffer con el atributo
		GLuint positionsVBO;
		GLuint positionsVBO_boca;
		GLuint positionsVBO_eye1;
		GLuint positionsVBO_eye2;

		GLuint positionsVBO_r;
		GLuint positionsVBO_boca_r;
		GLuint positionsVBO_eye1_r;
		GLuint positionsVBO_eye2_r;

		//
		GLenum primitiveType;
		GLenum primitiveType_r;

		int sizeDitto;
		int sizeBoca;
		int sizeEye1;
		int sizeEye2;

		int sizeDitto_r;
		int sizeBoca_r;
		int sizeEye1_r;
		int sizeEye2_r;

};
