#pragma once
#include <cmath>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

class Camera
{
    float yaw;
    float pitch;

    float yawSens;
    float pitchSens;
    float movementSens;

    glm::vec3 direction;
    glm::vec3 up;

    float perspectiveFoVy;

    float near;
    float far;

    const float PI;

    glm::mat4 ViewMatrix();
    glm::mat4 ProjectionMatrix();

public:
    float aspectRatio;
    glm::vec3 position;

    Camera();

    void UpdateCamera(glm::vec2 mouseDelta, glm::vec3 movement);

    glm::mat4 ViewProjectionMatrix();

    void SetTarget(glm::vec3 target);

    glm::vec3 right();
};

