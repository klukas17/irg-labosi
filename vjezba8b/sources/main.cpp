//
// Created by mihael on 3/23/22.
//

// Local Headers
#include "Renderer.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include <chrono>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int width = 1000, height = 1000;
glm::vec3 color = glm::vec3(0, 0, 0);

float camera_speed = 0.05;
float last_x = width/2;
float last_y = height/2;
float sensitivity = 0.03;
bool mouse_moved = false;
bool shift_pressed = false;
float shift_factor = 1;

Renderer* renderer = nullptr;

void framebuffer_size_callback(GLFWwindow * window, int Width, int Height) {
    width = Width;
    height = Height;

    if (width == height) {
        glViewport(0, 0, width, height);
    }
    else if (width > height) {
        glViewport((width - height) / 2, 0, height, height);
    }
    else if (width < height) {
        glViewport(0, (height - width) / 2, width, width);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key != GLFW_KEY_LEFT_SHIFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        Camera* camera = renderer->camera;
        switch(key) {
            case GLFW_KEY_W:
                camera->camera_position += camera->camera_front * camera_speed;
                break;

            case GLFW_KEY_S:
                camera->camera_position -= camera->camera_front * camera_speed;
                break;

            case GLFW_KEY_A:
                camera->camera_position -= glm::normalize(glm::cross(camera->camera_front, camera->camera_up)) * camera_speed;
                break;

            case GLFW_KEY_D:
                camera->camera_position += glm::normalize(glm::cross(camera->camera_front, camera->camera_up)) * camera_speed;
                break;

            case GLFW_KEY_E:
                camera->camera_position -= glm::normalize(camera->camera_up) * camera_speed;
                break;

            case GLFW_KEY_Q:
                camera->camera_position += glm::normalize(camera->camera_up) * camera_speed;
                break;
        }
    }

    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS)
            shift_pressed = true;
        else
            shift_pressed = false;
        shift_factor = shift_pressed ? 3 : 1;
    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {

    if (!mouse_moved) {
        mouse_moved = true;
        return;
    }

    float x_offset = x - last_x;
    float y_offset = last_y - y;
    last_x = x;
    last_y = y;

    x_offset *= sensitivity * shift_factor;
    y_offset *= sensitivity * shift_factor;

    Camera* camera = renderer->camera;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));;
    direction.y = sin(glm::radians(camera->pitch));
    direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));

    camera->camera_front = glm::normalize(direction);
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;
    glfwInit();
    window = glfwCreateWindow(width, height, "Zadatak 8b", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD");
        exit(-1);
    }
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glClearColor(0.3, 0.3, 0.3, 1); //boja brisanja platna izmedu iscrtavanja dva okvira
    glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    Assimp::Importer importer;

    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("/resources"); //za linux pretvoriti u forwardslash
    std::string objPath(resPath);
    objPath.append("/glava/glava.obj"); //za linux pretvoriti u forwardslash

    const aiScene* scene = importer.ReadFile(objPath.c_str(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    if (!scene) {
        std::cerr << importer.GetErrorString();
        return false;
    }

    if (scene->HasMeshes() && scene->HasMaterials()) {

        renderer = new Renderer();
        Camera* camera = new Camera();
        renderer->camera = camera;

        Light* light = new Light(glm::vec3(0, 0, 5), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1, 1, 1));
        renderer->light = light;

        Material* material = nullptr;
        for (int i = 0; i < scene->mNumMaterials; i++) {
            aiString name;
            scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
            if (strcmp(name.C_Str(), "Body") != 0) continue;

            float shininess;
            aiColor3D ambientK, diffuseK, specularK;

            scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess);
            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);

            material = new Material(glm::vec3(ambientK.r, ambientK.g, ambientK.b), glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b), glm::vec3(specularK.r, specularK.g, specularK.b), shininess);

            std::cout << "k_a " << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;
            std::cout << "k_d " << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;
            std::cout << "k_s " << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;
        }

        auto m = scene->mMeshes[0];
        std::vector<glm::vec4> v;
        std::vector<unsigned int> f;
        for (int i = 0; i < m->mNumVertices; i++)
            v.push_back({m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z, 1.0f});

        std::vector<glm::vec3> n;
        for (int i = 0; i < m->mNumVertices; i++)
            n.push_back({m->mNormals[i].x, m->mNormals[i].y, m->mNormals[i].z});

        for (int i = 0; i < m->mNumFaces; i++)
            for (int j = 0; j < m->mFaces[i].mNumIndices; j++)
                f.push_back(m->mFaces[i].mIndices[j]);

        Shader* shader = Shader::loadShader(argv[0], "shader", false);
        Mesh* mesh = new Mesh(v, n, f, shader);
        Transform* transform = new Transform(mesh);
        Object* object = new Object(mesh, transform, material);

        renderer->objects.push_back(object);

        for (auto obj : renderer->objects) {
            obj->normalize();
            obj->buffer_data();

            obj->mesh->I_a = renderer->light->I_a;
            obj->mesh->I_d = renderer->light->I_d;
            obj->mesh->I_s = renderer->light->I_s;
            obj->mesh->light_location = renderer->light->location;
        }

        transform->setSize(1);
        transform->setPosition(glm::vec3(0, 0, -3.0f));
    }

    srand(time(NULL));

    while (glfwWindowShouldClose(window) == false) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return EXIT_SUCCESS;
}