#version 330 core

in vec2 textureCoords; 

out vec4 pixelColor;

uniform sampler2D tex;    
uniform vec4 color; 

void main(void){

	if (texture(tex, textureCoords).r > 0.75) {
		pixelColor = color;
	} else {		
		pixelColor = vec4(0.0);
	}
	
	//pixelColor = texture(tex, textureCoords); 
	
}
