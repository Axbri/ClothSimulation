# version 330 core

layout(location=0) in vec3 position;

out vec3 interpolatedTextureCoordinates;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() { 		
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);		
	
	interpolatedTextureCoordinates = position * vec3(1, -1, 1);   	
}


