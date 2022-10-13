#pragma once


#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <numeric>

#include "Properties.h"

/*

Takes data from windowManager
Sorts out which key was pressed, where the mouse is, mouse clicked etc.
Straightforward

So far:

 only two mice buttons  
 Can only drag with left click.
 Only key press and released, not held

 Use as start, move to proper Event System when applications get large
*/

# define numOfMouseButtons 2
# define numKeys 350



class InputHandler {

public:
    InputHandler() {}
    ~InputHandler() {};
    void processKeyPress(int key, int scancode, int actions, int mods);
    void processMouseScroll(float xOffset, float yOffset);
    void processMouseMove(float xPos, float yPos);
    void processMouseClick(int button, int action, int mods);
    void endFrame();

    //mouse getters   
    inline float& getMousePosX()    { return m_lastX; }
    inline float& getMousePosY()    { return m_lastY; }
    inline float& getMouseDeltaX()  { return m_mouseDeltaX; }
    inline float& getMouseDeltaY()  { return m_mouseDeltaY; }
    inline float& getMouseScrollX() { return m_Xscroll; }
    inline float& getMouseScrollY() { return m_Yscroll; }
    inline bool& isRightClicked()   { return m_mouseButtonPressed[1]; }
    inline bool& isLeftClicked()    { return m_mouseButtonPressed[0]; }
    inline bool  isDragging()       { return m_isDragging && m_mouseButtonPressed[1]; }
    inline bool  mouseHasMoved()    { return (m_mouseDeltaX != 0.0 && m_mouseDeltaY != 0.0); }

    // keyboard getters
    bool  keyHasBeenPressed();
    bool& isKeyPressed(int keyCode) { return m_keysPressed[keyCode]; }
    void processedKey(int keyCode)  { m_keysPressed[keyCode] = false; } // after processing a key event


private:
    // mouse vars
    float m_Xscroll = 0.0;
    float m_Yscroll = 0.0;
    float m_Xpos = SCR_WIDTH / 2;  //centre of the screen 
    float m_Ypos = SCR_HEIGHT / 2;
    float m_mouseDeltaX = 0.0f;
    float m_mouseDeltaY = 0.0f;
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
    bool m_mouseButtonPressed[numOfMouseButtons] = { 0 };  // mouse buttons 0 is left, 1 is right, can be extended for fancier mice
    bool m_isDragging = false;
    bool m_firstPress = true;
    // key vars
    bool m_keysPressed[numKeys] = { 0 }; // array of all keys, look at GLFWinputCodes.h , index in array corresponds to key value. Example, C is 67, D is 68, and so on..







};
