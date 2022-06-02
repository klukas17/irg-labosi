//
// Created by mihael on 09.05.22..
//

#ifndef IRGLAB_LIGHT_H
#define IRGLAB_LIGHT_H

#include <glm/glm.hpp>
#include <glad/glad.h>

class Light {
public:
    glm::vec3 location;
    glm::vec3 I_a;
    glm::vec3 I_d;
    glm::vec3 I_s;

    Light(glm::vec3 l, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is);
    ~Light();
};


#endif //IRGLAB_LIGHT_H
