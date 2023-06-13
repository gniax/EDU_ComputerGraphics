#pragma once

#include <glm.hpp>

class Camera {
public:
    Camera();

    // Met � jour la matrice de vue en utilisant les valeurs actuelles de position de la cam�ra, direction de la cam�ra, vecteur haut.
    void updateViewMatrix();

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getRight() const;
    float getPitch();
    float getYaw();
    void setYaw(float pYaw);
    void setPitch(float pPitch);
    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& offsetPos);
    void rotate(float yaw, float pitch); // en degr�s

private:
    glm::mat4 viewMatrix;
    glm::vec3 position;
    float pitch; // rotation autour de l'axe X
    float yaw; // rotation autour de l'axe Y
    const glm::vec3 UP;
};