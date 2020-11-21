#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <MeGlWindow.h>
#include <glm\glm.hpp>
#include <Primitives\Vertex.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Primitives\ShapeGenerator.h>
#include <cstdlib>
#include<ctime>
using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
float theta = 0;
float scale = 1;

//float  velocity = rand() % 1000;


vec3 myDiamond[] =
{

glm::vec3(0.0 , 1.0 , 0.0),

glm::vec3(1.0 ,0.0 , 0.0),

glm::vec3(0.0 , -1.0 , 0.0),

glm::vec3(-1.0 , 0.0 , 0.0),


};

vec3 position(0.0,0.0,0.0);
vec3 velocity;
GLuint numIndices;


void sendDataToOpenGL()
{
	ShapeData tri = ShapeGenerator::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufferSize(), tri.indices, GL_STATIC_DRAW);
	numIndices = tri.numIndices;
	tri.cleanup();

}


void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	GLint dominatingColorUniformLocation = 
		glGetUniformLocation(programID, "dominatingColor");
	//GLint yFlipUniformLocation =
		//glGetUniformLocation(programID, "yFlip");
	GLuint fullTransformationUniformLocation;
	GLuint modelToWorldMatrixUniformLocation;
	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");

	GLuint diffuseLightLocation = glGetUniformLocation(programID, "diffuseLightLocation");
	GLuint ambientLightValue = glGetUniformLocation(programID, "ambientLightValue");
	GLuint cameraPosition = glGetUniformLocation(programID, "cameraPosition");
	//GLint velocityValue =
		//glGetUniformLocation(programID, "velocity");
	//GLint velocityValue =
		//glGetUniformLocation(programID, "velocity");

	mat4 rotator = glm::rotate(theta,-3.0f,20.0f,0.0f);
	mat4 cubeModelToWorldMatrix = glm::translate(0.0f, 0.0f, -7.0f);
	mat4 smooshItMatrix = glm::perspective(60.0f, width() / (float)height(), 0.01f, 30.0f);
	mat4 shippingNormalStuff = cubeModelToWorldMatrix * rotator;
	mat4 finalMatrix = smooshItMatrix * cubeModelToWorldMatrix * rotator;

	vec3 diffuseLight(10.0f, -10.0f, -20.0f);
	vec3 ambientLight(0.2f, 0.f, 0.2f);
	vec3 cameraPos(-10.0f, 0.0f, 0.0f);

	glUniform3fv(cameraPosition, 1, &cameraPos[0]);
	glUniform3fv(diffuseLightLocation, 1, &diffuseLight[0]);
	glUniform3fv(ambientLightValue, 1, &ambientLight[0]);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&shippingNormalStuff[0][0]);
	//vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	//glUniform3fv(velocityValue, 1, &position[0]);
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, 1.0f);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	//glDrawArrays(GL_LINES, 0, 4);
	//vec3 zero;
	//glUniform3fv(velocityValue, 1, &zero[0]);

	//glBegin(GL_LINES);

	//for (uint i = 0; i < 4; i++)
	//{
		//glVertex2f(myDiamond[i].x, myDiamond[i].y);
		//glVertex2f(myDiamond[ (i + 1) % 4 ].x, myDiamond[(i + 1) % 4].y);
	//}
	//glEnd();

	//dominatingColor.r = 0;
	//dominatingColor.b = 1;
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, -1.0f);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if ( ! meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShaderID);

		// Use the infoLog as you see fit.
		std::cout << (char*)infoLog.data();

		// In this simple program, we'll just leave
		return;
	}
	if( ! checkShaderStatus(vertexShaderID) || ! checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if ( ! checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();

	//srand(time(0));

	//velocity.x = rand() / (float)RAND_MAX * 0.1;
	//velocity.y = rand() / (float)RAND_MAX *0.1;

	//velocity.x = 0.1;
	//velocity.y = 0.2;

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(10);

}


void MeGlWindow::myUpdate()
{
	theta += 1.0f;
	
	/* position = position + velocity;

	vec3 normal;
	for (uint i = 0; i < 4; i++ )
	{

		vec3 line = myDiamond[(i + 1)%4] - myDiamond[i];

		normal.x = line.y;
		normal.y = -line .x;

		vec3 result = position - myDiamond[i];

		float dotProduct = glm::dot(result, normal);
		
		if (dotProduct < 0)
		{
			velocity.x = 0.0;
			velocity.y = 0.0;
		}

	} */

	repaint();
	

}