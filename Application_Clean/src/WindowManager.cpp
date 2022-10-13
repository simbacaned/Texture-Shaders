#include "WindowManager.h"
#include <string>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, width); }



void WindowManager::initialise()
{
    if (!glfwInit())
        throw std::runtime_error("GLFW failed to initialize.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;


    glfwSetWindowUserPointer(m_window, m_rawHandler);  // points to an Input Handler instance, this is key to using callbacks from this class https://www.glfw.org/docs/3.3/group__window.html

                                 // callbacks
    // each a lambda function which calls the associated function defined in InputHandler.cpp

    // mouse cursor
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        auto& self = *static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        self.processMouseMove(xPos, yPos);
        });

    // mouse scroll cursor
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        auto& self = *static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        self.processMouseScroll(xOffset, yOffset);
        });

    // mouse buttons
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto& self = *static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        self.processMouseClick(button, action, mods);
        });

    // key press
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int actions, int mods) {
        auto& self = *static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        self.processKeyPress(key, scancode, actions, mods);

        });


    // FrameBuffer callback, defined in this file
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);  // need to move this 
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // for FPS camera, mouse position recorded when out off application window focus


    if (m_vSync) glfwSwapInterval(1);
    else         glfwSwapInterval(0);


    std::cout << "VERSON" << glGetString(GL_VERSION) << std::endl;
}




WindowManager::~WindowManager()
{
    glfwTerminate();
    glfwDestroyWindow(m_window); 
}

// start frame, clear colour and depth buffers
void WindowManager::startFrame()
{
    tick();
    //joyPadTest();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(m_clearR, m_clearG, m_clearB, 1.0f);


}

// end frame, swap buffers
// poll for input
void WindowManager::endFrame() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}


// calculate delta time
void WindowManager::tick() {
    m_currentFrame = glfwGetTime();
    m_deltaTime = m_currentFrame - m_lastFrame;
    m_lastFrame = m_currentFrame;

}



//void WindowManager::joyPadTest()
//{    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
//   std::cout << " JoyPad " << present << std::endl;
//   int count;
//   const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
//    std::cout << " JoyPad Buttons " << count << std::endl;
//}



