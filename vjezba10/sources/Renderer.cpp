//
// Created by mihael on 3/23/22.
//

#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render(int pass) {

    if (pass == 1) {

        for (auto obj : objects) {
            obj->mesh->model_matrix = obj->transform->getModelMatrix();
            obj->mesh->light_view_matrix = shadow_map->light_view_matrix;
            obj->mesh->light_projection_matrix = shadow_map->light_projection_matrix;
            obj->mesh->eye = glm::vec3(-2.0f, 4.0f, -1.0f);
            obj->mesh->render(pass);
        }
    }

    else if (pass == 2) {

        for (auto obj : objects) {
            obj->mesh->model_matrix = obj->transform->getModelMatrix();
            obj->mesh->view_matrix = camera->getViewMatrix();
            obj->mesh->projection_matrix = camera->getProjectionMatrix();
            obj->mesh->light_view_matrix = shadow_map->light_view_matrix;
            obj->mesh->light_projection_matrix = shadow_map->light_projection_matrix;
            obj->mesh->eye = camera->camera_position;
            obj->mesh->texture_shadow = shadow_map->depth_map;
            obj->mesh->render(pass);
        }
    }

}