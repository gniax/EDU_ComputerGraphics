#pragma once

#include <glm.hpp>

class Camera {
public:
    Camera();

    // Met à jour la matrice de vue en utilisant les valeurs actuelles de position de la caméra
    void updateViewMatrix();

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getRight() const;
    float getPitch() const;
    float getYaw() const;
    float getFov() const;
    void setYaw(float pYaw);
    void setPitch(float pPitch);
    void zoom(float pAmount);
    glm::mat4 getProjectionMatrix(int pWidth, int pHeight);
    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& offsetPos);
    void rotate(float yaw, float pitch); // en degrés

private:
    float fov;
    glm::mat4 viewMatrix;
    glm::vec3 position;
    float pitch; // rotation autour de l'axe X
    float yaw; // rotation autour de l'axe Y
    const glm::vec3 UP;
};