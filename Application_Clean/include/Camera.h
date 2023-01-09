#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "InputHandler.h"
#include "Properties.h"



/* Camera Class
*
 Could be abstract, could make pure virtual functions - 
 Main things are to return view and projection

 Needs a handler - this can be a window with an input handler for orbit camera or FPS camera
 Or, later, could attach Game Object and follow it

 Shoud really update to use quaterions
 Then, camera would just need vec3 position and quat orientation

 */

class Camera {

public:

	// default values
	Camera(glm::vec3 pos = glm::vec3(0, 0, 10)) :
		m_position(pos),
		m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_zoom(45.0),
		m_yaw(-90.0f),
		m_pitch(0.0),
		m_nearP(0.1f),
		m_farP(500.0f)
	{};

	// default view and proj matrices
	virtual glm::mat4 getViewMatrix() { return glm::lookAt(m_position, m_position + m_front, WORLD_UP); }
	virtual glm::mat4 getProjectionMatrix() { return glm::perspective(glm::radians(m_zoom), 1.0f, 0.1f, 20.0f); }

	virtual glm::vec3& getPosition() { return m_position; }
	virtual float getFOV() { return glm::radians(m_zoom); }
	virtual float getZoom() { return m_zoom; }
	virtual float& getNearPlane() { return m_nearP; }
	virtual float& getFarPlane() { return m_farP; }
	virtual glm::vec3& getFront() { return  m_front; }
	virtual glm::vec3& getUp() { return  m_up; }
	virtual glm::vec3& getRight() { return  m_right; }

	// Control
	virtual void update(float dt) {};
	virtual void attachHandler(GLFWwindow* W, std::shared_ptr<InputHandler> H) {}; // keyboard


protected:

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_right;     // camera right
	glm::vec3 m_up;        // camera up

	float m_zoom;
	float m_yaw;
	float m_pitch;
	float m_nearP;
	float m_farP;

};


