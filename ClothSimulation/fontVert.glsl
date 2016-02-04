#version 330 core

layout (location = 0) in vec2 position;

out vec2 textureCoords;

uniform mat4 transformationMatrix;
uniform int character;

void main(void) {
	gl_Position = transformationMatrix * vec4(position, 1.0, 1.0);	
	
	vec2 tempTextureCoords = vec2((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0);  
		
	// calculate the character possition in a 16x16 ascii font texture based on the uniform char-variable	
	int xIndex = character % 16; 
	int yIndex = (character / 16); 
	
	yIndex = 15 - yIndex; 
	
	tempTextureCoords = tempTextureCoords / 16.0; 
	tempTextureCoords = tempTextureCoords + vec2(xIndex * (1.0/16.0), yIndex * (1.0/16.0)); 
	
	textureCoords = tempTextureCoords; 
}
