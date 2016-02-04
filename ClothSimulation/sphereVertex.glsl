#version 330 core

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal; 

out vec3 interpolatedPosition;
out vec3 interpolatedNormal;
out vec3 toLightVector; 
out vec3 toCameraVector; 

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const vec3 lightPosition = vec3(200, 500, 200);

void main (void) 
{
	vec4 worldPosition = modelMatrix * vec4(position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;	
	
	interpolatedPosition = worldPosition.xyz; 
		
	interpolatedNormal = mat3(modelMatrix) * normal;  	
	
	toLightVector = lightPosition - vec3(worldPosition); 
	toCameraVector = vec3(inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)) - vec3(worldPosition);
}