/**
    @headerfile scene.h
    @author Tarkan Kemalzade
    @date 23/03/2017
*/
#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <Graphics-Engine\camera.h>

class Scene
{
    public:
        //!Defualt constructor which sets the member varible to true.
        Scene() : m_bAnimate(true) {};

        /**
        Loads textures and initialises shaders.

        @param camera <Camera> - Load the camera into the scene.
        */
        virtual void initScene(Camera camera) = 0;

        /**
        Draws the scene

        @param camera <Camera> - Draws the camera veiw into the scene
        */
        virtual void render(Camera camera) = 0;

        /**
        Called when the screen is resized
        */
        virtual void resize(Camera camera, int, int) = 0;

        /**
        Defines whether something is being animated
        */
        void animate(bool bIsAnimated) { m_bAnimate = bIsAnimated; }

        /**
        Returns the value of the boolean when something is animated or isnt animated
        */
        bool animating() { return m_bAnimate; }

    protected:
        bool m_bAnimate; //! Member vairbale to determine whether something is being animated or not.
};
#endif //!SCENE_H
