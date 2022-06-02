//
// Created by mihael on 02.06.22..
//

#ifndef IRGLAB_SHADOWMAP_H
#define IRGLAB_SHADOWMAP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
public:
    unsigned int depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1000;
    const unsigned int SHADOW_HEIGHT = 1000;
    unsigned int depth_map;

    float near_plane = 0.01f, far_plane = 50.0f;
    glm::mat4 light_projection_matrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
    glm::mat4 light_view_matrix = glm::lookAt(glm::vec3( -1.5f, 7.0f, 14.0f),glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3( 0.0f, 1.0f,  0.0f));

    ShadowMap();
    ~ShadowMap();
};


#endif //IRGLAB_SHADOWMAP_H
