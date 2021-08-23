#ifndef CYLINDER_H
#define CYLINDER_H

// GLM headers 
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Cylinder
{
public:
	struct Vertex {
		glm::vec3 postion;
		glm::vec2 texCoords;
		glm::vec3 normal;
	};

	Cylinder(float topRadius, float bottomRadius, int numSlices, float height, bool topCircle, bool bottomCirlce);
	void render() const;
	void deleteVBO();

private:
	int numVerticesSide;		// How many vertices to render side of the cylinder
	int numVerticesTopBottom;	// How many vertices to render top / bottom of the cylinder
	int numVerticesTotal;		// Just a sum of both numbers above

	unsigned int VAO, VBO;

	bool hasTop;
	bool hasBottom;
};


#endif // !CYLINDER_H
