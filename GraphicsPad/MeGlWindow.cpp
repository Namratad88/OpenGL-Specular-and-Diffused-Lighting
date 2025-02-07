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
#include<Camera.h>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>

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

GLuint cubeNumIndices;
GLuint arrowNumIndices;
GLuint planeNumIndices;
GLuint sphereNumIndices;
GLuint torusNumIndices;

GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArrayObjectID;
GLuint sphereVertexArrayObjectID;
GLuint torusVertexArrayObjectID;

GLuint cubeIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint planeIndexByteOffset;
GLuint sphereIndexByteOffset;
GLuint torusIndexByteOffset;
Camera camera;

const char* texName = "brick2.png";
QImage timg = QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
const char* texName1 = "red.png";
QImage timg1 = QGLWidget::convertToGLFormat(QImage(texName1, "PNG"));

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
GLuint theBufferID;
GLuint passThroughProgramID;


void sendDataToOpenGL()
{
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane();
	
	ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData torus = ShapeGenerator::makeTorus();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		cube.vertexBufferSize() + cube.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		torus.vertexBufferSize() + torus.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	arrowIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	sphereIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.vertexBufferSize(), torus.vertices);
	currentOffset += torus.vertexBufferSize();
	torusIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.indexBufferSize(), torus.indices);
	currentOffset += torus.indexBufferSize();

	cubeNumIndices = cube.numIndices;
	arrowNumIndices = arrow.numIndices;
	planeNumIndices = plane.numIndices;
	sphereNumIndices = sphere.numIndices;
	torusNumIndices = torus.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &torusVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);


	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint sphereByteOffset = planeByteOffset + plane.vertexBufferSize() + plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)sphereByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint torusByteOffset = sphereByteOffset + sphere.vertexBufferSize() + sphere.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)torusByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);


	glActiveTexture(GL_TEXTURE0);
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



	glActiveTexture(GL_TEXTURE1);
	GLuint tid1;
	glGenTextures(1, &tid1); 
	glBindTexture(GL_TEXTURE_2D, tid1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg1.width(), timg1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg1.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	

	cube.cleanup();
	arrow.cleanup();
	plane.cleanup();
	sphere.cleanup();
	torus.cleanup();

}


void MeGlWindow::paintGL()
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	int loc = glGetUniformLocation(programID, "myTexture");
	

	glm::vec3 lightPositionWorld (2.0f, 5.0f, 12.0f);

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLuint fullTransformationUniformLocation;
	GLuint modelToWorldMatrixUniformLocation;

	glUseProgram(programID);
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.05f, 0.05f, 0.05f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	GLint eyePositionWorldUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	glm::vec3 eyePosition = camera.getPosition();
	glUniform3fv(eyePositionWorldUniformLocation, 1, &eyePosition[0]);
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);

	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeModelToWorldMatrix = glm::translate(3.0f, 0.0f, 3.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniform1i(loc, 0);
	//else fprintf(stderr, "\n");
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	cubeModelToWorldMatrix = glm::translate(-5.0f, 0.0f, -5.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniform1i(loc, 1);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	//plane 

	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix;
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	// Sphere
	glBindVertexArray(sphereVertexArrayObjectID);
	mat4 sphereModelToWorldMatrix = glm::translate(3.0f, 3.0f, 3.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * sphereModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&sphereModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);

/*	glBindVertexArray(cubeVertexArrayObjectID);
	cubeModelToWorldMatrix =
		glm::translate(lightPositionWorld) *
		glm::scale(0.1f, 0.1f, 0.1f);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUseProgram(passThroughProgramID);
	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);
	*/
/*
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
	//vec3 cameraPos(-10.0f, 0.0f, 0.0f);

	//glUniform3fv(cameraPosition, 1, &cameraPos[0]);
	glUniform3fv(diffuseLightLocation, 1, &diffuseLight[0]);
	glUniform3fv(ambientLightValue, 1, &ambientLight[0]);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&shippingNormalStuff[0][0]);
	//vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	//glUniform3fv(velocityValue, 1, &position[0]);
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, 1.0f);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0); */

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

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	setFocus();
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
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
	// Load texture file 
	
	sendDataToOpenGL();
	installShaders();
/*
	const char * texName = "tiles.jpg";
	QImage timg;
	try{
		QImage timg = QGLWidget::convertToGLFormat(QImage(texName, "JPG"));
		}
	catch(const std::exception& e){
		std::cout << e.what();
		QImage timg = QImage(texName, "JPG");
		}
	// Copy file to OpenGL
	glActiveTexture(GL_TEXTURE0);
	GLuint tid;

	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Set the Tex1 sampler uniform to refer to texture unit 0
	int loc = glGetUniformLocation(programID, "myTexture");
	if (loc >= 0) glUniform1i(loc, 0);
	else fprintf(stderr, "Uniform variable Tex1 not found!\n");*/

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