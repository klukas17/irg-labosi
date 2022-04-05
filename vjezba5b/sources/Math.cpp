//
// Created by mihael on 4/2/22.
//

#include "Math.h"

glm::mat4 Math::translate(glm::vec3 translate_vector) {

    glm::mat4 translation_matrix = glm::mat4(1);
    translation_matrix[3][0] = translate_vector.x;
    translation_matrix[3][1] = translate_vector.y;
    translation_matrix[3][2] = translate_vector.z;

    return translation_matrix;
}

glm::mat4 Math::scale(glm::vec3 scale_vector) {

    glm::mat4 scale_matrix = glm::mat4(1);
    scale_matrix[0][0] = scale_vector.x;
    scale_matrix[1][1] = scale_vector.y;
    scale_matrix[2][2] = scale_vector.z;

    return scale_matrix;
}

glm::mat4 Math::look_at(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {

    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 u = glm::normalize(up);
    glm::vec3 s = glm::normalize(glm::cross(f, u));
    u = glm::cross(s, f);

    glm::mat4 look_at_matrix = glm::mat4(1);

    look_at_matrix[0][0] = s.x;
    look_at_matrix[1][0] = s.y;
    look_at_matrix[2][0] = s.z;
    look_at_matrix[0][1] = u.x;
    look_at_matrix[1][1] = u.y;
    look_at_matrix[2][1] = u.z;
    look_at_matrix[0][2] = -f.x;
    look_at_matrix[1][2] = -f.y;
    look_at_matrix[2][2] = -f.z;
    look_at_matrix[3][0] = -glm::dot(s, eye);
    look_at_matrix[3][1] = -glm::dot(u, eye);
    look_at_matrix[3][2] = glm::dot(f, eye);

    return look_at_matrix;
}

glm::mat4 Math::frustum(float l, float r, float b, float t, float n, float f) {

    glm::mat4 frustum_matrix = glm::mat4(0);

    frustum_matrix[0][0] = 2 * n / (r - l);
    frustum_matrix[1][1] = 2 * n / (t - b);
    frustum_matrix[2][0] = (r + l) / (r - l);
    frustum_matrix[2][1] = (t + b) / (t - b);
    frustum_matrix[2][2] = - (f + n) / (f - n);
    frustum_matrix[2][3] = -1;
    frustum_matrix[3][2] = -2 * f * n / (f - n);

    return frustum_matrix;
}