#include "light.h"

Light::Light()
{
	color = Vec3(1, 1, 1);
	attenuation = Vec3(1, 0, 0);
}

Light::Light(double x, double y, double z)
{
	pos = Vec3(x, y, z);
	color = Vec3(1, 1, 1);
	attenuation = Vec3(1, 0, 0);
}

void Light::loadLightsToShader(Shader shader, vector<Light> allLights)
{
	int numberOfLights = allLights.size(); 
	ostringstream stream;

	for (int i{ 0 }; i < 8; i++)	// max 8 lights are loaded to the shaderprogram
	{
		char lightPosLoc[18];	
		char lightColorLoc[15];
		char lightAttLoc[21];

		stream << "lightPosition[" << i << "]";
		strcpy_s(lightPosLoc, stream.str().c_str());
		stream.clear();
		stream.str("");

		stream << "lightColor[" << i << "]";
		strcpy_s(lightColorLoc, stream.str().c_str());
		stream.clear();
		stream.str("");

		stream << "lightAttenuation[" << i << "]";
		strcpy_s(lightAttLoc, stream.str().c_str());
		stream.clear();
		stream.str("");
		
		if (i < numberOfLights)		// load light data for the given light. 
		{
			Light light = allLights[i]; 
			shader.setUniformVec3(lightPosLoc, light.pos);
			shader.setUniformVec3(lightColorLoc, light.color);
			shader.setUniformVec3(lightAttLoc, light.attenuation);
		}
		else  // if no more lights exist in the list, load a dummy-light to the shader with britness = 0; 
		{
			shader.setUniformVec3(lightPosLoc, 0, 0, 0);
			shader.setUniformVec3(lightColorLoc, 0, 0, 0);
			shader.setUniformVec3(lightAttLoc, 1, 0, 0);
		}
	}
}

