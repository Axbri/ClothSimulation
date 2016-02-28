# version 330 core

out vec4 pixelColor;

in vec3 interpolatedTextureCoordinates;

uniform samplerCube cubeMap1;

void main (){	
	vec3 texture1Color = texture(cubeMap1, interpolatedTextureCoordinates).rgb; 
	
	pixelColor = vec4(texture1Color, 1.0);
}

