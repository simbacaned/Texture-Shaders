#pragma once
#include "FPSCamera.h"
/*
Everything in scene goes here
I'd suggest using this as a base class and extending for specific scenes
*/

class Scene {

public:
	Scene(GLFWwindow* window, std::shared_ptr<InputHandler> H) {};
	~Scene() {};
	virtual void update(float dt) {};
private:
	GLFWwindow* m_window;
	std::shared_ptr<InputHandler> m_handler;
	std::shared_ptr<FirstPersonCamera> m_camera;
	

};
