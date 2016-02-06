#include "light.h"

Light::Light()
{
	color.set(1, 1, 1); 
	attenuation.set(1, 0, 0); 
}

Light::Light(double x, double y, double z)
{
	pos.set(x, y, z); 
	color.set(1, 1, 1);
	attenuation.set(1, 0, 0);
}

void Light::loadLightsToShader(Shader shader, vector<Light> allLights)
{
	int numberOfLights = allLights.size(); 

	for (int i{ 0 }; i < 8; i++)	// max 8 lights are loaded to the shaderprogram
	{
		char lightPosLoc[128];	
		char lightColorLoc[128];
		char lightAttLoc[128];

		ostringstream stream1; 
		ostringstream stream2;
		ostringstream stream3;

		stream1 << "lightPosition[" << i << "]";
		strcpy_s(lightPosLoc, stream1.str().c_str());
		stream1.clear();

		stream2 << "lightColor[" << i << "]";
		strcpy_s(lightColorLoc, stream2.str().c_str());
		stream2.clear();

		stream3 << "lightAttenuation[" << i << "]";
		strcpy_s(lightAttLoc, stream3.str().c_str());
		stream3.clear();
		
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

