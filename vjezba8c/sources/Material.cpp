//
// Created by mihael on 09.05.22..
//

#include "Material.h"
#include "iostream"

Material::Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ksn) {
    k_a = ka;
    k_d = kd;
    k_s = ks;
    k_sn = ksn;
}

Material::~Material() {}