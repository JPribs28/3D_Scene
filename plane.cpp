// STL
#include <vector>
#include <iostream>

// GLAD header
#include <glad/glad.h>	

// Project
#include "plane.h"

Plane::Plane(float x, float z)
{
	// postions
	std::vector<glm::vec3> positions;
	// first triangle
	positions.push_back(glm::vec3(-x, 0.0f, z));
	positions.push_back(glm::vec3(-x, 0.0f, -z));
	positions.push_back(glm::vec3(x, 0.0f, -z));
	// second triangle
	positions.push_back(glm::vec3(-x, 0.0f, z));
	positions.push_back(glm::vec3(x, 0.0f, -z));
	positions.push_back(glm::vec3(x, 0.0f, z));


	// texture coords
	std::vector<glm::vec2> texCoords;
	// first triangle
	texCoords.push_back(glm::vec2(0.0f, 1.0f));
	texCoords.push_back(glm::vec2(0.0f, 0.0f));
	texCoords.push_back(glm::vec2(1.0f, 0.0f));
	// second triangle
	texCoords.push_back(glm::vec2(0.0f, 1.0f));
	texCoords.push_back(glm::vec2(1.0f, 0.0f));
	texCoords.push_back(glm::vec2(1.0f, 1.0f));


	// normals
	std::vector<glm::vec3> normals;
	for (int i = 0; i < positions.size(); i++)
	{
		normals.push_back(glm::vec3(0.0, 1.0, 0.0));
	}
	
	// assemble vertices into a vector of Vertex structs
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		Vertex vertex;

		vertex.postion = positions.at(i);
		vertex.texCoords = texCoords.at(i);
		vertex.normal = normals.at(i);

		vertices.push_back(vertex);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

void Plane::render() const
{
	glBindVertexArray(VAO);

	// render sides of cone
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Plane::deleteVBO()
{
	glDeleteBuffers(1, &VBO);
}