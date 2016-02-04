#include "sphere.h"

Sphere::Sphere(double x, double y, double z, int numberOfSegments, Loader loader)
{
	sphereShader.createShader("sphereVertex.glsl", "sphereFragment.glsl");

	float groundPos[] = {
		-1.0f,  0.0f,  1.0f,
		-1.0f,  0.0f, -1.0f,
		1.0f,  0.0f, -1.0f,
		1.0f,  0.0f,  1.0f
	};
	
	float normals[] = {
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};

	int indices[] = {
		0, 1, 3, 3, 1, 2
	};

	sphereModel = loader.createTexturelessModel(groundPos, 12, normals, 12, indices, 6);
	sphereModel.setPosition(x, y, z);

	GLuint groundTexture = loader.loadBMPtexture("testTexture.bmp");
	sphereModel.set_texture(groundTexture);
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
