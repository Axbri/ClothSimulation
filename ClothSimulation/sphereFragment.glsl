#version 330 core

in vec3 interpolatedPosition;
in vec2 interpolatedTextureCoords; 
in vec3 interpolatedNormal;
in vec3 toLightVector; 
in vec3 toCameraVector; 

out vec4 pixel_color; 

uniform sampler2D mainTexture; 

const vec3 lightColor = vec3(1.0);
const float shineDampener = 20.0; 
const float reflectance = 0.5; 
const float ambient = 0.15; 
	
void main (void) 
{
	vec4 textureColor = texture(mainTexture, interpolatedTextureCoords); 
	vec3 unitToCameraVector = normalize(toCameraVector);	
	vec3 totalDiffuse = vec3(0.0); 
	vec3 totalSpecular = vec3(0.0); 

	vec3 normal = normalize(interpolatedNormal); 
	
	// for loop for multiple lights goes here
	vec3 unitToLightVector = normalize(toLightVector); 		
	float nDot1 = dot(normal, unitToLightVector); 
	float brightness = max(nDot1, 0.0); 		
	vec3 lightDirection = -unitToLightVector; 	
	vec3 reflectedLightDirection = reflect(lightDirection, normal); 	
	float specularFactor = max(dot(unitToCameraVector, reflectedLightDirection), 0.0); 
	totalDiffuse = totalDiffuse + (brightness * lightColor); 
	totalSpecular = totalSpecular + pow(specularFactor, shineDampener) * lightColor * clamp(reflectance, 0.0, 1.0); 
	// end for loop
	
	totalDiffuse = max(totalDiffuse, ambient); 	// ambient light
	totalDiffuse = min(totalDiffuse, 1.0);
	totalSpecular = min(totalSpecular, 1.0);
	
	vec3 materialAndLighting = textureColor.xyz * totalDiffuse + totalSpecular; 
	
	
	float distance = length(interpolatedPosition); 
	
	float alpha = clamp(2-(distance / 20), 0.0, 1.0);
	
	pixel_color = vec4(materialAndLighting, alpha);
}