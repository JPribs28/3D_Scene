#ifndef PLANE_H
#define PLANE_H

// GLM headers 
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Plane
{
public:
	struct Vertex {
		glm::vec3 postion;
		glm::vec2 texCoords;
		glm::vec3 normal;
	};

	Plane(float x, float z);
	void render() const;
	void deleteVBO();

private:
	unsigned int VAO, VBO;
};


#endif // !PLANE_H
