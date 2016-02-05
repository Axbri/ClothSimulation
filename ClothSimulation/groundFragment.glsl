#version 330 core

in vec3 interpolatedPosition;
in vec2 interpolatedTextureCoords; 
in vec3 interpolatedNormal;
in vec3 toLightVector[8]; 
in vec3 toCameraVector; 

out vec4 pixel_color; 

uniform sampler2D mainTexture; 
uniform sampler2D normalTexture; 
uniform vec3 lightColor[8]; 
uniform vec3 lightAttenuation[8];

const float shineDampener = 20.0; 
const float reflectance = 0.5; 
const float ambient = 0.15; 
	
void main (void) 
{
	vec4 textureColor = texture(mainTexture, interpolatedTextureCoords * 100); 
	
	vec3 normal = normalize((texture(normalTexture, interpolatedTextureCoords * 100).xzy * 2.0) - 1.0); 
	normal.z = -normal.z; 
	
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
	
	vec3 materialAndLighting = textureColor.xyz * totalDiffuse + totalSpecular; 
	
	
	float distance = length(interpolatedPosition); 
	
	float alpha = clamp(2-(distance / 20), 0.0, 1.0);
	
	pixel_color = vec4(materialAndLighting, alpha);
}