/*****************************************************************//**
@file   BasicScene.h
@brief  This class provides functionality for setting up and rendering a basic scene using GLFW and OpenGL.

@author Joseph-Cossins-Smith
@date   July 2023
 *********************************************************************/
#pragma once

#include "Scene.h"
#include "Shader.h"
#include "Renderer.h"
#include "TDO.h"

 /** @brief Class representing a basic scene with rendering and camera functionality.*/
class BasicScene : public Scene
{
public:
    /**
    * @brief Constructor for BasicScene.
    * Initializes the basic scene with the given window and input handler.
    *
    * @param window A pointer to the GLFW window for rendering.
    * @param H A shared pointer to an InputHandler for input processing.
    */
    BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H);

    /**
    * @brief Update function for per-frame updates of the scene.
    * @param dt The time elapsed since the last frame in seconds.
    */
    void update(float dt) override;

    FirstPersonCamera* m_camera;  /**< Pointer to the first-person camera in the scene. */
    Renderer renderer = Renderer(SCR_WIDTH, SCR_HEIGHT);  /**< The renderer used for rendering. */

    cube myCube;  /**< An instance of the cube class used in the scene. */

private:
    /**
    * @brief Set uniforms for shaders.
    * @param shader The shader for which uniforms need to be set.
    */
    void SetUniforms(Shader shader);

    /**
    * @brief Set light and material uniforms for shaders.
    * @param shader The shader for which light and material uniforms need to be set.
    */
    void setLightAndMaterialUniforms(Shader& shader);

    /** @brief Set framebuffer color and depth attachments. */
    void setFBOcolourAndDepth();

    unsigned int m_floorVBO, m_cubeVBO, m_floorEBO, m_cubeEBO, m_cubeVAO, m_floorVAO; //!< Vertex and element buffer objects for floor and cube.
    unsigned int FBO_colourAndDepth, depthAttachment; //!< Framebuffer and depth attachment IDs.
    unsigned int colourAttachment[3]; //!< Color attachments for the framebuffer.
    glm::mat4 m_projection; //!< Projection matrix.
    glm::mat4 m_view; //!< View matrix.
    glm::mat4 m_model; //!< Model matrix.
    glm::vec3 cubeColour; //!< Color of the cube.
    glm::vec3 floorColour; //!< Color of the floor.
    glm::vec3 lightColour; //!< Color of the light.
    Shader* m_cubeShader; //!< Shader for rendering the cube.
    Shader* m_planeShader; //!< Shader for rendering the floor.
    Shader* m_lightShader; //!< Shader for rendering the light source.
    Shader* m_postProcessDepth; //!< Shader for post-processing depth.
    Shader* m_postProcessCol; //!< Shader for post-processing color.
    Shader* m_shadowMapShader; //!< Shader for shadow map rendering.
};