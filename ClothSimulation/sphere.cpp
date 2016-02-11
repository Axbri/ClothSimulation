#include "sphere.h"

Sphere::Sphere(double x, double y, double z, double r, Loader loader)
{
	radius = r;
	position = Vec3{ x, y, z }; 

	int indices[(NUMBER_OF_SEGMENTS - 1) * (NUMBER_OF_SEGMENTS - 1) * 6];
	float positions[(NUMBER_OF_SEGMENTS) * (NUMBER_OF_SEGMENTS) * 3];
	float normals[(NUMBER_OF_SEGMENTS) * (NUMBER_OF_SEGMENTS) * 3];
	int indicesIndex{ 0 }, positionIndex{ 0 }, normalIndex{ 0 };

	for (int p{ 0 }; p < NUMBER_OF_SEGMENTS; p++)
	{
		for (int a{ 0 }; a < NUMBER_OF_SEGMENTS; a++)
		{
			double phi = (double)(p * 3.1415 * 2) / (double)(NUMBER_OF_SEGMENTS -1);
			double alpha = -3.1415 + (double)(a * 3.1415) / (double)(NUMBER_OF_SEGMENTS -1);
			
			Vec3 pos{ radius * cos(phi) * sin(alpha) , radius * cos(alpha) , radius * sin(phi) * sin(alpha) };
			positions[positionIndex++] = pos.x;
			positions[positionIndex++] = pos.y;
			positions[positionIndex++] = pos.z;

			Vec3 normal{ pos };
			normal.normalize();
			normals[normalIndex++] = normal.x;
			normals[normalIndex++] = normal.y;
			normals[normalIndex++] = normal.z;
		}
	}

	int topLeft{ 0 }, topRight{ 0 }, bottomLeft{ 0 }, bottomRight{ 0 };

	
	for (int p{ 0 }; p < NUMBER_OF_SEGMENTS -1; p++)
	{
		for (int a{ 0 }; a < NUMBER_OF_SEGMENTS -1; a++)
		{
			topLeft = (a * NUMBER_OF_SEGMENTS) + p;
			topRight = topLeft + 1;
			bottomLeft = ((a + 1) * NUMBER_OF_SEGMENTS) + p;
			bottomRight = bottomLeft + 1;
			indices[indicesIndex++] = topLeft;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = bottomRight;
		}
	}
	sphereModel = loader.createTexturelessModel(positions, positionIndex, normals, normalIndex, indices, indicesIndex);	
	sphereShader.createShader("sphereVertex.glsl", "sphereFragment.glsl");
	updateModelMatrix(); 
}

void Sphere::updateModelMatrix()
{
	Mat4 translation;
	translation.loadTranslation(position.x, position.y, position.z);
	sphereModel.setModelMatrix(translation);
}

void Sphere::render(GLFWwindow * window, Camera camera, vector<Light> allLights)
{
	sphereShader.start();
	sphereShader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	sphereShader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	sphereShader.setUniformMat4("modelMatrix", sphereModel.getModelMatrix());
	Light::loadLightsToShader(sphereShader, allLights);

	glBindVertexArray(sphereModel.get_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glDrawElements(GL_TRIANGLES, sphereModel.get_vertexcount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	sphereShader.stop();
}

void Sphere::cleanUp()
{
	sphereShader.cleanUp(); 
}

void Sphere::setPos(Vec3 pos)
{	
	position = pos;
	updateModelMatrix(); 
}

Vec3 Sphere::getPos() 
{
	return position;
}

double Sphere::getRadius()
{
	return radius;
}
