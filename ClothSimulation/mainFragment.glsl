#version 330 core

in vec3 interpolatedPosition;
in vec2 interpolatedTextureCoords; 
in vec3 interpolatedNormal;
in vec3 toLightVector; 
in vec3 toCameraVector; 

out vec4 pixel_color; 

uniform sampler2D mainTexture; 

const vec3 lightColor = vec3(1.0);
const float ambient = 0.15; 

void main (void) 
{
	vec4 textureColor = texture(mainTexture, interpolatedTextureCoords * 3);
	
	vec3 normal = normalize(interpolatedNormal); 
	
	vec3 unitToLightVector = normalize(toLightVector); 		
	float nDot1 = dot(normal, unitToLightVector); 
	float brightness = max(nDot1, 0.0); 			
	vec3 diffuse = (brightness * lightColor); 
	
	diffuse = max(diffuse, ambient); 	// ambient light
	diffuse = min(diffuse, 1.0);
	
	vec3 materialAndLighting = textureColor.xyz * diffuse; 
		
	pixel_color = vec4(materialAndLighting, 1);
}