#include "InputHandler.h"


void InputHandler::processMouseScroll(float xOffset, float yOffset)
{
	m_Xscroll = xOffset;
	m_Yscroll = yOffset;

}

void InputHandler::processMouseMove(float xPos, float yPos)
{

		if (m_firstPress) {  // initialise lastPos to currentPos on first frame

			m_lastX = m_Xpos;
			m_lastY = m_Ypos;
			m_firstPress = false;
		}
		// difference between current position and pre position
		m_mouseDeltaX = xPos - m_lastX;
		m_mouseDeltaY = m_lastY - yPos;

		m_lastX = xPos;
		m_lastY = yPos;
	
}

void InputHandler::processMouseClick(int button, int action, int mods)
{
	if (button < numOfMouseButtons) {  // left or right mouse, update if need additional mouse buttons
		if (action == GLFW_PRESS) {
			m_mouseButtonPressed[button] = true;  // button is pressed
			m_isDragging = false;                 // mouse is dragging
		}
		else if ((action == GLFW_RELEASE)) {
			m_mouseButtonPressed[button] = false;  // button release
			m_isDragging = false;                  // no longer dragging
		}
	}

}

//reset
void InputHandler::endFrame()
{
	m_mouseDeltaX = 0.0f;
	m_mouseDeltaY = 0.0f;
}

// has ANY key been pressed?
bool InputHandler::keyHasBeenPressed()
{
	int sum = std::accumulate(m_keysPressed, m_keysPressed + numKeys, 0);  //count all keys pressed, binary values so..
	return sum > 0.0;  // if at least one is pressed then this value will be > 0
}

void InputHandler::processKeyPress(int key, int scancode, int action, int mods)
{

	if (key < numKeys) {  // only supporting standard keyboards keys
		if (action == GLFW_PRESS)  // if some key has been pressed
			m_keysPressed[key] = true;  // this key is pressed
		else if ((action == GLFW_RELEASE))
			m_keysPressed[key] = false;
	}
}




