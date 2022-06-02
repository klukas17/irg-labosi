//
// Created by mihael on 09.05.22..
//

#include "Light.h"

Light::Light(glm::vec3 l, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is) {
    location = l;
    I_a = Ia;
    I_d = Id;
    I_s = Is;
}

Light::~Light() {}