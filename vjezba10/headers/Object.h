//
// Created by mihael on 3/23/22.
//

#ifndef IRGLAB_OBJECT_H
#define IRGLAB_OBJECT_H

#include <glad/glad.h>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "Texture.h"

class Object {
public:
    Mesh* mesh;
    Transform* transform;
    Material* material;
    Texture* texture;

    Object(Mesh* m, Transform* t, Material* mat, Texture* tex);
    ~Object();

    void normalize();
    void buffer_data();
};


#endif //IRGLAB_OBJECT_H
