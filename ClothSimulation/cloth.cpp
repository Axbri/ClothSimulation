#include "cloth.h"

Cloth::Cloth(Loader loader, Vec3 pos, double size, double totalWeight)
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
			double particleZpos = -((double)y / (double)(NUMBER_OF_VERTICES - 1)) * size;
			double particleYpos = 0;// 0.05 * (sin(x) + sin(y));
			// jag bytta platts på z och y här ovan för att få tyget att generar i x-z planet istället
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
			textureCoords[textureCoordIndex++] = particles[x][y].textureCoord.x;
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

	position = pos;	// position of the cloth in the world. 

	clothModel = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, normals, normalIndex, indices, indicesIndex);	
	

	GLuint clothTexture = loader.loadBMPtexture("textil01.bmp");
	clothModel.set_texture(clothTexture);
}

Cloth::~Cloth()
{
	cleanUp(); 
}

// update the cloth, this function will handle the phisics simulation of the cloth. 
void Cloth::update(double delta_time, double time, Sphere sphere)
{

	double step = 0.016; // 60 uppdateringar per sekund
	bool use_forces = true;
	double k[3] = { 10000, 10000, 10000 };
	time_passed += delta_time;
	if (time_passed > step)
	{
		time_passed -= step;
		reset_forces();
		// update the particle's positions
		for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
		{
			for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
			{
				if (!use_forces) {
					resolve_constraint(x, y);
					verlet_constraints(x, y, step, 0.98);
					collision(x, y, sphere);
				} else {
					calculate_force(x, y, k);
					verlet_forces(x, y, step, 25);
					collision(x, y, sphere);
				}
				
			}
		}
	}

	for (int i{ 0 }; i < (NUMBER_OF_VERTICES); i++)
	{
		if (i % 8 == 0 || i % 8 == 1)
		{
			particles[i][0].pos = Vec3(((double)i / (double)(NUMBER_OF_VERTICES - 1)) * size, 0, 0);
		}			
	}

	//particles[0][0].pos = Vec3(0+0.1, 0, 0);
	//particles[int((NUMBER_OF_VERTICES - 1) / 4)][0].pos = Vec3(size / 4, 0, 0);
	//particles[int((NUMBER_OF_VERTICES - 1) / 2)][0].pos = Vec3(size / 2, 0, 0);
	//particles[int((3 * NUMBER_OF_VERTICES - 1) / 4)][0].pos = Vec3(3 * size / 4, 0, 0);
	//particles[NUMBER_OF_VERTICES - 1][0].pos = Vec3(size-0.1, 0, 0);

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
	// create the new model-arrays: 
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
			normal = Vec3(0, 0, 0);
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
	for (int r{ 0 }; r < 2; r++)
	{
		if (y < NUMBER_OF_VERTICES - 1) {
			// ||||
			Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - restlength) / deltalength;
			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x][y + 1].pos -= delta*0.5*diff*con_inf[0];

		}
		if (x < NUMBER_OF_VERTICES - 1) {
			// ----
			Vec3 delta = particles[x + 1][y].pos - particles[x][y].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - restlength) / deltalength;

			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x + 1][y].pos -= delta*0.5*diff*con_inf[0];

		}
		if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) {
			// cross forward

			Vec3 delta = particles[x + 1][y + 1].pos - particles[x][y].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - 1.4142*restlength) / deltalength;

			particles[x][y].pos += delta*0.5*diff*con_inf[1];
			particles[x + 1][y + 1].pos -= delta*0.5*diff*con_inf[1];

		}
		if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) {
			// cross back
			Vec3 delta = particles[x + 1][y].pos - particles[x][y + 1].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - 1.4142*restlength) / deltalength;

			particles[x][y + 1].pos += delta*0.5*diff*con_inf[1];
			particles[x + 1][y].pos -= delta*0.5*diff*con_inf[1];

		}
		if (y < NUMBER_OF_VERTICES - 2) {
			// ||||
			Vec3 delta = particles[x][y + 2].pos - particles[x][y].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - 2 * restlength) / deltalength;

			particles[x][y].pos += delta*0.5*diff*con_inf[2];
			particles[x][y + 2].pos -= delta*0.5*diff*con_inf[2];

		}
		if (x < NUMBER_OF_VERTICES - 2) {
			// ----
			Vec3 delta = particles[x + 2][y].pos - particles[x][y].pos;
			double deltalength = sqrt(delta*delta);
			double diff = (deltalength - 2 * restlength) / deltalength;

			particles[x][y].pos += delta*0.5*diff*con_inf[0];
			particles[x + 2][y].pos -= delta*0.5*diff*con_inf[0];
		}
	}
}

void Cloth::calculate_force(int x, int y, double k[]) {

	if (y < NUMBER_OF_VERTICES - 1) {
		// ||||
		Vec3 delta = particles[x][y + 1].pos - particles[x][y].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - restlength) / deltalength;

		particles[x][y].force += delta*0.5*diff*k[0];
		particles[x][y + 1].force -= delta*0.5*diff*k[0];

	}
	if (x < NUMBER_OF_VERTICES - 1) {
		// ----
		Vec3 delta = particles[x + 1][y].pos - particles[x][y].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - restlength) / deltalength;

		particles[x][y].force += delta*0.5*diff*k[0];
		particles[x + 1][y].force -= delta*0.5*diff*k[0];

	}
	if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) {
		// cross forward

		Vec3 delta = particles[x + 1][y + 1].pos - particles[x][y].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - 1.4142*restlength) / deltalength;

		particles[x][y].force += delta*0.5*diff*k[1];
		particles[x + 1][y + 1].force -= delta*0.5*diff*k[1];

	}
	if (x < NUMBER_OF_VERTICES - 1 && y < NUMBER_OF_VERTICES - 1) {
		// cross back
		Vec3 delta = particles[x + 1][y].pos - particles[x][y + 1].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - 1.4142*restlength) / deltalength;

		particles[x][y + 1].force += delta*0.5*diff*k[1];
		particles[x + 1][y].force -= delta*0.5*diff*k[1];

	}
	if (y < NUMBER_OF_VERTICES - 2) {
		// ||||
		Vec3 delta = particles[x][y + 2].pos - particles[x][y].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - 2 * restlength) / deltalength;

		particles[x][y].force += delta*0.5*diff*k[2];
		particles[x][y + 2].force -= delta*0.5*diff*k[2];

	}
	if (x < NUMBER_OF_VERTICES - 2) {
		// ----
		Vec3 delta = particles[x + 2][y].pos - particles[x][y].pos;
		double deltalength = sqrt(delta*delta);
		double diff = (deltalength - 2 * restlength) / deltalength;

		particles[x][y].force += delta*0.5*diff*k[2];
		particles[x + 2][y].force -= delta*0.5*diff*k[2];
	}
}

void Cloth::verlet_constraints(int x, int y, double step, double damping) {
	//verlet
	Vec3 g = Vec3(0, -9.81, 0);
	double mass = particles[x][y].mass;
	Vec3 p_curr = Vec3(particles[x][y].pos);
	Vec3 p_old = Vec3(particles[x][y].pos_old);

	particles[x][y].pos += (p_curr - p_old)*damping + g*mass*step*step; // 0.98 är dämningsfaktor eftersom p_curr - p_old är velocity, kan användas för att skapa känsla av tyngd. [0.9, 0.99] rekommenderat
	particles[x][y].pos_old = p_curr;
}

void Cloth::verlet_forces(int x, int y, double step, double damping) {
	//verlet
	Vec3 g = Vec3(0, -9.81, 0);
	double mass = particles[x][y].mass;
	Vec3 p_curr = Vec3(particles[x][y].pos);
	Vec3 p_old = Vec3(particles[x][y].pos_old);

	Vec3 v = (p_curr - p_old)*(1 / step);
	Vec3 Fd = v*(-1)*damping;

	particles[x][y].pos += p_curr - p_old + (g + particles[x][y].force + Fd)*mass*step*step; // 0.98 är dämningsfaktor eftersom p_curr - p_old är velocity, kan användas för att skapa känsla av tyngd. [0.9, 0.99] rekommenderat
	particles[x][y].pos_old = p_curr;
}

void Cloth::collision(int x, int y, Sphere sphere) {
	// sfärkollision: 
	Vec3 pos = sphere.getPos();
	double rad = sphere.getRadius();
	Vec3 delta = pos - (particles[x][y].pos + position);
	double deltalength = sqrt(delta*delta);
	if (deltalength < rad*1.05)
	{
		double diff = (deltalength - rad*1.05) / deltalength; // generalisera margin
		particles[x][y].pos += delta*diff;
	}

	// markplanskollision: 
	particles[x][y].pos.y = max(particles[x][y].pos.y, 0.02 - position.y);
}

void Cloth::reset_forces() {
	for (int x{ 0 }; x < NUMBER_OF_VERTICES; x++)
	{
		for (int y{ 0 }; y < NUMBER_OF_VERTICES; y++)
		{
			particles[x][y].force = { 0, 0, 0 };
		}
	}
}