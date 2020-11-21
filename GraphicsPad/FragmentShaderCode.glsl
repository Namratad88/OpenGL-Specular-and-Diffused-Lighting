#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec3 normalWorld;
in vec3 theColor;
in vec3 ambientLight;
in vec3 cameraVector;

in vec3 diffuseLight;

void main()
{
	daColor = vec4(theColor, 1.0);
	vec3 directionVector = vertexPositionWorld - diffuseLight;
	vec3 lightVectorWorld = normalize(diffuseLight - vertexPositionWorld);
	vec3 reflectedLightVector = reflect(-1 *lightVectorWorld, normalize(normalWorld));
	float specularBrightness = dot(cameraVector, reflectedLightVector);
	specularBrightness = pow(specularBrightness,2);
	
	daColor = vec4(theColor * max(dot( normalize(directionVector), normalize(normalWorld)),0) + ambientLight + max(specularBrightness,0),1.0);
}