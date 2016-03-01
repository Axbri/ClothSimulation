#include "Skybox.h"

Skybox::Skybox(Loader loader)
{
	float SIZE = 500;
	float VERTICES[] = { 
		-SIZE, SIZE, -SIZE, -SIZE, -SIZE, -SIZE, SIZE, -SIZE, -SIZE, 
		SIZE, -SIZE, -SIZE,	SIZE, SIZE, -SIZE, -SIZE, SIZE, -SIZE,

		-SIZE, -SIZE, SIZE, -SIZE, -SIZE, -SIZE, -SIZE, SIZE, -SIZE, 
		-SIZE, SIZE, -SIZE, -SIZE, SIZE, SIZE, -SIZE, -SIZE, SIZE, 
		
		SIZE, -SIZE, -SIZE, SIZE, -SIZE, SIZE, SIZE, SIZE, SIZE, 
		SIZE, SIZE,	SIZE, SIZE, SIZE, -SIZE, SIZE, -SIZE, -SIZE, 
		
		-SIZE, -SIZE, SIZE, -SIZE, SIZE, SIZE, SIZE, SIZE, SIZE, 
		SIZE, SIZE,	SIZE, SIZE, -SIZE, SIZE, -SIZE, -SIZE, SIZE, 

		-SIZE, SIZE, -SIZE, SIZE, SIZE, -SIZE, SIZE, SIZE, SIZE, 
		SIZE, SIZE,	SIZE, -SIZE, SIZE, SIZE, -SIZE, SIZE, -SIZE, 

		-SIZE, -SIZE, -SIZE, -SIZE, -SIZE, SIZE, SIZE, -SIZE, -SIZE, 
		SIZE, -SIZE, -SIZE, -SIZE, -SIZE, SIZE, SIZE, -SIZE, SIZE };

	model = loader.createModel(VERTICES, 2*9*6);

	shader.createShader("skyVertexShader.glsl", "skyFragmentShader.glsl");

	/*GLuint texture = loader.loadBMPcubeTexture(
		"skyPosX.bmp", "skyNegX.bmp",
		"skyPosY.bmp", "skyNegY.bmp", 
		"skyPosZ.bmp", "skyNegZ.bmp");*/

	texture = loader.loadBMPcubeTexture(
		"miramar_rt.bmp", "miramar_lf.bmp", 
		"miramar_dn.bmp", "miramar_up.bmp", 
		"miramar_bk.bmp", "miramar_ft.bmp");

	modelMatrix.loadTranslation(0, 0, 0);
}


Skybox::~Skybox()
{
	cleanUp(); 
}

void Skybox::render(GLFWwindow * window, Camera camera)
{
	modelMatrix.loadTranslation(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	glDisable(GL_CULL_FACE);

	shader.start(); 
	shader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	shader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	shader.setUniformMat4("modelMatrix", modelMatrix);

	shader.setUniformInt("cubeMap1", 0);

	glBindVertexArray(model);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glDrawArrays(GL_TRIANGLES, 0, 4 * 9);	// varför 4*9 ??

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader.stop();

	glEnable(GL_CULL_FACE);
}

void Skybox::cleanUp()
{
	shader.cleanUp(); 
}
