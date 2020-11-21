#pragma once
#include <Primitives\ShapeData.h>
typedef unsigned int uint;

class ShapeGenerator
{
	static ShapeData makePlaneVerts(uint dimensions);
	static ShapeData makePlaneIndices(uint dimensions);
	static ShapeData makePlaneUnseamedIndices(uint tesselation);

	// Teapot helpers
	
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makeArrow();
	static ShapeData makePlane(uint dimensions = 10);
	//static ShapeData makeTeapot(uint tesselation = 10, const glm::mat4& lidTransform = glm::mat4());
	static ShapeData makeSphere(uint tesselation = 20);
	static ShapeData makeTorus(uint tesselation = 20);
	static ShapeData generateNormals(const ShapeData& data);
};

