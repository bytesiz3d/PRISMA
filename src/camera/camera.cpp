#include "Camera.hpp"

Camera::Camera()
    : yaw(0), pitch(0), yawSens(0.001), pitchSens(0.001), movementSens(0.1), PI(glm::pi<float>())
{
    position = glm::vec3(0, 0, 0);
    direction = glm::vec3(0, 0, 1);
    up = glm::vec3(0, 1, 0);

    perspectiveFoVy = PI / 2;
    aspectRatio = 1;

    near = 0.01;
    far = 1000;

    // near = 0.0001;
    // far = 100000;

    auto dir = direction;
    auto len = dir[0] * dir[0];
    len += dir[1] * dir[1];
    len = std::sqrt(len);

    yaw = std::atan2(dir[2], dir[0]);
    pitch = std::atan2(dir[1], len);
}

void Camera::UpdateCamera(glm::vec2 mouseDelta, glm::vec3 movement)
{
    yaw += mouseDelta[0] * yawSens;
    pitch += -mouseDelta[1] * pitchSens;

    pitch = glm::min(PI / 2, glm::max(-PI / 2, pitch));
    direction = glm::vec3(std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch));

    movement *= movementSens;

    position += direction * movement[2];
    position += right() * movement[0];
    position += up * movement[1];
}

void Camera::UpdateCamera2D(glm::vec2 mouseDelta, glm::vec3 movement)
{
    yaw += mouseDelta[0] * yawSens;
    pitch += -mouseDelta[1] * pitchSens;

    pitch = glm::min(PI / 2, glm::max(-PI / 2, pitch));
    direction = glm::vec3(std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch));

    movement *= movementSens;

    glm::vec3 forward = direction;
    forward[1] = 0;
    forward = glm::normalize(forward);

    glm::vec3 normal = glm::cross(forward, glm::vec3(0, 1, 0));
    normal[1] = 0;
    normal = glm::normalize(normal);
    
    position += forward * movement[2];
    position += normal * movement[0];
    position += glm::vec3(0, 1, 0) * movement[1];
}

glm::mat4 Camera::ViewMatrix()
{
    glm::vec3 center = position + direction;
    return glm::lookAt(position, center, up);
}

glm::mat4 Camera::ProjectionMatrix()
{
    return glm::perspective(perspectiveFoVy, aspectRatio, near, far);
}

glm::mat4 Camera::ViewProjectionMatrix()
{
    return ProjectionMatrix() * ViewMatrix();
}

void Camera::SetTarget(glm::vec3 target)
{
    direction = target - position;
    direction = glm::normalize(direction);

    // Adjust pitch and yaw:
    auto dir = direction;
    auto len = dir[0] * dir[0];
    len += dir[1] * dir[1];
    len = std::sqrt(len);

    yaw = std::atan2(dir[2], dir[0]);
    pitch = std::atan2(dir[1], len);
}

glm::vec3 Camera::right()
{
    glm::vec3 n_up = glm::normalize(up);
    return glm::cross(direction, n_up);
}

