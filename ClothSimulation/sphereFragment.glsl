#version 330 core

in vec3 interpolatedNormal;
in vec3 toLightVector[8]; 
in vec3 toCameraVector; 

out vec4 pixel_color; 

uniform vec3 lightColor[8]; 
uniform vec3 lightAttenuation[8];

const float shineDampener = 20.0; 
const float reflectance = 0.5; 
const float ambient = 0.15; 
const vec3 color = vec3(0.9, 0.4, 0.2); 
	
void main (void) 
{
	vec3 normal = normalize(interpolatedNormal); 	
	vec3 unitToCameraVector = normalize(toCameraVector);	
	vec3 totalDiffuse = vec3(0.0); 
	vec3 totalSpecular = vec3(0.0); 
	
	for (int i = 0; i < 8; i++) {
		float distanceToLight = length(toLightVector[i]); 
		float attenuationFactor = lightAttenuation[i].x + (lightAttenuation[i].y * distanceToLight) + (lightAttenuation[i].z * distanceToLight * distanceToLight);
		vec3 unitToLightVector = normalize(toLightVector[i]); 		
		float nDot1 = dot(normal, unitToLightVector); 
		float brightness = max(nDot1, 0.0); 		
		vec3 lightDirection = -unitToLightVector; 	
		vec3 reclectedLightDirection = reflect(lightDirection, normal); 	
		float specularFactor = max(dot(unitToCameraVector, reclectedLightDirection), 0.0); 
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attenuationFactor; 
		totalSpecular = totalSpecular + (pow(specularFactor, shineDampener) * lightColor[i] * clamp(reflectance, 0.0, 1.0)) / attenuationFactor; 
	}
		
	totalDiffuse = max(totalDiffuse, ambient); 	// ambient light
	totalDiffuse = min(totalDiffuse, 1.0);
	totalSpecular = min(totalSpecular, 1.0);
	
	vec3 materialAndLighting = color * totalDiffuse + totalSpecular; 
	
	pixel_color = vec4(materialAndLighting, 1.0);
}