#include <Graphics-Engine\camera.h>

/**
    Defualt Constructor
*/
Camera::Camera()
{
    m_WorldCoordinateX = glm::vec3(1.f, 0.f, 0.f);
    m_WorldCoordinateY = glm::vec3(0.f, 1.f, 0.f);
    m_WorldCoordinateZ = glm::vec3(0.f, 0.f, 1.f);
}

/**
    Constructs a camera by settings its position, field of view, aspect
    ratio, near plane and far plane.
    @param position
    @param fov - Field of View
    @param aspect ration
    @param near plane
    @param far plane
*/
Camera::Camera(glm::vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_WorldCoordinateX = glm::vec3(1.f, 0.f, 0.f);
    m_WorldCoordinateY = glm::vec3(0.f, 1.f, 0.f);
    m_WorldCoordinateZ = glm::vec3(0.f, 0.f, 1.f);

    m_cameraPosition = position;
    m_fFieldOfView = fov;
    m_fFarPlane = farPlane;
    m_fNearPlane = nearPlane;

    m_projectionMatrix = glm::perspective(m_fFieldOfView, m_fAspectRatio, m_fNearPlane, m_fFarPlane);
    updateCameraView();
}

/**
    Sets the camera's field of view
    @param fov - field of view
*/
void Camera::setFieldOfView(float fov)
{
    m_fFieldOfView = fov;
}

/**
    Sets the camera's aspect ration
    @param aspect ratio
*/
void Camera::setAspectRatio(float aspectRatio)
{
    m_fAspectRatio = aspectRatio;
}

/**
    Sets the camera's far plane
    @param far plane
*/
void Camera::setFarPlane(float farPlane)
{
    m_fFarPlane = farPlane;
}

/**
    Sets camera's near plane
    @param near plane
*/
void Camera::setNearPlane(float nearPlane)
{
    m_fNearPlane = nearPlane;
}

/**
    Sets the camera's position
    @param position
*/
void Camera::setCameraPosition(glm::vec3 position)
{
    m_cameraPosition = position;
}

/**
    Gets the field of view
    @return m_fFieldOfView
*/
float Camera::getFieldOfView() const
{
    return m_fFieldOfView;
}

/**
    Gets the aspect ration
    @return m_fAspectRatio
*/
float Camera::getAspectRatio() const
{
    return m_fAspectRatio;
}

/**
    Gets the far plane
    @return m_fFarPlane
*/
float Camera::getFarPlane() const
{
    return m_fFarPlane;
}

/**
    Gets the near plane
    @returns m_fNearPlane
*/
float Camera::getNearPlane() const
{
    return m_fNearPlane;
}

/**
    Gets the camera's position
    @return m_cameraPosition
*/
glm::vec3 Camera::getCameraPosition() const
{
    return m_cameraPosition;
}

/**
    Resets the camera to a defualt setting
*/
void Camera::resetCamera(glm::vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_WorldCoordinateX = glm::vec3(1.f, 0.f, 0.f);
    m_WorldCoordinateY = glm::vec3(0.f, 1.f, 0.f);
    m_WorldCoordinateZ = glm::vec3(0.f, 0.f, 1.f);

    m_cameraPosition = position;
    m_fFieldOfView = fov;
    m_fFarPlane = farPlane;
    m_fNearPlane = nearPlane;

    m_projectionMatrix = glm::perspective(m_fFieldOfView, m_fAspectRatio, m_fNearPlane, m_fFarPlane);
    updateCameraView();
}

/**
    Updates the camera view
*/
void Camera::updateCameraView()
{
    // Set the view matrix
    m_viewMatrix = glm::mat4_cast(m_orientation);

    // Extract camera coordinates
    m_cameraPosX = m_WorldCoordinateX;
    m_cameraPosY = m_WorldCoordinateY;
    m_cameraPosZ = m_WorldCoordinateZ;

    m_cameraPosX = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[0][1], m_viewMatrix[0][2]);
    m_cameraPosY = glm::vec3(m_viewMatrix[1][0], m_viewMatrix[1][1], m_viewMatrix[1][2]);
    m_cameraPosZ = glm::vec3(m_viewMatrix[2][0], m_viewMatrix[2][1], m_viewMatrix[2][2]);

    // Translate view matrix
    m_viewMatrix[3][0] = -glm::dot(m_cameraPosX, m_cameraPosition);
    m_viewMatrix[3][1] = -glm::dot(m_cameraPosY, m_cameraPosition);
    m_viewMatrix[3][2] = -glm::dot(m_cameraPosZ, m_cameraPosition);
}

/**
    Rotates the camera
*/
void Camera::rotateCamera(const float pitch, const float yaw)
{
    glm::quat rotation = getAxisAngle(m_WorldCoordinateX, pitch);
    m_orientation = m_orientation * rotation;
    glm::normalize(m_orientation);

    rotation = getAxisAngle(m_WorldCoordinateY, yaw);
    m_orientation = m_orientation * rotation;
    glm::normalize(m_orientation);

    updateCameraView();
}

/**
    Enables camera zooming
*/
void Camera::zoom(float zoom)
{
    m_cameraPosition += (m_cameraPosY * zoom);
    updateCameraView();
}

/**
    Enables camera panning
*/
void Camera::pan(float pitch, float yaw)
{
    m_cameraPosition += (m_cameraPosX * pitch);
    m_cameraPosition += (m_cameraPosY * yaw);
    updateCameraView();
}

/**
    Enables camera rolling
*/
void Camera::roll(float roll)
{
    glm::quat rotation = getAxisAngle(m_WorldCoordinateZ, roll);
    m_orientation *= rotation;
    glm::normalize(m_orientation);
    updateCameraView();
}

/**
    Gets the camera's veiw matrix
    @return m_viewMatrix
*/
glm::mat4 Camera::getViewMatrix()
{
    return m_viewMatrix;
}

/**
    Gets the camera's projection matrix
    @return m_projectionMatrix
*/
glm::mat4 Camera::getProjectionMatrix()
{
    return m_projectionMatrix;
}

/**
    Gets the camera's axis angle.
    @param axis
    @param angle
*/
glm::quat Camera::getAxisAngle(glm::vec3 axis, float angle)
{
    glm::quat rotation;
    rotation.w = glm::cos(angle / 2);
    rotation.x = glm::sin(angle / 2) * axis.x;
    rotation.y = glm::sin(angle / 2) * axis.y;
    rotation.z = glm::sin(angle / 2) * axis.z;
    return rotation;
}