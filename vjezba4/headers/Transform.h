//
// Created by mihael on 3/24/22.
//

#ifndef IRGLAB_TRANSFORM_H
#define IRGLAB_TRANSFORM_H

#include "Mesh.h"

class Transform {
public:
    Mesh* mesh;

    Transform(Mesh* m);
    ~Transform();

    void translate();
    void scale();
};


#endif //IRGLAB_TRANSFORM_H
