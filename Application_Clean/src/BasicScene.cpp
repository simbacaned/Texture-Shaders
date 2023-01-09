#include "BasicScene.h"
#include <stb_image.h>

BasicScene::BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H) : Scene(window, H)
{

	glEnable(GL_DEPTH_TEST);
	m_camera = new FirstPersonCamera();
	m_camera->attachHandler(window, H);

	m_cubeShader = new Shader("..\\shaders\\Fvert.vert", "..\\shaders\\FfragTEST.frag");
	m_planeShader = new Shader("..\\shaders\\Fvert.vert", "..\\shaders\\FfragTEST.frag");
	m_lightShader = new Shader("..\\shaders\\lightShader.vs", "..\\shaders\\lightShader.fs");
	m_postProcessDepth = new Shader("..\\shaders\\PP.vs", "..\\shaders\\PPd.fs");
	m_postProcessCol = new Shader("..\\shaders\\PP.vs", "..\\shaders\\PPc.fs");
	m_shadowMapShader = new Shader("..\\shaders\\SM.vert", "..\\shaders\\SM.frag");

	myCube.setPos(glm::vec3(1.0));
	myCube.setScale(glm::vec3(1.0));

	m_cubeShader->use();
	setLightAndMaterialUniforms(*m_cubeShader);

	m_planeShader->use();
	setLightAndMaterialUniforms(*m_planeShader);

	m_postProcessDepth->use();
	m_postProcessDepth->setInt("image", 0);
	m_postProcessCol->use();
	m_postProcessCol->setInt("image", 0);

	setFBOcolourAndDepth();
}

void BasicScene::update(float dt)
{

	m_camera->update(dt); // check for input

	// update transforms
	m_projection = glm::perspective(m_camera->getZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);   // near plane, far plane - these could/should be global properties or member variables
	m_view = m_camera->getViewMatrix();
	m_model = glm::mat4(1.0f);

	// set uniforms - why do we set this each frame?


	glBindFramebuffer(GL_FRAMEBUFFER, FBO_colourAndDepth);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetUniforms(*m_cubeShader);
	SetUniforms(*m_planeShader);
	renderer.renderScene(*m_cubeShader, *m_planeShader, *m_camera, glfwGetTime());
	//myCube._draw(GL_TRIANGLES);
	renderer.renderLights(*m_lightShader, *m_camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	renderer.drawQuad(*m_postProcessCol, colourAttachment[0]);
}



void BasicScene::setLightAndMaterialUniforms(Shader& shader)
{
	float bloomMinBrightness = 0.5f;
	glm::vec3 dirAm = glm::vec3(0.5);
	glm::vec3 dirDiff = glm::vec3(1.0);
	glm::vec3 dirSpec = glm::vec3(1.0);

	// Dir Light
	shader.setVec3("lightPosition", glm::vec3(1, 1, 0));
	shader.setVec3("light.ambient", dirAm);
	shader.setVec3("light.diffuse", dirDiff);
	shader.setVec3("light.specular", dirSpec);

	// Point light 1
	shader.setVec3("pointLightPositions[0]", glm::vec3(1.4, 3.0, 0.6));
	shader.setVec3("pLights[0].ambient", 0.0f, 0.0f, 0.8f);
	shader.setVec3("pLights[0].diffuse", 0.0f, 0.0f, 1.0f);
	shader.setVec3("pLights[0].specular", 0.0f, 0.0f, 1.0f);
	shader.setFloat("pLights[0].Kc", 1.0);
	shader.setFloat("pLights[0].Kl", 0.35);
	shader.setFloat("pLights[0].Ke", 0.022);
	// Point light 2
	shader.setVec3("pointLightPositions[1]", glm::vec3(-3.5, -1.2, -4.0));
	shader.setVec3("pLights[1].ambient", 0.5f, 0.1f, 0.1f);
	shader.setVec3("pLights[1].diffuse", 1.0f, 0.2f, 0.2f);
	shader.setVec3("pLights[1].specular", 1.0f, 0.0f, 0.0f);
	shader.setFloat("pLights[1].Kc", 1.0);
	shader.setFloat("pLights[1].Kl", 0.35);
	shader.setFloat("pLights[1].Ke", 0.022);
	// Point light 3
	shader.setVec3("pointLightPositions[2]", glm::vec3(-3.5, -1.1, 4.0));
	shader.setVec3("pLights[2].ambient", 0.5f, 0.9f, 0.0f);
	shader.setVec3("pLights[2].diffuse", 1.0f, 1.0f, 0.2f);
	shader.setVec3("pLights[2].specular", 1.0f, 1.0f, 0.0f);
	shader.setFloat("pLights[2].Kc", 1.0);
	shader.setFloat("pLights[2].Kl", 0.35);
	shader.setFloat("pLights[2].Ke", 0.022);

	shader.setInt("numOfpLights", 3);

	// material properties 
	shader.setVec3("mat.ambient", 0.4f, 0.4f, 0.4f);
	shader.setInt("mat.diffuseTexture", 0);
	shader.setInt("mat.specularTexture", 1);
	shader.setInt("mat.normalMap", 2);
	shader.setInt("mat.heightMap", 3);
	shader.setFloat("mat.shininess", 2);

	shader.setFloat("bloomBrightness", bloomMinBrightness);
}

void BasicScene::SetUniforms(Shader shader)
{
	shader.use();
	//toggles
	shader.setBool("useBlinn", m_camera->isBlinn);
	shader.setBool("useSpot", m_camera->isSpot);
	shader.setBool("usePoint", m_camera->isPoint);
	shader.setBool("useNorm", m_camera->isNorm);
	shader.setBool("useRim", m_camera->isRim);
}

void BasicScene::setFBOcolourAndDepth()
{
	glGenFramebuffers(1, &FBO_colourAndDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_colourAndDepth);

	glGenTextures(2, colourAttachment);

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colourAttachment[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colourAttachment[i], 0);
	}

	//depth
	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


