// STL
#include <iostream>
#include <string>
#include <vector>

// GLAD header
#include <glad/glad.h>	

// Project
#include "cylinder.h"

using namespace std;

Cylinder::Cylinder(float topRadius, float bottomRadius, int numSlices, float height, bool topCircle, bool bottomCirlce)
{

	this->hasTop = topCircle;
	this->hasBottom = bottomCirlce;

	// calculate number of vertices
	numVerticesSide = (numSlices + 1) * 2;
	numVerticesTopBottom = numSlices + 2;
	numVerticesTotal = numVerticesSide + numVerticesTopBottom * 2;

	// calculate sines/cosines for number of slices
	const auto sliceAngleStep = 2.0f * glm::pi<float>() / float(numSlices);
	auto currentSliceAngle = 0.0f;
	std::vector<float> sines, cosines;
	for (auto i = 0; i <= numSlices; i++)
	{
		sines.push_back(sin(currentSliceAngle));
		cosines.push_back(cos(currentSliceAngle));

		// update slice angle
		currentSliceAngle += sliceAngleStep;
	}

	// calculate X and Z coordinates for top and bottom circles
	std::vector<float> xTop, xBottom;
	std::vector<float> zTop, zBottom;
	for (auto i = 0; i <= numSlices; i++)
	{
		xTop.push_back(cosines[i] * topRadius);
		zTop.push_back(sines[i] * topRadius);
		xBottom.push_back(cosines[i] * bottomRadius);
		zBottom.push_back(sines[i] * bottomRadius);
	}

	std::vector<glm::vec3> postions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	/*
	* POSTIONS
	*/

	// calculate vertices for cone sides
	for (auto i = 0; i <= numSlices; i++)
	{
		const auto topPosition = glm::vec3(xTop[i], height / 2.0f, zTop[i]);
		const auto bottomPosition = glm::vec3(xBottom[i], -height / 2.0f, zBottom[i]);
		postions.push_back(topPosition);
		postions.push_back(bottomPosition);
	}

	// add top circle vertices

	if (hasTop)
	{
		glm::vec3 topCenterPosition(0.0f, height / 2.0f, 0.0f);
		postions.push_back(topCenterPosition);

		for (auto i = 0; i <= numSlices; i++)
		{
			const auto topPosition = glm::vec3(xTop[i], height / 2.0f, zTop[i]);
			postions.push_back(topPosition);
		}
	}
	
	// add bottom circle vertices
	if (hasBottom)
	{
		glm::vec3 bottomCenterPosition(0.0f, -height / 2.0f, 0.0f);
		postions.push_back(bottomCenterPosition);

		for (auto i = 0; i <= numSlices; i++)
		{
			const auto bottomPosition = glm::vec3(xBottom[i], -height / 2.0f, -zBottom[i]);
			postions.push_back(bottomPosition);
		}
	}
	

	/*
	* TEXTURES
	*/

	// calculate texture coord for two wraps around object
	const auto sliceTextureStepU = 2.0f / float(numSlices);
	auto currentSliceTexCoordU = 0.0f;

	for (auto i = 0; i <= numSlices; i++)
	{
		texCoords.push_back(glm::vec2(currentSliceTexCoordU, 1.0f));
		texCoords.push_back(glm::vec2(currentSliceTexCoordU, 0.0f));

		// update texture coordinate of current slice 
		currentSliceTexCoordU += sliceTextureStepU;
	}

	// generate texture coords for top circle
	glm::vec2 topBottomCenterTexCoord(0.5f, 0.5f);
	if (hasTop)
	{
		texCoords.push_back(topBottomCenterTexCoord);
		for (auto i = 0; i <= numSlices; i++)
		{
			texCoords.push_back(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y + cosines[i] * 0.5f));
		}
	}
	
	// generate texture coords for bottom circle
	if (hasBottom)
	{
		texCoords.push_back(topBottomCenterTexCoord);
		for (auto i = 0; i <= numSlices; i++)
		{
			texCoords.push_back(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y - cosines[i] * 0.5f));
		}
	}
	

	/*
	* NORMALS
	*/
	for (auto i = 0; i <= numSlices; i++)
	{
		normals.push_back(glm::vec3(cosines[i], 0.0f, sines[i]));
		normals.push_back(glm::vec3(cosines[i], 0.0f, sines[i]));
	}

	// add normals for all vertices in top circle
	if (hasTop)
	{
		for (auto i = 0; i <= numVerticesTopBottom; i++)
		{
			normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	
	// add normals for all vertices in bottom circle
	if (hasBottom)
	{
		for (auto i = 0; i <= numVerticesTopBottom; i++)
		{
			normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}
	

	/*
	* CREATE VERTICES
	*/
	std::vector<Vertex> vertices;
	for (int i = 0; i < postions.size(); i++)
	{
		Vertex vertex;

		vertex.postion = postions.at(i);
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

void Cylinder::render() const
{
	glBindVertexArray(VAO);

	// render sides of cone
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numVerticesSide);

	// render top circle
	
	glDrawArrays(GL_TRIANGLE_FAN, numVerticesSide, numVerticesTopBottom);
	

	// render bottom circle
	
	glDrawArrays(GL_TRIANGLE_FAN, numVerticesSide + numVerticesTopBottom, numVerticesTopBottom);
	
}

void Cylinder::deleteVBO()
{
	glDeleteBuffers(1 ,&VBO);
}