#version 330 core

in vec2 interpolatedTextureCoords; 

out vec4 pixel_color; 

uniform sampler2D mainTexture; 

void main (void) 
{
	pixel_color = texture(mainTexture, interpolatedTextureCoords * 3); 
}