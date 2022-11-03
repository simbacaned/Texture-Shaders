#include "FPSCamera.h"


FirstPersonCamera::FirstPersonCamera(glm::vec3 pos) : Camera(pos), m_speed(5.0f), m_sensitivity(0.025f)
{
	m_zoom = 45.0;
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_pitchContrain = 89.0f;
	updateCameraVectors();
	m_shader = new Shader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
}

//getters

// view matrix
glm::mat4 FirstPersonCamera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, WORLD_UP);
}
// perspective projection matrix
glm::mat4 FirstPersonCamera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, m_nearP, m_farP);
}

// for updates
void FirstPersonCamera::attachHandler(GLFWwindow* W, std::shared_ptr<InputHandler> H)
{
	m_window = W;
	m_handler = H;
}

// Camera Key movement
// Standard WSAD, could extend for any other movement you want ( eg, up, down, etc..)
void FirstPersonCamera::update(float dt)
{
	float vel = m_speed * dt;
	bool mouseMove = m_handler->mouseHasMoved();

	if (m_handler->keyHasBeenPressed()) {
		if (m_handler->isKeyPressed(GLFW_KEY_W)) {
			m_position += m_front * vel;
		}
		if (m_handler->isKeyPressed(GLFW_KEY_S)) {
			m_position -= m_front * vel;
		}
		if (m_handler->isKeyPressed(GLFW_KEY_A)) {
			m_position -= m_right * vel;
		}
		if (m_handler->isKeyPressed(GLFW_KEY_D)) {
			m_position += m_right * vel;
		}
		if (m_handler->isKeyPressed(GLFW_KEY_E)) {
			if (isPressingE == false)
			{
				isBlinn = !isBlinn;
				m_shader->setBool("useBlinn", isBlinn);
				isPressingE = true;
			}
		}
		if (m_handler->isKeyPressed(GLFW_KEY_1)) {
			if (isPressing1 == false)
			{
				isPressing1 = !isPressing1;
				m_shader->setBool("useSpot", isPressing1);
				isPressing1 = true;
			}
		}
		if (m_handler->isKeyPressed(GLFW_KEY_2)) {
			if (isPressing2 == false)
			{
				isPressing2 = !isPressing2;
				m_shader->setBool("usePoint", isPressing2);
				isPressing2 = true;
			}
		}
		if (m_handler->isKeyPressed(GLFW_KEY_3)) {
			if (isPressing3 == false)
			{
				isPressing3 = !isPressing3;
				m_shader->setBool("useRim", isPressing3);
				isPressing3 = true;
			}
		}
	}
	if (!m_handler->isKeyPressed(GLFW_KEY_E))
	{
		isPressingE = false;
	}
	if (!m_handler->isKeyPressed(GLFW_KEY_1))
	{
		isPressing1 = false;
	}
	if (!m_handler->isKeyPressed(GLFW_KEY_2))
	{
		isPressing2 = false;
	}
	if (!m_handler->isKeyPressed(GLFW_KEY_3))
	{
		isPressing3 = false;
	}

	look(m_handler->getMouseDeltaX(), m_handler->getMouseDeltaY());
	zoom(m_handler->getMouseScrollY());

	m_handler->endFrame();  // reset delta values
}



void FirstPersonCamera::updateCameraVectors()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);
	//re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, WORLD_UP));  // Normalize the vectors, because their length gets closer to zero the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));

}


void FirstPersonCamera::zoom(float offsetY)
{
	// can adjust these values - or perhaps these are better as member variables
	float min = 20.0f;
	float max = 45.0f;

	if (m_zoom >= min && m_zoom <= max) m_zoom -= offsetY;
	if (m_zoom <= min) m_zoom = min;
	if (m_zoom >= max) m_zoom = max;
}


// move mouse around and update pitch and yaw
void FirstPersonCamera::look(float x, float y)
{
		x *= m_sensitivity;
		y *= m_sensitivity;

		m_yaw += x;
		m_pitch += y;

		if (m_pitch > m_pitchContrain) m_pitch = m_pitchContrain;
		if (m_pitch < -m_pitchContrain) m_pitch = -m_pitchContrain;

		updateCameraVectors();
}


