#include "cloth.h"

Cloth::Cloth(Loader loader, Vec3 pos, double size, double totalWeight)
{
	theLoader = loader;
	shader.createShader( "mainVertex.glsl", "mainFragment.glsl" );
	this->size = size;
	this->useContraints = true; 
	this->position = pos;	// position of the cloth in the world. this is used to generate the model matrix. 

	// create all the particles
	double particleMass = totalWeight / (NUMBER_OF_VERTICES * NUMBER_OF_VERTICES); 
	restlength = size / (NUMBER_OF_VERTICES - 1);

	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			double particleXpos = ((double)x / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleZpos = ((double)y / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleYpos = 0.02 * (sin(particleXpos * 20) + cos(particleZpos * 20));
			double particleUtextureCoord = ((double)x / (double)NUMBER_OF_VERTICES);
			double particleVtextureCoord = ((double)y / (double)NUMBER_OF_VERTICES);			
			particles[x][y] = Particle(particleXpos, particleYpos, particleZpos, particleUtextureCoord, particleVtextureCoord, particleMass);
		}
	}
		
	// create the model objcet
	int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6 * 2];
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3 * 2];
	float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2 * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3 * 2];
	int indicesIndex{ 0 }, positionIndex{ 0 }, textureCoordIndex{ 0 }, normalIndex{ 0 };
		
	// data for front side
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
		
	// data for back side
	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].pos.x;
			positions[positionIndex++] = particles[x][y].pos.y;
			positions[positionIndex++] = particles[x][y].pos.z;
			textureCoords[textureCoordIndex++] = -particles[x][y].textureCoord.x;
			textureCoords[textureCoordIndex++] = particles[x][y].textureCoord.y;
			normals[normalIndex++] = -particles[x][y].normal.x;
			normals[normalIndex++] = -particles[x][y].normal.y;
			normals[normalIndex++] = -particles[x][y].normal.z;
		}
	}
	
	int topLeft{ 0 }, topRight{ 0 }, bottomLeft{ 0 }, bottomRight{ 0 };

	// indeices for front side
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

	// indeices for back side
	for (int x{ 0 }; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES - 1; y++)
		{
			topLeft = (NUMBER_OF_VERTICES * NUMBER_OF_VERTICES) + (y * NUMBER_OF_VERTICES) + x;
			topRight = topLeft + 1;
			bottomLeft = (NUMBER_OF_VERTICES * NUMBER_OF_VERTICES) + ((y + 1) * NUMBER_OF_VERTICES) + x;
			bottomRight = bottomLeft + 1;
			indices[indicesIndex++] = topLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomRight;
		}
	}
	clothModel = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, normals, normalIndex, indices, indicesIndex);	
	
	contraintsTexture = loader.loadBMPtexture("constraints.bmp");
	massSpringTexture = loader.loadBMPtexture("massSpring.bmp");
	clothModel.set_texture(contraintsTexture);
}

Cloth::~Cloth()
{
	cleanUp(); 
}

void Cloth::setMode(bool useContraints)
{
	this->useContraints = useContraints; 
	
	if (useContraints)
	{		
		clothModel.set_texture(contraintsTexture);
	}
	else
	{
		clothModel.set_texture(massSpringTexture);
	}
}

void Cloth::reset()
{
	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			double particleXpos = ((double)x / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleZpos = ((double)y / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleYpos = 0.02 * (sin(particleXpos*20) + cos(particleZpos*20));
			particles[x][y].pos = Vec3{ particleXpos , particleYpos , particleZpos };
			particles[x][y].force = Vec3{ 0, 0, 0 }; 
		}
	}
}

// update the cloth, this function will handle the phisics simulation of the cloth. 
void Cloth::update(double delta_time, double time, vector<Sphere> allSpheres)
{
	double step = 0.016; // 0.016; // limit to 60 updates per second
	time_passed += delta_time;
	if (time_passed > step)
	{
		time_passed -= step;

		double dampening  = 0.98; 
		double springs[3] = { 10000, 10000, 10000 };
		//double springs[3] = { 2000, 2000, 2000 };
		Vec3 gravity{ 0,-9.81,0 };

		dampening = 0.98;
		gravity = Vec3{ 0,-9.81,0 };
		
		if(!(this->useContraints))
		{
			 dampening = 0.92;
			 gravity = Vec3{ 0,-19.81,0 };
			reset_forces();
		}
				
		for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
		{
			for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
			{
				if (this->useContraints)
				{
					resolve_constraint(x, y);
					verlet(x, y, step, dampening, gravity);
					collision(x, y, &allSpheres);
				} 
				else 
				{
					calculate_force(x, y, springs);
					verlet(x, y, step, dampening, gravity);
					collision(x, y, &allSpheres);
				}
			}
		}
	}

	// hang the clockth in the top vertices
	for (int i{ 0 }; i < (NUMBER_OF_VERTICES); i++)
	{
		if (i % 8 == 0 || i % 8 == 1)
		{
			particles[i][0].pos = Vec3{ ((double)i / (double)(NUMBER_OF_VERTICES - 1)) * size, 0, 0 };
		}			
	}

	// update the model matrix desribing the position of this cloth in the world. 
	Mat4 modelMatrix;
	modelMatrix.loadTranslation(position.x, position.y, position.z);
	clothModel.setModelMatrix(modelMatrix);

	updateNormals(); 	
	updateVBOs(); 
}

// render this cloth on the provided window from the provided camera's viewpoint. 
void Cloth::render(GLFWwindow * window, Camera camera, vector<Light> allLights)
{
	shader.start();
	shader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	shader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	shader.setUniformMat4("modelMatrix", clothModel.getModelMatrix());
	Light::loadLightsToShader(shader, allLights);

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
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3 * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3 * 2];
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

	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			positions[positionIndex++] = particles[x][y].pos.x;
			positions[positionIndex++] = particles[x][y].pos.y;
			positions[positionIndex++] = particles[x][y].pos.z;
			normals[normalIndex++] = -particles[x][y].normal.x;
			normals[normalIndex++] = -particles[x][y].normal.y;
			normals[normalIndex++] = -particles[x][y].normal.z;
		}
	}
	theLoader.updateDataInAtributeList(clothModel.get_position_vbo(), positions, positionIndex);
	theLoader.updateDataInAtributeList(clothModel.get_normal_vbo(), normals, normalIndex);
}

// calculate the normals of all particles. 
void Cloth::updateNormals()
{
	Vec3 pos1{}, pos2{}, pos3{}, vector1{}, vector2{}, normal{};
	for (int x{ 0 }; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES - 1; y++)
		{
			pos1 = particles[x][y].pos;
			pos2 = particles[x + 1][y].pos;
			pos3 = particles[x][y + 1].pos;
			vector1 = pos3 - pos1;
			vector2 = pos2 - pos1;
			normal = Vec3{ 0, 0, 0 };
			normal.cross(vector1, vector2);
			particles[x][y].normal = normal;
			particles[x + 1][y].normal = normal;
			particles[x][y + 1].normal = normal;
		}
	}
	particles[NUMBER_OF_VERTICES - 1][NUMBER_OF_VERTICES - 1].normal = normal;
}

void Cloth::resolve_constraint(int x, int y) {
	double nv = (double)NUMBER_OF_VERTICES;
	double con_inf[3] = { nv / (120 + nv), nv / (120 + nv), nv / (45 + nv) };  // behöver tweakas, detta funkar okej. Värden nära 1 generellt ostabila (0.2,0.2,0.4) för 30 verts
	//resolve constraints
	//for (int r{ 0 }; r < 1; r++)
	//{
		if (y < NUMBER_OF_VERTICES - 1) 
		{
			// ||||
			Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
			double deltalength = delta.length();
			double diff = (deltalength - restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x][y + 1].pos -= delta*0.5*diff*con_inf[0];
		}

		if (x < NUMBER_OF_VERTICES - 1) 
		{
			// ----
			Vec3 delta = particles[x + 1][y].pos - particles[x][y].pos;
			double deltalength = delta.length();
			double diff = (deltalength - restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x + 1][y].pos -= delta*0.5*diff*con_inf[0];
		}

		if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) 
		{
			// cross forward
			Vec3 delta = particles[x + 1][y + 1].pos - particles[x][y].pos;
			double deltalength = delta.length();
			double diff = (deltalength - 1.4142*restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[1];
			particles[x + 1][y + 1].pos -= delta*0.5*diff*con_inf[1];
		}

		if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) 
		{
			// cross back
			Vec3 delta = particles[x + 1][y].pos - particles[x][y + 1].pos;
			double deltalength = delta.length();
			double diff = (deltalength - 1.4142*restlength) / deltalength;
			particles[x][y + 1].pos += delta*0.5*diff*con_inf[1];
			particles[x + 1][y].pos -= delta*0.5*diff*con_inf[1];
		}

		if (y < NUMBER_OF_VERTICES - 2) 
		{
			// ||||
			Vec3 delta = particles[x][y + 2].pos - particles[x][y].pos;
			double deltalength = delta.length();
			double diff = (deltalength - 2 * restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[2];
			particles[x][y + 2].pos -= delta*0.5*diff*con_inf[2];
		}

		if (x < NUMBER_OF_VERTICES - 2) {
			// ----
			Vec3 delta = particles[x + 2][y].pos - particles[x][y].pos;
			double deltalength = delta.length();
			double diff = (deltalength - 2 * restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x + 2][y].pos -= delta*0.5*diff*con_inf[0];
		}
	//}
}

void Cloth::calculate_force(int x, int y, double k[]) {

	if (y < NUMBER_OF_VERTICES - 1) 
	{
		// ||||
		Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
		double deltalength = delta.length();
		double diff = (deltalength - restlength) / deltalength;
		particles[x][y].force += delta*diff*k[0];
		particles[x][y + 1].force -= delta*diff*k[0];
	}

	if (x < NUMBER_OF_VERTICES - 1) 
	{
		// ----
		Vec3 delta = particles[x + 1][y].pos - particles[x][y].pos;
		double deltalength = delta.length();
		double diff = (deltalength - restlength) / deltalength;
		particles[x][y].force += delta*diff*k[0];
		particles[x + 1][y].force -= delta*diff*k[0];
	}

	if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) 
	{
		// cross forward

		Vec3 delta = particles[x + 1][y + 1].pos - particles[x][y].pos;
		double deltalength = delta.length();
		double diff = (deltalength - 1.4142*restlength) / deltalength;
		particles[x][y].force += delta*diff*k[1];
		particles[x + 1][y + 1].force -= delta*diff*k[1];
	}

	if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) 
	{
		// cross back
		Vec3 delta = particles[x + 1][y].pos - particles[x][y + 1].pos;
		double deltalength = delta.length();
		double diff = (deltalength - 1.4142*restlength) / deltalength;
		particles[x][y + 1].force += delta*diff*k[1];
		particles[x + 1][y].force -= delta*diff*k[1];
	}

	if (y < NUMBER_OF_VERTICES - 2) 
	{
		// ||||
		Vec3 delta = particles[x][y + 2].pos - particles[x][y].pos;
		double deltalength = delta.length();
		double diff = (deltalength - 2 * restlength) / deltalength;
		particles[x][y].force += delta*diff*k[2];
		particles[x][y + 2].force -= delta*diff*k[2];

	}

	if (x < NUMBER_OF_VERTICES - 2) 
	{
		// ----
		Vec3 delta = particles[x + 2][y].pos - particles[x][y].pos;
		double deltalength = delta.length();
		double diff = (deltalength - 2 * restlength) / deltalength;
		particles[x][y].force += delta*diff*k[2];
		particles[x + 2][y].force -= delta*diff*k[2];
	}
}

void Cloth::verlet(int x, int y, double step, double damping, Vec3 g) {
	//verlet
	//double mass = particles[x][y].mass;
	Vec3 p_curr{ particles[x][y].pos };
	//Vec3 p_old{  };
	particles[x][y].pos += (p_curr - particles[x][y].pos_old)*damping + (g + particles[x][y].force)*particles[x][y].mass*step*step; // 0.98 är dämningsfaktor eftersom p_curr - p_old är velocity, kan användas för att skapa känsla av tyngd. [0.9, 0.99] rekommenderat
	particles[x][y].pos_old = p_curr;
}

void Cloth::collision(int x, int y, vector<Sphere> *allSpheres) {
	// sfärkollision: 
	//for (Sphere &sphere : (*allSpheres))
	//{

		double rad{ allSpheres->at(0).getRadius() };
		Vec3 delta{ allSpheres->at(0).getPos() - (particles[x][y].pos + position) };
		double deltalength = delta.length();
		//cout << deltalength << ", " << delta.qlength() << endl;
		//cout << deltalength << " < " << rad*1.05 << endl;

		if (deltalength < (rad) * 1.05)
		{
			double diff{ (deltalength - rad * 1.05) / deltalength }; // generalisera margin
			particles[x][y].pos += delta * diff;
		}

	//}

	// markplanskollision: 
	particles[x][y].pos.y = max(particles[x][y].pos.y, 0.02 - position.y);
}

void Cloth::reset_forces() {
	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			particles[x][y].force = Vec3{ 0, 0, 0 };
		}
	}
}