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
	Model createTexturelessModel(float positions[], int size_pos, float normals[], int size_normals, int indices[], int size_index);
	GLuint create2Dmodel(float positions[], int size_pos);
	GLuint loadBMPtexture(const char * imagepath);
	void cleanUp(); 

	void updateDataInAtributeList(GLuint vbo, float data[], int n);

private:
	GLuint createVao(); 
	void bindIndicesBuffer(int indices[], int n);
	GLuint storeDataInAtributeList(int attributeNumber, int coordinateSize, float data[], int n);
	
	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;
};
