/**
    @headerfile camera.h
	@author Tarkan Kemalzade
	@date 19/03/2017
*/

#pragma once

#ifndef _QUATERNION_CAMERA_H
#define _QUATERNION_CAMERA_H

#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3, float, float, float, float);

    void setFieldOfView(float);
    void setAspectRatio(float);
    void setFarPlane(float);
    void setNearPlane(float);
    void setCameraPosition(glm::vec3);

    float getFieldOfView() const;
    float getAspectRatio() const;
    float getFarPlane() const;
    float getNearPlane() const;
    glm::vec3 getCameraPosition() const;

    void resetCamera(glm::vec3, float, float, float, float);
    void updateCameraView();
    void rotateCamera(const float, const float);
    void zoom(float);
    void pan(float, float);
    void roll(float);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::quat getAxisAngle(glm::vec3, float);

private:
    float m_fFieldOfView;
    float m_fAspectRatio;
    float m_fFarPlane;
    float m_fNearPlane;

    glm::vec3 m_cameraPosX;
    glm::vec3 m_cameraPosY;
    glm::vec3 m_cameraPosZ;
    glm::vec3 m_WorldCoordinateX;
    glm::vec3 m_WorldCoordinateY;
    glm::vec3 m_WorldCoordinateZ;
    glm::vec3 m_cameraPosition;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::quat m_orientation;
};

#endif // !_QUATERNION_CAMERA_H

