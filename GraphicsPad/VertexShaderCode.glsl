#version 430


in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;

out vec3 theColor;
out vec3 vertexPositionWorld;
out vec3 normalWorld;
out vec3 diffuseLight;
out vec3 ambientLight;
out vec3 cameraVector;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 diffuseLightLocation;
uniform vec3 ambientLightValue;
uniform vec3 cameraPosition;

void main()
{
	//mat2 r=mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
	//gl_Position = vec4(position, 1.0);
	//gl_Position.xy = gl_Position.xy + velocity.xy;
	//gl_Position.xy = gl_Position.xy  * sin(theta);

	cameraVector =vec3( normalize(cameraPosition) - vec3(vertexPositionModel));

	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	normalWorld = vec3(modelToWorldMatrix * vec4(normalModel,0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);

	ambientLight = ambientLightValue;
	diffuseLight = diffuseLightLocation;
	theColor = vertexColor;
}