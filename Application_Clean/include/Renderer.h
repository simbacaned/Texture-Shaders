#pragma once

#include "Scene.h"
#include "Shader.h"
#include "normalMapper.h"

class Renderer
{
public:
	Renderer(unsigned int w, unsigned int h);
	void renderScene(Shader& cubeShader, Shader& floorShader, Camera& cam, float time);
	void renderLights(Shader& lightShader, Camera& cam);
	void drawCube(Shader& shader, float time);
	void drawFloor(Shader& shader);
	void drawLight(Shader& lightShader, glm::mat4 model);
	void createCube();
	void createFloor();
	void createQuad();
	void drawQuad(Shader& shader, unsigned int& textureObj);
	unsigned int loadTexture(char const* path);
private:
	void loadTextures();
	unsigned int cubeVAO, floorVAO, cubeVBO, cubeEBO, floorEBO, floorVBO, quadVAO, VBO;
	unsigned int scrWidth, scrHeight;
	unsigned int metalPlateDiffuse, metalPlateNormal, metalPlateSpec, rustyIronDiffuse, rustyIronNormal, rustyIronSpec, rustyIronHeight, metalPlateHeight;
};