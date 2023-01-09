#pragma once

#include "Scene.h"
#include "Shader.h"
#include "Renderer.h"
#include "TDO.h"

class BasicScene : public Scene
{
public:
	BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H); // constructor
	Shader* getShader1() {return m_cubeShader;}
	void update(float dt) override;   // per frame update
	FirstPersonCamera* m_camera;
	Renderer renderer =  Renderer(SCR_WIDTH, SCR_HEIGHT);
	cube myCube;
private:
	void SetUniforms(Shader shader);
	void setLightAndMaterialUniforms(Shader& shader);
	void setFBOcolourAndDepth();
	unsigned int  m_floorVBO, m_cubeVBO, m_floorEBO, m_cubeEBO, m_cubeVAO, m_floorVAO;
	unsigned int FBO_colourAndDepth, depthAttachment;
	unsigned int colourAttachment[3];
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_model;
	glm::vec3 cubeColour;
	glm::vec3 floorColour;
	glm::vec3 lightColour;
	Shader* m_cubeShader;
	Shader* m_planeShader;
	Shader* m_lightShader;
	Shader* m_postProcessDepth;
	Shader* m_postProcessCol;
	Shader* m_shadowMapShader;
	

	

	





};