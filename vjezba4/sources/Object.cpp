//
// Created by mihael on 3/23/22.
//

#include "Object.h"
#include "Mesh.h"
#include "Transform.h"

Object::Object(Mesh* m, Transform* t) {
    mesh = m;
    transform = t;
}
Object::~Object() {}