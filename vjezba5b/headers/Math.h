//
// Created by mihael on 4/2/22.
//

#ifndef IRGLAB_MATH_H
#define IRGLAB_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Math {
public:
    static glm::mat4 translate(glm::vec3 translate_vector);
    static glm::mat4 scale(glm::vec3 scale_vector);
    static glm::mat4 look_at(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    static glm::mat4 frustum(float l, float r, float b, float t, float n, float f);
};


#endif //IRGLAB_MATH_H
