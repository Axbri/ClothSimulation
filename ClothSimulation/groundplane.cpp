#include "groundplane.h"

GroundPlane::GroundPlane(Loader loader)
{
	groundShader.createShader("groundVertex.glsl", "groundFragment.glsl");
	
	float groundPos[] = {
		-1.0f,  0.0f,  1.0f,
		-1.0f,  0.0f, -1.0f,
		1.0f,  0.0f, -1.0f,
		1.0f,  0.0f,  1.0f
	};

	float textureCoords[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
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

	// create a ground-plane that is 2x2 meters and scale it so it becomes 100x100 meters. 
	groundModel = loader.createModel(groundPos, 12, textureCoords, 8, normals, 12, indices, 6);
	groundModel.setScale(50, 1, 50);

	// load texture attach it to the ground plane. 	
	GLuint groundTexture = loader.loadBMPtexture("groundTexture.bmp");
	groundModel.set_texture(groundTexture);

	normalMapTexture = loader.loadBMPtexture("tile_normal.bmp"); 
}

void GroundPlane::render(GLFWwindow * window, Camera camera)
{
	float projectionMatrix[16];
	float viewMatrix[16];
	float modelMatrix[16];

	camera.getProjectionMatrix(projectionMatrix);
	camera.getViewMatrix(viewMatrix);
	groundModel.getModelMatrix(modelMatrix);

	groundShader.start();
	groundShader.setUniformInt("mainTexture", 0);
	groundShader.setUniformInt("normalTexture", 1);
	groundShader.setUniformMat4("projectionMatrix", projectionMatrix);
	groundShader.setUniformMat4("viewMatrix", viewMatrix);
	groundShader.setUniformMat4("modelMatrix", modelMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(groundModel.get_id());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundModel.get_texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture);

	glDrawElements(GL_TRIANGLES, groundModel.get_vertexcount(), GL_UNSIGNED_INT, 0);
	

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	groundShader.stop();
}

void GroundPlane::cleanUp()
{
	groundShader.cleanUp(); 
}
