#pragma once

#include "Shader.h"
#include <vector>

struct vertex 
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 biTan;
};

class normalMapper
{
public:
	normalMapper();
	void calculateTan(float* vertexData, int length, unsigned int* indicesData, int indLength);
	std::vector<float> getUpdatedVertexData();
private:
	void unpackVertices();
	void extractVertices(float* vertexData, int length);
	void computeTanAndBitan(unsigned int* indicesData, int indLength);
	std::vector<vertex> vertices;
	std::vector<float> updatedVertexData;
};