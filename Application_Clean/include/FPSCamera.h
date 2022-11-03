#pragma once
#include "Camera.h"
#include "Shader.h"


/*

Very Basic First Person Camera
WSAD for movement and mouse for rotate/zoom




*/
class FirstPersonCamera : public Camera {

public:
	FirstPersonCamera(glm::vec3 pos = glm::vec3(0.0f, 1.0f, 10.0f));

	glm::mat4 getViewMatrix()                                          override;
	glm::mat4 getProjectionMatrix()                                    override;
	void update(float dt)                                              override;
	void attachHandler(GLFWwindow* W, std::shared_ptr<InputHandler> H) override;

private:

	float m_speed;         // camera speed
	float m_sensitivity;   // mouse sensitivity
	float m_pitchContrain; // don't allow pitch to go above/below 90 otherwise screen flips
	bool isBlinn = true;
	bool isPressingE = false;
	bool isPressing1 = false;
	bool isPressing2 = false;
	bool isPressing3 = false;
	Shader* m_shader;
	GLFWwindow* m_window;
	std::shared_ptr<InputHandler> m_handler;

	void updateCameraVectors();
	void zoom(float offsetY);
	void look(float offsetX, float offsetY);  // mouse scroll



};

