#version 430

out vec4 daColor;

in vec3 vertexPositionWorld;
in vec3 normalWorld;
in vec3 theColor;
in vec3 ambientLight;

in vec3 diffuseLight;

void main()
{
	daColor = vec4(theColor, 1.0);
	vec3 directionVector = vertexPositionWorld - diffuseLight;
	daColor = vec4(theColor * max(dot( normalize(directionVector), normalize(normalWorld)),0) + ambientLight,1.0);
}