#include "cloth.h"




Cloth::Cloth(Loader loader, double size, double totalWeight)
{
	theLoader = loader;

	shader.createShader( "mainVertex.glsl", "mainFragment.glsl" );

	// create all the particles
	double particleMass = totalWeight / (NUMBER_OF_VERTICES * NUMBER_OF_VERTICES); 

	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES; y++)
		{
			double particleXpos = ((double)x / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleYpos = -((double)y / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleZpos = 0.1 * (sin(x) + sin(y));
			double particleUtextureCoord = ((double)x / (double)NUMBER_OF_VERTICES);
			double particleVtextureCoord = ((double)y / (double)NUMBER_OF_VERTICES);
			
			particles[x][y] = Particle(particleXpos, particleYpos, particleZpos, particleUtextureCoord, particleVtextureCoord, particleMass);
		}
	}

	
	// create the model objcet
	int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6];
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	int indicesIndex = 0, positionIndex = 0, textureCoordIndex = 0, normalIndex = 0;

	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].xPos;
			positions[positionIndex++] = particles[x][y].yPos;
			positions[positionIndex++] = particles[x][y].zPos;		
			textureCoords[textureCoordIndex++] = particles[x][y].uTexture;
			textureCoords[textureCoordIndex++] = particles[x][y].vTexture;
			normals[normalIndex++] = particles[x][y].xNormal;
			normals[normalIndex++] = particles[x][y].yNormal;
			normals[normalIndex++] = particles[x][y].zNormal;
		}
	}

	for (int x = 0; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES - 1; y++)
		{
			int topLeft = (y * NUMBER_OF_VERTICES) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((y + 1) * NUMBER_OF_VERTICES) + x;
			int bottomRight = bottomLeft + 1;
			indices[indicesIndex++] = topLeft;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = bottomRight;
		}
	}

	clothModel = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, normals, normalIndex, indices, indicesIndex);	
	clothModel.setPosition(-1, 2, 0);

	GLuint clothTexture = loader.loadBMPtexture("gaze01.bmp");
	clothModel.set_texture(clothTexture);
}

Cloth::~Cloth()
{

}


void Cloth::update(double delta_time, double time)
{
	// update the particle's positions
	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES; y++)
		{
			particles[x][y].zPos = 0.1 * (sin(x/4.0 + time*4) + sin(y / 4.0 + time*4));
		}
	}

	// update the VBOs on the GPU with the new data
	updateVBOs(); 
}


void Cloth::render(GLFWwindow * window, Camera camera)
{
	float projectionMatrix[16];
	float viewMatrix[16];
	float modelMatrix[16];

	camera.getProjectionMatrix(projectionMatrix);
	camera.getViewMatrix(viewMatrix);
	clothModel.getModelMatrix(modelMatrix);

	shader.start();
	shader.setUniformMat4("projectionMatrix", projectionMatrix);
	shader.setUniformMat4("viewMatrix", viewMatrix);	
	shader.setUniformMat4("modelMatrix", modelMatrix);

	glBindVertexArray(clothModel.get_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, clothModel.get_texture());
	glDrawElements(GL_TRIANGLES, clothModel.get_vertexcount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	
	shader.stop();
}

void Cloth::cleanUp()
{
	shader.cleanUp(); 
}

void Cloth::updateVBOs()
{
	// create the new model-arrays: 
	int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6];
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	int indicesIndex = 0, positionIndex = 0, textureCoordIndex = 0, normalIndex = 0;

	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].xPos;
			positions[positionIndex++] = particles[x][y].yPos;
			positions[positionIndex++] = particles[x][y].zPos;
			textureCoords[textureCoordIndex++] = particles[x][y].uTexture;
			textureCoords[textureCoordIndex++] = particles[x][y].vTexture;
			normals[normalIndex++] = particles[x][y].xNormal;
			normals[normalIndex++] = particles[x][y].yNormal;
			normals[normalIndex++] = particles[x][y].zNormal;
		}
	}

	theLoader.updateDataInAtributeList(clothModel.get_position_vbo(), positions, positionIndex);
	theLoader.updateDataInAtributeList(clothModel.get_texture_vbo(), textureCoords, textureCoordIndex);
	theLoader.updateDataInAtributeList(clothModel.get_normal_vbo(), normals, normalIndex);
}
