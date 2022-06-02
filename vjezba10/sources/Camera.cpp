//
// Created by mihael on 3/31/22.
//

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    camera_position = glm::vec3(0.0f, 10.0f, 15.0f);
    camera_direction = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_right = glm::normalize(glm::cross(up, camera_direction));
    camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
    camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

    pitch = 0.0f;
    yaw = -90.0f;
}
Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(camera_position, camera_position + camera_front, camera_up);
}

glm::mat4 Camera::getProjectionMatrix() {
    //return glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
    return glm::perspective(glm::radians(45.0f), 1.0f, 0.01f, 100.0f);
}