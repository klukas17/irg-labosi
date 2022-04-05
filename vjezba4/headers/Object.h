//
// Created by mihael on 3/23/22.
//

#ifndef IRGLAB_OBJECT_H
#define IRGLAB_OBJECT_H

#include <glad/glad.h>
#include "Mesh.h"
#include "Transform.h"

class Object {
public:
    Mesh* mesh;
    Transform* transform;

    Object(Mesh* m, Transform* t);
    ~Object();
};


#endif //IRGLAB_OBJECT_H
