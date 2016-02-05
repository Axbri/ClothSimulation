#include "cloth.h"

Cloth::Cloth(Loader loader, double size, double totalWeight)
{
	theLoader = loader;
	shader.createShader( "mainVertex.glsl", "mainFragment.glsl" );
	this->size = size;

	// create all the particles
	double particleMass = totalWeight / (NUMBER_OF_VERTICES * NUMBER_OF_VERTICES); 
	restlength = size / (NUMBER_OF_VERTICES - 1);

	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			double particleXpos = ((double)x / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleYpos = -((double)y / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleZpos = 0.01 * (sin(x) + sin(y));
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
	int indicesIndex{ 0 }, positionIndex{ 0 }, textureCoordIndex{ 0 }, normalIndex{ 0 };

	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].pos.x;
			positions[positionIndex++] = particles[x][y].pos.y;
			positions[positionIndex++] = particles[x][y].pos.z;
			textureCoords[textureCoordIndex++] = particles[x][y].textureCoord.x;
			textureCoords[textureCoordIndex++] = particles[x][y].textureCoord.y;
			normals[normalIndex++] = particles[x][y].normal.x;
			normals[normalIndex++] = particles[x][y].normal.y;
			normals[normalIndex++] = particles[x][y].normal.z;
		}
	}
	
	int topLeft{ 0 }, topRight{ 0 }, bottomLeft{ 0 }, bottomRight{ 0 };
	for (int x{ 0 }; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES - 1; y++)
		{
			topLeft = (y * NUMBER_OF_VERTICES) + x;
			topRight = topLeft + 1;
			bottomLeft = ((y + 1) * NUMBER_OF_VERTICES) + x;
			bottomRight = bottomLeft + 1;
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
	Vec3 g = Vec3(0, -9.81, 0);
	double step = 0.02;
	time_passed += delta_time;
	if (time_passed > step)
	{
		time_passed -= step;
		// update the particle's positions
		for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
		{
			for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
			{
				//particles[x][y].pos.z = 0.1 * (sin(x/4.0 + time*4) + sin(y / 4.0 + time*4));
				double mass = particles[x][y].mass;
				Vec3 p_curr = Vec3(particles[x][y].pos);
				Vec3 p_old = Vec3(particles[x][y].pos_old);

				particles[x][y].pos += p_curr - p_old + g*mass*step*step;
				particles[x][y].pos_old = p_curr;

				for (int r{ 0 }; r < 1; r++) 
				{
					if (y < NUMBER_OF_VERTICES-1) {
						// ||||
						Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
						double deltalength = ((delta*delta / restlength) + restlength) / 2;
						double diff = (deltalength - restlength) / (deltalength * 2 / mass);

						particles[x][y].pos += delta*(1 / mass)*diff;
						particles[x][y+1].pos -= delta*(1 / mass)*diff;

					}
					if(x < NUMBER_OF_VERTICES-1) {
						// ----
						Vec3 delta = particles[x+1][y].pos - particles[x][y].pos;
						double deltalength = ((delta*delta / restlength) + restlength) / 2;
						double diff = (deltalength - restlength) / (deltalength * 2 / mass);

						particles[x][y].pos += delta*(1 / mass)*diff;
						particles[x+1][y].pos -= delta*(1 / mass)*diff;

					}
					if (y < NUMBER_OF_VERTICES - 1) {
						// ||||
						Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
						double deltalength = ((delta*delta / restlength) + restlength) / 2;
						double diff = (deltalength - restlength) / (deltalength * 2 / mass);

						particles[x][y].pos += delta*(1 / mass)*diff;
						particles[x][y + 1].pos -= delta*(1 / mass)*diff;

					}
					if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) {
						// cross forward
						Vec3 delta = particles[x + 1][y + 1].pos - particles[x][y].pos;
						double deltalength = ((delta*delta / 1.4142*restlength) + 1.4142*restlength) / 2;
						double diff = (deltalength - 1.4142*restlength) / (deltalength * 2 / mass);

						particles[x][y].pos += delta*(1 / mass)*diff;
						particles[x + 1][y + 1].pos -= delta*(1 / mass)*diff;


						// cross back
						delta = particles[x + 1][y].pos - particles[x][y + 1].pos;
						deltalength = ((delta*delta / 1.4142*restlength) + 1.4142*restlength) / 2;
						diff = (deltalength - 1.4142*restlength) / (deltalength * 2 / mass);

						particles[x][y + 1].pos += delta*(1 / mass)*diff;
						particles[x + 1][y].pos -= delta*(1 / mass)*diff;

					}
				}
			}
		}
	}
	particles[0][0].pos = Vec3(0, 0, 0);
	particles[int((NUMBER_OF_VERTICES-1)/2)][0].pos = Vec3(size/2, 0, 0);
	particles[NUMBER_OF_VERTICES - 1][0].pos = Vec3(size, 0, 0);

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
	int positionIndex{ 0 }, normalIndex{ 0 };

	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].pos.x;
			positions[positionIndex++] = particles[x][y].pos.y;
			positions[positionIndex++] = particles[x][y].pos.z;
			normals[normalIndex++] = particles[x][y].normal.x;
			normals[normalIndex++] = particles[x][y].normal.y;
			normals[normalIndex++] = particles[x][y].normal.z;
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

	int topLeft{ 0 }, topRight{ 0 }, bottomLeft{ 0 }, bottomRight{ 0 };

	for (int x{ 0 }; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES - 1; y++)
		{
			topLeft = (y * NUMBER_OF_VERTICES) + x;
			topRight = topLeft + 1;
			bottomLeft = ((y + 1) * NUMBER_OF_VERTICES) + x;
			bottomRight = bottomLeft + 1;

			pos1.set(particles[x][y].pos.x, particles[x][y].pos.y, particles[x][y].pos.z);
			pos2.set(particles[x + 1][y].pos.x, particles[x + 1][y].pos.y, particles[x + 1][y].pos.z);
			pos3.set(particles[x][y + 1].pos.x, particles[x][y + 1].pos.y, particles[x][y + 1].pos.z);

			vector1.set(pos2);
			vector1.subtract(pos1);
			vector2.set(pos3);
			vector2.subtract(pos2);
			normal.set(0, 0, 0);
			normal.cross(vector1, vector2);

			particles[x][y].normal.x = normal.x;
			particles[x][y].normal.y = normal.y;
			particles[x][y].normal.z = normal.z;
			particles[x + 1][y].normal.x = normal.x;
			particles[x + 1][y].normal.y = normal.y;
			particles[x + 1][y].normal.z = normal.z;
			particles[x][y + 1].normal.x = normal.x;
			particles[x][y + 1].normal.y = normal.y;
			particles[x][y + 1].normal.z = normal.z;
		}
	}

	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].normal.x = normal.x;
	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].normal.y = normal.y;
	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].normal.z = normal.z;
}
