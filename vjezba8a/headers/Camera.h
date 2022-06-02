//
// Created by mihael on 3/31/22.
//

#ifndef IRGLAB_CAMERA_H
#define IRGLAB_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 camera_position;
    glm::vec3 camera_direction;
    glm::vec3 up;
    glm::vec3 camera_right;
    glm::vec3 camera_up;
    glm::vec3 camera_front;

    float pitch;
    float yaw;

    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

};


#endif //IRGLAB_CAMERA_H
