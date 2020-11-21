#pragma once
#include <Primitives\ShapeData.h>
typedef unsigned int uint;

class ShapeGenerator
{
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makePlane(uint dimensions = 10);
	//static ShapeData makeTeapot(uint tesselation = 10, const glm::mat4& lidTransform = glm::mat4());
	static ShapeData makeSphere(uint tesselation = 20);
	static ShapeData makeTorus(uint tesselation = 20);
};

