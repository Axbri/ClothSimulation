#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h> 
#include <sstream>
#include <vector>

#include "model.h"

using namespace std;

class Loader
{
public:
	Model createModel(float positions[], int size_pos, float textureCoords[], int size_texCoords, float normals[], int size_normals, int indices[], int size_index);
	Model createModel(float positions[], int size_pos, int indices[], int size_index);
	Model createTexturelessModel(float positions[], int size_pos, float normals[], int size_normals, int indices[], int size_index);
	GLuint create2Dmodel(float positions[], int size_pos);
	GLuint createModel(float positions[], int size_pos);
	GLuint loadBMPtexture(const char *imagepath);
	GLuint loadBMPcubeTexture(const char *pos_x, const char *neg_x, const char *pos_y, const char *neg_y, const char *pos_z, const char *neg_z);
	void cleanUp(); 

	void updateDataInAtributeList(GLuint vbo, float data[], int n);

private:

	void loadTexImage2D(GLenum target, const char *imagepath);

	GLuint createVao(); 
	void bindIndicesBuffer(int indices[], int n);
	GLuint storeDataInAtributeList(int attributeNumber, int coordinateSize, float data[], int n);
	
	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;
};
