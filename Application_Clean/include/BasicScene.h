#pragma once

#include "Scene.h"
#include "Shader.h"

class BasicScene : public Scene
{
public:
	BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H); // constructor
	void update(float dt) override;   // per frame update
	FirstPersonCamera* m_camera;
	//a seccond test comment

private:
	void createBuffers();
	unsigned int m_floorVBO, m_cubeVBO, m_floorEBO, m_cubeEBO, m_cubeVAO, m_floorVAO;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_model;

	glm::vec3 cubeColour;
	glm::vec3 floorColour;
	glm::vec3 lightDir;
	glm::vec3 lightColour;

	Shader* m_shader;

	//////////////////// cube data

	// vertices
	float m_cubeVertices[144] = {
		//back
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 0
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   //1
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	    //front
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   //4
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		//left
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  //8
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//right
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   //12
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		//bottom
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  //16
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		//top	
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //20
			  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

	};
	// indices for index draw calls
	unsigned int m_cubeIndices[36] = {
	    1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		23,22,21,
		23,21,20,

		16,17,18,
		16,18,19,

	};

	//////////////////// floor data

	float floorSize = 5.0f;
	float floorLevel = -2.0f;

	float m_floorVertices[24] = {
		 -floorSize, floorLevel,  -floorSize, 0.0, 1.0, 0.0,
		 floorSize, floorLevel,   -floorSize, 0.0, 1.0, 0.0,
		 floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,
		-floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,
	};

	unsigned int m_floorIndices[6] = {
		3,2,1,
		3,1,0
	};





};