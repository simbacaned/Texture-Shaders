#include "normalMapper.h"

normalMapper::normalMapper()
{

}

void normalMapper::calculateTan(float* vertexData, int length, unsigned int* indicesData, int indLength)
{
	extractVertices(vertexData, length);
	computeTanAndBitan(indicesData, indLength);
	unpackVertices();
}

std::vector<float> normalMapper::getUpdatedVertexData()
{
	return updatedVertexData;
}

void normalMapper::unpackVertices()
{
	updatedVertexData.clear();
	updatedVertexData.shrink_to_fit();
	updatedVertexData.reserve(336);
	for (vertex v : vertices)
	{
		v.tan = glm::normalize(v.tan);
		v.tan = glm::normalize(v.biTan);
		int stride = 14;
		float vertexData[] =
		{
			v.pos.x, v.pos.y, v.pos.z,
			v.normal.x, v.normal.y, v.normal.z,
			v.uv.x, v.uv.y,
			v.tan.x, v.tan.y, v.tan.z,
			v.biTan.x, v.biTan.y, v.biTan.z
		};

		for (int i = 0; i < stride; i++)
		{
			updatedVertexData.push_back(vertexData[i]);
		}
		//updatedVertexData.push_back(v.pos.x);
		//updatedVertexData.push_back(v.pos.y);
		//updatedVertexData.push_back(v.pos.z);
		//updatedVertexData.push_back(v.normal.x);
		//updatedVertexData.push_back(v.normal.y);
		//updatedVertexData.push_back(v.normal.z);
		//updatedVertexData.push_back(v.uv.x);
		//updatedVertexData.push_back(v.uv.y);
		//updatedVertexData.push_back(v.tan.x);
		//updatedVertexData.push_back(v.tan.y);
		//updatedVertexData.push_back(v.tan.z);
		//updatedVertexData.push_back(v.biTan.x);
		//updatedVertexData.push_back(v.biTan.y);
		//updatedVertexData.push_back(v.biTan.z);

	}

}

void normalMapper::extractVertices(float* vertexData, int length)
{
	vertex V;
	V.tan = glm::vec3(0.0f, 0.0f, 0.0f);
	V.biTan = glm::vec3(0.0f, 0.0f, 0.0f);
	for (unsigned i = 0; i < length; i = i + 8)
	{
		V.pos = glm::vec3{ vertexData[i], vertexData[i + 1], vertexData[i + 2] };
		V.normal = glm::vec3{ vertexData[i + 3], vertexData[i + 4], vertexData[i + 5] };
		V.uv = glm::vec2{ vertexData[i + 6], vertexData[i + 7] };
		vertices.push_back(V);
	}
}

void normalMapper::computeTanAndBitan(unsigned int* indicesData, int indLength)
{
	for (unsigned i = 0; i < indLength; i = i + 3)
	{
		vertex& v1 = vertices.at(indicesData[i + 0]);
		vertex& v2 = vertices.at(indicesData[i + 1]);
		vertex& v3 = vertices.at(indicesData[i + 2]);

		glm::vec3 dp1 = v2.pos - v1.pos;
		glm::vec3 dp2 = v3.pos - v1.pos;

		glm::vec2 uv1 = v2.uv - v1.uv;
		glm::vec2 uv2 = v3.uv - v1.uv;

		float r = 1.f / ((uv1.x * uv2.y) - (uv1.y * uv2.x));
		//set tangent
		glm::vec3 tan = r * ((dp1 * uv2.y) - (dp2 * uv1.y));
		glm::vec3 biTan = r * ((dp2 * uv1.x) - (dp1 * uv2.x));
		v1.tan += tan;
		v1.biTan += biTan;
		v2.tan += tan;
		v2.biTan += biTan;
		v3.tan += tan;
		v3.biTan += biTan;
	}
}
