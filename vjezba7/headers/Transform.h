//
// Created by mihael on 3/24/22.
//

#ifndef IRGLAB_TRANSFORM_H
#define IRGLAB_TRANSFORM_H

#include "Mesh.h"

class Transform {
public:
    Mesh* mesh;
    glm::vec3 position;
    glm::vec3 up_vector;
    float angle;
    float size;

    Transform(Mesh* m);
    ~Transform();

    void translate();
    void scale();

    glm::mat4 getModelMatrix();

    void setSize(float s);
    void setOrientation(glm::vec3 up_v, float a);
    void setPosition(glm::vec3 p);
};


#endif //IRGLAB_TRANSFORM_H
