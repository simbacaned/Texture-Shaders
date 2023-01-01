#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <memory>
#include <iostream>
#include "InputHandler.h"






/*
* A lot going on in this class
* GLFw window
* also creates GL context
* an input handler is initialised here as a raw pointer
* raw because of the GLFW callbacks ( glfwSetWindowUserPointer(m_window, m_rawHandler); )
* when the handler is returned in getter it is converted to a shared pointer as it might be passed around
* GLFWwindow* and shared_ptr<InputHandler> returned and passed on to application
* GL context is global, be careful if you add multi-threading
*
*/

class WindowManager {

public:

    WindowManager(const char* title, int width, int height, bool vsync) :
        m_title(title),
        m_width(width),
        m_height(height),
        m_vSync(vsync)
    {
        initialise();

    }

    ~WindowManager();
    void startFrame();
    void endFrame();
    inline void shutDown() { glfwTerminate(); }

    inline float getDeltaTime() { return m_deltaTime; }
    inline GLFWwindow* getWindow() { return m_window; }
    inline std::shared_ptr<InputHandler> getHandler() { return std::shared_ptr<InputHandler>(m_rawHandler); }

    inline void setClearColour(float r, float g, float b) { m_clearR = r; m_clearG = g; m_clearB = b; }



private:

    GLFWwindow* m_window;
    InputHandler* m_rawHandler = new InputHandler();
    void initialise();
    float m_clearR =  0.0f;  // clear screen red
    float m_clearG =  0.0f;  // green
    float m_clearB =  0.0f;  // blue
    const char* m_title;
    bool m_vSync; // synch to monitor frame rate?
    int m_width, m_height; // dimensions of app window

    //time stuff
    float m_currentFrame = 0.0;
    float m_deltaTime = 0.0;
    float m_lastFrame = 0.0;

    void tick(); // delta time

};

