#include "model.h"

using namespace std; 

Model::Model()
{
	vao_id = vertex_count = 0;
	modelMatrix.loadIdentity(); 
}

Model::Model(const int id, const int size, GLuint position_vao, GLuint texture_vao, GLuint normal_vao)
{
	vao_id = id;
	vertex_count = size;
	pos_vbo = position_vao;
	tex_vbo = texture_vao;
	norm_vbo = normal_vao;
	modelMatrix.loadIdentity();
}

Model::Model(const int id, const int size, GLuint position_vao, GLuint normal_vao)
{
	vao_id = id;
	vertex_count = size;
	pos_vbo = position_vao;
	norm_vbo = normal_vao;
	modelMatrix.loadIdentity();
}

GLuint Model::get_id()
{
	return vao_id;
}

GLuint Model::get_texture()
{
	return texture_id; 
}

Mat4 Model::getModelMatrix()
{
	return modelMatrix; 
}

void Model::setModelMatrix(Mat4 matrix)
{
	modelMatrix = Mat4(matrix);
}

void Model::set_texture(GLuint texture)
{
	texture_id = texture;
}

int Model::get_vertexcount()
{
	return vertex_count;
}

GLuint Model::get_position_vbo()
{
	return pos_vbo;
}

GLuint Model::get_texture_vbo()
{
	return tex_vbo;
}

GLuint Model::get_normal_vbo()
{
	return norm_vbo;
}
