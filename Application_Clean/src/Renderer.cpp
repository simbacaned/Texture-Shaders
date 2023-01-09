#include "Renderer.h"
#include <stb_image.h>

Renderer::Renderer(unsigned int w, unsigned int h)
{
	createCube();
	createFloor();
	createQuad();
	loadTextures();
	scrWidth = w;
	scrHeight = h;
}

void Renderer::renderScene(Shader& cubeShader, Shader& floorShader, Camera& cam, float time)
{
	glm::mat4 projection = glm::perspective(glm::radians(cam.getZoom()), (float)(scrWidth / scrHeight), 0.1f, 100.0f);
	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	//std::cout << "rendering scene" << std::endl;
	cubeShader.use();
	cubeShader.setMat4("projection", projection);
	cubeShader.setMat4("view", view);
	cubeShader.setVec3("eyePos", cam.getPosition());
	
	floorShader.use();
	floorShader.setMat4("projection", projection);
	floorShader.setMat4("view", view);
	floorShader.setMat4("model", model);
	floorShader.setVec3("eyePos", cam.getPosition());

	drawCube(cubeShader, time);
	drawFloor(floorShader);
}

void Renderer::renderLights(Shader& lightShader, Camera& cam)
{
	//std::cout << "rendering lights" << std::endl;
	glm::mat4 projection = glm::perspective(glm::radians(cam.getZoom()), (float)(scrWidth / scrHeight), 0.1f, 100.0f);
	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	lightShader.use();
	lightShader.setMat4("projection", projection);
	lightShader.setMat4("view", view);

	lightShader.setVec3("lightColour", glm::vec3(0.5, 0.5, 1.0));
	model = glm::translate(model, glm::vec3(1.4, 3.0, 0.6));
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	lightShader.setMat4("model", model);
	drawLight(lightShader, model);
	lightShader.setVec3("lightColour", glm::vec3(1.0, 0.2, 0.2));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.5, -1.2, -4.0));
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	lightShader.setMat4("model", model);
	drawLight(lightShader, model); 
	lightShader.setVec3("lightColour", glm::vec3(0.5, 0.9, 0.0));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.5, -1.1, 4.0));
	model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
	lightShader.setMat4("model", model);
	drawLight(lightShader, model);
}

void Renderer::drawCube(Shader& shader, float time)
{
	//std::cout << "drawing cube" << std::endl;
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalPlateDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalPlateSpec);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metalPlateNormal);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, metalPlateHeight);
	glBindVertexArray(cubeVAO);  // bind and draw cube
	// Cube 1
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Cube 2
	model = glm::rotate(model, time, glm::vec3(0.01, 0.01, 0.02));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
	model = glm::translate(model, glm::vec3(4.0, 2.0, 1.0));
	shader.setMat4("model", model);
	model = glm::mat4(1.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Cube 3
	model = glm::scale(model, glm::vec3(0.75, 0.75, 0.75));
	model = glm::translate(model, glm::vec3(-2.0, -2.0, 4.0));
	shader.setMat4("model", model);
	model = glm::mat4(1.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Cube 4
	model = glm::rotate(model, 10.0f, glm::vec3(1.0, 0.3, 0.2));
	model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));
	model = glm::translate(model, glm::vec3(-3.0, -3.0, -4.0));
	shader.setMat4("model", model);
	model = glm::mat4(1.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void Renderer::drawFloor(Shader& shader)
{
	//std::cout << "drawing floor" << std::endl;
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rustyIronDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rustyIronSpec);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, rustyIronNormal);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, rustyIronHeight);
	glBindVertexArray(floorVAO);  // bind and draw floor

	// floor
	shader.setMat4("model", model);
	model = glm::mat4(1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::createCube()
{
	//std::cout << "creating cube" << std::endl;
	// vertices
	float m_cubeVertices[192] =
	{
		//xyznxnynzuv
		//back
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,// 0 
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, //1
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		//front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,//4
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

		//left
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//right
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,//12
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,//16
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		//top	
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //20
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
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

		19,18,17,
		19,17,16,

		23,22,21,
		23,21,20,

	};

	// Create VAO
	// Cube

	normalMapper normMap;
	normMap.calculateTan(m_cubeVertices, 192, m_cubeIndices, 36);
	std::vector<float> updatedCubeVertices = normMap.getUpdatedVertexData();

	glGenVertexArrays(1, &cubeVAO);  // generate 1 VAO buffer called m_cubeVAO
	glGenBuffers(1, &cubeVBO);       // generate buffer called m_cubeVBO  - this is for vertex data
	glGenBuffers(1, &cubeEBO);       // another called EBO for index info

	glBindVertexArray(cubeVAO);  // bind VAO

	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, updatedCubeVertices.size() * sizeof(float), updatedCubeVertices.data(), GL_STATIC_DRAW);   // 'send' or copy the data in m_cubeVertices from CPU to GPU, 'STATIC_DRAW' is a hint to OpenGL that we will not be updating the data in this buffer
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);  // bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_cubeIndices), m_cubeIndices, GL_STATIC_DRAW); // 'send' or copy the data in m_cubeIndices from CPU to GPU

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0); //params: at position 0, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 0 here)
	glEnableVertexAttribArray(0);  // first attribute
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(1);
	// uv 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// biTan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
}

void Renderer::createFloor()
{	
	//std::cout << "creating floor" << std::endl;
	/////////////////// floor data
	float floorSize = 5.0f;
	float floorLevel = -2.0f;
	float m_floorVertices[32] = {
		//xyznxnynzuv
		-floorSize, floorLevel, -floorSize, 0.0, 1.0, 0.0, 0.0f, 0.0f,
		 floorSize, floorLevel, -floorSize, 0.0, 1.0, 0.0, 1.0f, 0.0f,
		 floorSize, floorLevel, floorSize, 0.0, 1.0, 0.0, 1.0f,1.0f,
		-floorSize, floorLevel, floorSize, 0.0, 1.0, 0.0, 0.0f,1.0f

	};

	unsigned int m_floorIndices[6] = {
		3,2,1,
		3,1,0
	};

	normalMapper normMap;
	normMap.calculateTan(m_floorVertices, 32, m_floorIndices, 6);
	std::vector<float> updatedFloorVertices = normMap.getUpdatedVertexData();

	glGenVertexArrays(1, &floorVAO);  // generate 1 VAO buffer called floorVAO
	glGenBuffers(1, &floorVBO);       // generate buffer called floorVBO  - this is for vertex data
	glGenBuffers(1, &floorEBO);       // another called EBO for index info

	glBindVertexArray(floorVAO);  // bind VAO

	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, updatedFloorVertices.size() * sizeof(float), updatedFloorVertices.data(), GL_STATIC_DRAW);   // 'send' or copy the data in m_cubeVertices from CPU to GPU, 'STATIC_DRAW' is a hint to OpenGL that we will not be updating the data in this buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_floorIndices), m_floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// biTan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);
}



void Renderer::createQuad()
{
	float quadVertices[] =
	{
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// set plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::drawQuad(Shader& shader, unsigned int& textureObj)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void Renderer::drawLight(Shader& lightShader, glm::mat4 model)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?
	glBindVertexArray(cubeVAO);  // bind and draw cube

	//Cube1
	lightShader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Renderer::loadTextures()
{
	metalPlateDiffuse = loadTexture("..\\Resources\\Textures\\metalPlate\\diffuse.jpg");
	metalPlateNormal = loadTexture("..\\Resources\\Textures\\metalPlate\\normal.jpg");
	metalPlateSpec = loadTexture("..\\Resources\\Textures\\metalPlate\\specular.jpg");
	metalPlateHeight = loadTexture("..\\Resources\\Textures\\metalPlate\\height.jpg");
	rustyIronDiffuse = loadTexture("..\\Resources\\Textures\\metalRust\\diffuse.jpg");
	rustyIronNormal = loadTexture("..\\Resources\\Textures\\metalRust\\normal.jpg");
	rustyIronSpec = loadTexture("..\\Resources\\Textures\\metalRust\\specular.jpg");
	rustyIronHeight = loadTexture("..\\Resources\\Textures\\metalRust\\height.jpg");
}

unsigned int Renderer::loadTexture(char const* path)
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
		std::cout << "Loaded texture at path: " << path << " width " << width << " id " << textureID << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}