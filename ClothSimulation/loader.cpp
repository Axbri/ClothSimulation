#include "loader.h"

using namespace std;

// creates a vertax array object and uploads it to the GPU, a new model object is created containing a refferance to the VAO. 
Model Loader::createModel(float positions[], int size_pos, float textureCoords[], int size_texCoords, float normals[], int size_normals, int indices[], int size_index)
{	
	GLuint vao = createVao();
	bindIndicesBuffer(indices, size_index); 
	GLuint pos_vbo = storeDataInAtributeList(0, 3, positions, size_pos);
	GLuint tex_vbo = storeDataInAtributeList(1, 2, textureCoords, size_texCoords);
	GLuint norm_vbo = storeDataInAtributeList(2, 3, normals, size_normals);
	glBindVertexArray(0);
	return Model(vao, size_index, pos_vbo, tex_vbo, norm_vbo);
}

Model Loader::createTexturelessModel(float positions[], int size_pos, float normals[], int size_normals, int indices[], int size_index)
{
	GLuint vao = createVao();
	bindIndicesBuffer(indices, size_index);
	GLuint pos_vbo = storeDataInAtributeList(0, 3, positions, size_pos);
	GLuint norm_vbo = storeDataInAtributeList(1, 3, normals, size_normals);
	glBindVertexArray(0);
	return Model(vao, size_index, pos_vbo, norm_vbo);
}

GLuint Loader::create2Dmodel(float positions[], int size_pos)
{
	GLuint vao = createVao();
	GLuint pos_vbo = storeDataInAtributeList(0, 2, positions, size_pos);
	glBindVertexArray(0);
	return vao; 
}

// Creates a new empty vertex array object and returns it's ID. 
GLuint Loader::createVao()
{
	GLuint vao = 0;	
	glGenVertexArrays(1, &vao);
	vaos.push_back(vao);
	glBindVertexArray(vao);
	return vao; 
}

// Creates a new index-buffer contrining all the specified indices. 
void Loader::bindIndicesBuffer(int indices[], int n)
{
	// create index buffer
	GLuint index_vbo = 0;
	glGenBuffers(1, &index_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n * sizeof(int), indices, GL_STATIC_DRAW);
}

// Creates a new vertex buffer and fills it with the specified data. CoordinateSize is the number of 
// ellements per vertex, for example 3 for 3D position and 2 for texture coordinate. 
GLuint Loader::storeDataInAtributeList(int attributeNumber, int coordinateSize, float data[], int n)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	// TODO add vbo to list so it can be cleared later when the program closes
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo; 
}

void Loader::updateDataInAtributeList(GLuint vbo, float data[], int n)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), data, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, n, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Loades a BMP-image and creates a texture using the data. 
// The texture is uploaded to the GPU and the index of the texture is returned. 
GLuint Loader::loadBMPtexture(const char * imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3	
	unsigned char *data; // Actual RGB data

	// Open the file
	FILE *file;
	fopen_s(&file, imagepath, "rb");

	//FILE * file = fopen(imagepath, "rb");
	if (!file) 
	{ 
		printf("Image could not be opened\n"); 
		return 0; 
	}

	// The first thing in the file is a 54-bytes header. Read it. If not 54 bytes read : problem
	if (fread(header, 1, 54, file) != 54) 
	{ 
		printf("Not a correct BMP file\n");
		return false;		
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;		
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    
		imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component

	if (dataPos == 0)      
		dataPos = 54; // The BMP header is done that way

					  // Create a buffer
	data = new unsigned char[imageSize];
	
	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	
	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// använd lite mer högupplöst mipmapar även för objekt långt borta för att de ska se bra ut. 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);			
	// använd anastropisk filtrering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); 

	textures.push_back(textureID);
	return textureID;
}

// Call this function before the pogram exits. This function deletes all 
// the Vertex array objects, vertex buffer objects and all textures. 
void Loader::cleanUp()
{
	for (GLuint vao : vaos)
	{
		glDeleteVertexArrays(1, &vao);
	}

	for (GLuint vbo : vbos)
	{
		glDeleteBuffers(1, &vbo);
	}

	for (GLuint texture : textures)
	{
		glDeleteTextures(1, &texture);
	}

	vaos.clear(); 
	vbos.clear();
	textures.clear();
}