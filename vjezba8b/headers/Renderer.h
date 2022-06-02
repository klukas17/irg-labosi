//
// Created by mihael on 3/23/22.
//

#ifndef IRGLAB_RENDERER_H
#define IRGLAB_RENDERER_H

#include <vector>
#include "Object.h"
#include "Camera.h"
#include "Light.h"

class Renderer {
public:
    std::vector<Object*> objects;
    Camera* camera;
    Light* light;

    Renderer();
    ~Renderer();
    void render();
};


#endif //IRGLAB_RENDERER_H
