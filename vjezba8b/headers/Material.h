//
// Created by mihael on 09.05.22..
//

#ifndef IRGLAB_MATERIAL_H
#define IRGLAB_MATERIAL_H

#include <glm/glm.hpp>
#include <glad/glad.h>

class Material {
public:
    glm::vec3 k_a;
    glm::vec3 k_d;
    glm::vec3 k_s;
    float k_sn;

    Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ksn);
    ~Material();
};


#endif //IRGLAB_MATERIAL_H
