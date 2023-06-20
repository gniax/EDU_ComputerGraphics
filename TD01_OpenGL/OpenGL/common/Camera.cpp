#include "Camera.h"
#include <gtc/matrix_transform.hpp>

Camera::Camera()
    : position(0.0f, 0.0f, 3.0f), UP(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f), fov(45.0f)
{
    updateViewMatrix();
}

void Camera::updateViewMatrix()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    viewMatrix = glm::lookAt(position, position + front, UP);
}

glm::vec3 Camera::getFront() const
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}

glm::vec3 Camera::getRight() const
{
    return glm::normalize(glm::cross(getFront(), UP));
}

glm::mat4 Camera::getProjectionMatrix(int width, int height) {
    // utiliser le fov pour la perspective matrix
    return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
}

void Camera::zoom(float pAmount)
{
    fov -= pAmount;
    fov = glm::clamp(fov, 1.0f, 45.0f); // limite le fov entre 1 et 45 deg
}

float Camera::getPitch() const
{
    return pitch;
}

float Camera::getYaw() const
{
    return yaw;
}

float Camera::getFov() const
{
    return fov;
}

void Camera::setPitch(float pPitch)
{
    pitch = pPitch;
}

void Camera::setYaw(float pYaw)
{
    yaw = pYaw;
}

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(const glm::vec3& newPosition)
{
    position = newPosition;
    updateViewMatrix();
}

void Camera::move(const glm::vec3& offsetPos)
{
    position += offsetPos;
    updateViewMatrix();
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch += deltaPitch;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateViewMatrix();
}
