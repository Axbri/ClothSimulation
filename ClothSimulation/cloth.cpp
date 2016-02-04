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

	GLuint clothTexture = loader.loadBMPtexture("textil01.bmp");
	clothModel.set_texture(clothTexture);
}

Cloth::~Cloth()
{
	cleanUp(); 
}

// update the cloth, this function will handle the phisics simulation of the cloth. 
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

	updateNormals(); 
	
	updateVBOs(); 
}

// render this cloth on the provided window from the provided camera's viewpoint. 
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

// update the vertex buffer objects for positions and normals on the GPU with new data. 
void Cloth::updateVBOs()
{
	// create the new model-arrays: 
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	int positionIndex = 0, normalIndex = 0;

	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].xPos;
			positions[positionIndex++] = particles[x][y].yPos;
			positions[positionIndex++] = particles[x][y].zPos;
			normals[normalIndex++] = particles[x][y].xNormal;
			normals[normalIndex++] = particles[x][y].yNormal;
			normals[normalIndex++] = particles[x][y].zNormal;
		}
	}
	theLoader.updateDataInAtributeList(clothModel.get_position_vbo(), positions, positionIndex);
	theLoader.updateDataInAtributeList(clothModel.get_normal_vbo(), normals, normalIndex);
}

// calculate the normals of all particles. 
void Cloth::updateNormals()
{
	Vec3 pos1{};
	Vec3 pos2{};
	Vec3 pos3{};

	Vec3 vector1{};
	Vec3 vector2{};
	Vec3 normal{};

	for (int x = 0; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y = 0; y < NUMBER_OF_VERTICES - 1; y++)
		{
			int topLeft = (y * NUMBER_OF_VERTICES) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((y + 1) * NUMBER_OF_VERTICES) + x;
			int bottomRight = bottomLeft + 1;

			pos1.set(particles[x][y].xPos, particles[x][y].yPos, particles[x][y].zPos);
			pos2.set(particles[x + 1][y].xPos, particles[x + 1][y].yPos, particles[x + 1][y].zPos);
			pos3.set(particles[x][y + 1].xPos, particles[x][y + 1].yPos, particles[x][y + 1].zPos);

			vector1.set(pos2);
			vector1.subtract(pos1);
			vector2.set(pos3);
			vector2.subtract(pos2);
			normal.set(0, 0, 0);
			normal.cross(vector1, vector2);

			particles[x][y].xNormal = normal.x;
			particles[x][y].yNormal = normal.y;
			particles[x][y].zNormal = normal.z;
			particles[x + 1][y].xNormal = normal.x;
			particles[x + 1][y].yNormal = normal.y;
			particles[x + 1][y].zNormal = normal.z;
			particles[x][y + 1].xNormal = normal.x;
			particles[x][y + 1].yNormal = normal.y;
			particles[x][y + 1].zNormal = normal.z;
		}
	}

	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].xNormal = normal.x;
	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].yNormal = normal.y;
	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].zNormal = normal.z;
}
