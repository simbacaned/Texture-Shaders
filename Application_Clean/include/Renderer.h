/*****************************************************************//**
@file   Renderer.h
@brief  The Renderer class provides functions for rendering the scene, including drawing cubes, floors, lights, and textures.
@author Joseph-Cossins-Smith
@date   July 2023
 *********************************************************************/
#pragma once

#include "Scene.h"
#include "Shader.h"
#include "normalMapper.h"

 /**
* @class Renderer
* @brief Manages rendering and drawing operations for the scene.
*/
class Renderer
{
public:
    /**
    * @brief Constructor for the Renderer class.
    * @param w The width of the rendering viewport.
    * @param h The height of the rendering viewport.
    */
    Renderer(unsigned int w, unsigned int h);

    /**
    * @brief Renders the main scene.
    * @param cubeShader The shader for rendering cubes.
    * @param floorShader The shader for rendering the floor.
    * @param cam The camera used for rendering.
    * @param time The current time value.
    */
    void renderScene(Shader& cubeShader, Shader& floorShader, Camera& cam, float time);

    /**
    * @brief Renders the lights in the scene.
    * @param lightShader The shader for rendering lights.
    * @param cam The camera used for rendering.
    */
    void renderLights(Shader& lightShader, Camera& cam);

    /**
    * @brief Draws a cube in the scene.
    * @param shader The shader used for rendering the cube.
    * @param time The current time value.
    */
    void drawCube(Shader& shader, float time);

    /**
    * @brief Draws the floor in the scene.
    * @param shader The shader used for rendering the floor.
    */
    void drawFloor(Shader& shader);

    /**
    * @brief Draws a light source in the scene.
    * @param lightShader The shader used for rendering lights.
    * @param model The model matrix for the light.
    */
    void drawLight(Shader& lightShader, glm::mat4 model);

    /** @brief Creates a cube vertex array object. */
    void createCube();

    /** @brief Creates a floor vertex array object.*/
    void createFloor();

    /** @brief Creates a quad vertex array object. */
    void createQuad();

    /**
    * @brief Draws a quad with a given texture.
    * @param shader The shader used for rendering the quad.
    * @param textureObj The ID of the texture to be applied.
    */
    void drawQuad(Shader& shader, unsigned int& textureObj);

    /**
    * @brief Loads a texture from a file path.
    * @param path The file path to the texture image.
    * @return The ID of the loaded texture.
    */
    unsigned int loadTexture(char const* path);

private:
    /** @brief Loads and binds textures used in the scene. */
    void loadTextures();

    unsigned int cubeVAO, floorVAO, cubeVBO, cubeEBO, floorEBO, floorVBO, quadVAO, VBO;
    unsigned int scrWidth, scrHeight;
    unsigned int metalPlateDiffuse, metalPlateNormal, metalPlateSpec, rustyIronDiffuse, rustyIronNormal, rustyIronSpec, rustyIronHeight, metalPlateHeight;
};