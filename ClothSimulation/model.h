#pragma once

#include <GL/glew.h> 
#include "matrixmath.h"
using namespace std;

class Model
{
	public:
		Model();
		Model(const int id, const int size, GLuint position_vao, GLuint texture_vao, GLuint normal_vao);

		GLuint get_id();
		GLuint get_texture(); 
		void set_texture(GLuint texture);
		int get_vertexcount();

		GLuint get_position_vbo();
		GLuint get_texture_vbo();
		GLuint get_normal_vbo();

		void getModelMatrix(float matrix[]);

		void setPosition(float x, float y, float z); 
		void setScale(float sx, float sy, float sz);

	private: 
		GLuint vao_id;
		GLuint pos_vbo;
		GLuint tex_vbo;
		GLuint norm_vbo;
		GLuint texture_id; 
		int vertex_count;
		GLfloat modelMatrix[16];
};