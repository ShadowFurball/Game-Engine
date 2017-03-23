/**
    @headerfile engine-scene.h
    @author Tarkan Kemalzade
    @date 23/03/2017
*/
#pragma once

#ifndef _ENGINE_SCENE_H
#define _ENGINE_SCENE_H

#include <iostream>
#include <vector>
#include <gl_core_4_3.hpp>
#include <Graphics-Engine\shader-manager.h>
#include <Graphics-Engine\scene.h>

class EngineScene : public Scene
{
    public:
        EngineScene();

        void setLightingParameters(Camera camera);
        void initScene(Camera camera);
        void updateScene(float fTime);
        void render(Camera camera);
        void resize(Camera camera, int, int);

    private:
        ShaderManager program; // GLSL Program
        int iHeight, iWidth; // Scene width and height

        std::vector<std::string> m_fileName;

        glm::mat4 model; // Matrix for models that will be uploaded

        void setMatrices(Camera camera);
        void compileAndLinkShader();
};

#endif // !_ENGINE_SCENE_H
