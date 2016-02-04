#include "sphere.h"

Sphere::Sphere(double x, double y, double z, double r, Loader loader)
{
	radius = r; 

	int indices[(V_NUMBER_OF_VERTICES - 1) * (H_NUMBER_OF_VERTICES - 1) * 6];
	float positions[V_NUMBER_OF_VERTICES * H_NUMBER_OF_VERTICES * 3];
	float normals[V_NUMBER_OF_VERTICES * H_NUMBER_OF_VERTICES * 3];
	int indicesIndex{ 0 }, positionIndex{ 0 }, normalIndex{ 0 };

	for (int p{ 0 }; p < H_NUMBER_OF_VERTICES; p++)
	{
		for (int a{ 0 }; a < V_NUMBER_OF_VERTICES; a++)
		{
			double phi = (double)(p * 3.1415 * 4) / (double)(H_NUMBER_OF_VERTICES);
			double alpha = (double)(a * 3.1415) / (double)(V_NUMBER_OF_VERTICES);
			
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
	for (int p{ 0 }; p < H_NUMBER_OF_VERTICES - 1; p++)
	{
		for (int a{ 0 }; a < V_NUMBER_OF_VERTICES - 1; a++)
		{
			topLeft = (a * V_NUMBER_OF_VERTICES) + p;
			topRight = topLeft + 1;
			bottomLeft = ((a + 1) * V_NUMBER_OF_VERTICES) + p;
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
	sphereModel.setPosition(x, y, z);

	GLuint groundTexture = loader.loadBMPtexture("testTexture.bmp");
	sphereModel.set_texture(groundTexture);

	sphereShader.createShader("sphereVertex.glsl", "sphereFragment.glsl");
}

void Sphere::render(GLFWwindow * window, Camera camera)
{
	float projectionMatrix[16];
	float viewMatrix[16];
	float modelMatrix[16];

	camera.getProjectionMatrix(projectionMatrix);
	camera.getViewMatrix(viewMatrix);
	sphereModel.getModelMatrix(modelMatrix);

	sphereShader.start();
	sphereShader.setUniformMat4("projectionMatrix", projectionMatrix);
	sphereShader.setUniformMat4("viewMatrix", viewMatrix);
	sphereShader.setUniformMat4("modelMatrix", modelMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(sphereModel.get_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereModel.get_texture());

	glDrawElements(GL_TRIANGLES, sphereModel.get_vertexcount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	sphereShader.stop();
}

void Sphere::cleanUp()
{

}
