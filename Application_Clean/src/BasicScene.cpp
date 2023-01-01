#include "BasicScene.h"
#include <stb_image.h>

BasicScene::BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H) : Scene(window, H)
{
	glEnable(GL_DEPTH_TEST);
	m_camera = new FirstPersonCamera();
	m_camera->attachHandler(window, H);
	m_shader = new Shader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
	createBuffers();
	
}

void BasicScene::update(float dt)
{
	m_camera->update(dt); // check for input

	// update transforms
	m_projection = glm::perspective(m_camera->getFOV(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);   // near plane, far plane - these could/should be global properties or member variables
	m_view = m_camera->getViewMatrix();
	m_model = glm::mat4(1.0f);

	// set uniforms - why do we set this each frame?

	SetUniforms();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?
	glBindVertexArray(m_cubeVAO);  // bind and draw cube

	//Cube1
	m_model = glm::translate(m_model, glm::vec3(0.0, 1.0, 0.0));
	m_shader->setMat4("model", m_model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Cube2
	m_model = glm::translate(m_model, glm::vec3(0.0, 0.0, 5.0));
	m_shader->setMat4("model", m_model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Cube3
	m_model = glm::translate(m_model, glm::vec3(5.0, 0.0, 5.0));
	m_model = glm::rotate(m_model, (float)(glfwGetTime()), glm::vec3(2.0, 2.0, 2.0))	;
	m_shader->setMat4("model", m_model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Plane
	m_model = glm::mat4(1.0f);
	m_shader->setMat4("model", m_model);
	m_shader->setVec3("objectCol", glm::vec3(0.1, 0.3, 0.3));
	glBindVertexArray(m_floorVAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void BasicScene::SetUniforms()
{
	m_shader->use();  // do we need this command each frame? - Probably not if we only have one shader
	m_shader->setInt("diffuseTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalPlateTexture);

	m_shader->setMat4("projection", m_projection);
	m_shader->setMat4("view", m_view);
	m_shader->setMat4("model", m_model);
	m_shader->setVec3("cameraPos", m_camera->getPosition());
	m_shader->setVec3("lightDir", glm::vec3(0, -1, 0));
	m_shader->setVec3("lightCol", glm::vec3(1.0, 1.0, 1.0));
	m_shader->setVec3("objectCol", glm::vec3(1.0, 0.4, 0.4));

	//PointLight
	m_shader->setVec3("pLightPosition", glm::vec3(2.0, 3.0, 4.0));
	m_shader->setVec3("pLight.colour", glm::vec3(0.5, 0.5, 0.5));
	m_shader->setFloat("pLight.Kc", 1.0);
	m_shader->setFloat("pLight.Kl", 0.2f);
	m_shader->setFloat("pLight.Ke", 0.22f);

	//SpotLight
	m_shader->setVec3("sLight.position", m_camera->getPosition());
	m_shader->setVec3("sLight.direction", m_camera->getFront());
	m_shader->setVec3("sLight.colour", glm::vec3(0.1f, 0.1f, 0.1f));
	m_shader->setFloat("sLight.Kc", 1.0);
	m_shader->setFloat("sLight.Kl", 0.027f);
	m_shader->setFloat("sLight.Ke", 0.0028f);
	m_shader->setFloat("sLight.innerRad", glm::cos(glm::radians(12.5f)));
	m_shader->setFloat("sLight.outerRad", glm::cos(glm::radians(17.5f)));
}

unsigned int BasicScene::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}

		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}

		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		std::cout << "Loaded texture at path: " << path << " width " << width <<  " id " << textureID << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

void BasicScene::createBuffers()
{

	/*VAO stuff  - when you are comfortable what all of this is and what it is for - abstract to classes:
	  cube and plane class ( can use for platforms, ceilings, floors,  walls, etc.)
	  
	  Should abstract these buffer classes too VAO class, VBO class, etc. You will learn more about this in your Game Engine class.
	  */

	  // Create VAO
	  // Cube
	glGenVertexArrays(1, &m_cubeVAO);  // generate 1 VAO buffer called m_cubeVAO
	glGenBuffers(1, &m_cubeVBO);       // generate buffer called m_cubeVBO  - this is for vertex data
	glGenBuffers(1, &m_cubeEBO);       // another called EBO for index info

	glBindVertexArray(m_cubeVAO);  // bind VAO
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices), m_cubeVertices, GL_STATIC_DRAW);   // 'send' or copy the data in m_cubeVertices from CPU to GPU, 'STATIC_DRAW' is a hint to OpenGL that we will not be updating the data in this buffer
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeEBO);  // bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_cubeIndices), m_cubeIndices, GL_STATIC_DRAW); // 'send' or copy the data in m_cubeIndices from CPU to GPU


    // tell OpenGl how to interpret the data in VBO
	// This is 'stored' in VAO along with VBO so we only need to do this once and whenever we bind m_cubeVAO we bind the data and instructions on how to interpret
	// Whenever we bind m_cubeVAO we are now dealing with m_cubeVBO, m_cubeEBO, and the following instructions. All with one command: glBindVertexArray(m_cubeVAO);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //params: at position 0, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 0 here)
	glEnableVertexAttribArray(0);  // first attribute
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(1); // 2nd attribute

	//Floor
	glGenVertexArrays(1, &m_floorVAO);
	glGenBuffers(1, &m_floorVBO);
	glGenBuffers(1, &m_floorEBO);

	glBindVertexArray(m_floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_floorVertices), m_floorVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_floorIndices), m_floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


}
