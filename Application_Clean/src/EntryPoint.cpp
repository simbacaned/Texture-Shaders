#include "WindowManager.h"
#include "Properties.h"
#include "BasicScene.h" 

/*
Entry point for App
Creates a window with an input handler
Creates instances of a scene
Runs game loop
*/

void main() {

	WindowManager windowMan("Application Window", SCR_WIDTH, SCR_HEIGHT, true);  // name, width, height, vSync,
	BasicScene scene(windowMan.getWindow(), windowMan.getHandler()); // pass GLFWwindow and Input Handler to scene

	while (!glfwWindowShouldClose(windowMan.getWindow())) {


		windowMan.startFrame();
		scene.update(windowMan.getDeltaTime());
		windowMan.endFrame();

	}
	windowMan.shutDown();

}

