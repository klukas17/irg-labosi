//
// Created by mihael on 05.05.22..
//

#ifndef IRGLAB_BEZIER_H
#define IRGLAB_BEZIER_H

#include "vector"
#include <glm/glm.hpp>
#include <glad/glad.h>

class Bezier {
public:
    static std::vector<glm::vec3> approximation_Bezier_curve(std::vector<glm::vec3> points, int number_of_points);
    static std::vector<glm::vec3> interpolation_Bezier_curve(std::vector<glm::vec3> points, int number_of_points);
};


#endif //IRGLAB_BEZIER_H
