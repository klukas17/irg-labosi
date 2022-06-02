//
// Created by mihael on 3/23/22.
//

#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render() {
    for (auto obj : objects) {
        obj->mesh->model_matrix = obj->transform->getModelMatrix();
        obj->mesh->view_matrix = camera->getViewMatrix();
        obj->mesh->projection_matrix = camera->getProjectionMatrix();
        obj->mesh->eye = camera->camera_position;
        obj->mesh->render();
    }
}