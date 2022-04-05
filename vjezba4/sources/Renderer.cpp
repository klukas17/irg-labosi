//
// Created by mihael on 3/23/22.
//

#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render() {
    for (auto obj : objects)
        obj->mesh->render();
}