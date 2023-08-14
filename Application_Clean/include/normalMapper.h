/*****************************************************************//**
@file   normalMapper.h
@brief  Class for calculating tangents and bitangents for normal mapping.

@author Joseph-Cossins-Smith
@date   July 2023
 *********************************************************************/
#pragma once

#include "Shader.h"
#include <vector>

/** @brief Struct representing a vertex with position, normal, UV, tangent, and bitangent attributes. */
struct vertex
{
	glm::vec3 pos;     /**< Position attribute. */
	glm::vec3 normal;  /**< Normal attribute. */
	glm::vec2 uv;      /**< UV attribute. */
	glm::vec3 tan;     /**< Tangent attribute. */
	glm::vec3 biTan;   /**< Bitangent attribute. */
};

/** @brief Class for calculating tangents and bitangents for normal mapping. */
class normalMapper
{
public:
    /**
    * @brief Constructor for normalMapper.
    * Initializes the normalMapper instance.
    */
    normalMapper();

    /**
    * @brief Calculate tangents and bitangents for the given vertex and index data.
    * @param vertexData Pointer to the vertex data.
    * @param length Length of the vertex data.
    * @param indicesData Pointer to the index data.
    * @param indLength Length of the index data.
    */
    void calculateTan(float* vertexData, int length, unsigned int* indicesData, int indLength);

    /**
    * @brief Get the updated vertex data with calculated tangents and bitangents.
    * @return A vector containing the updated vertex data.
    */
    std::vector<float> getUpdatedVertexData();

private:
    /** @brief Unpack vertices from the vertex data. */
    void unpackVertices();

    /**
    * @brief Extract vertex attributes from the vertex data.
    * @param vertexData Pointer to the vertex data.
    * @param length Length of the vertex data.
    */
    void extractVertices(float* vertexData, int length);

    /**
    * @brief Compute tangents and bitangents for the given index data.
    * @param indicesData Pointer to the index data.
    * @param indLength Length of the index data.
    */
    void computeTanAndBitan(unsigned int* indicesData, int indLength);

    std::vector<vertex> vertices;      /**< Vector of vertices with attributes. */
    std::vector<float> updatedVertexData; /**< Vector of updated vertex data with tangents and bitangents. */
};