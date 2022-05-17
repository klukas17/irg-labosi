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
    window = glfwCreateWindow(width, height, "Zadatak 6", nullptr, nullptr);
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

    glClearColor(1, 1, 1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira
    glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    Assimp::Importer importer;

    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("/resources"); //za linux pretvoriti u forwardslash
    std::string objPath(resPath);
    objPath.append("/kocka/kocka.obj"); //za linux pretvoriti u forwardslash

    const aiScene* scene = importer.ReadFile(objPath.c_str(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    if (!scene) {
        std::cerr << importer.GetErrorString();
        return false;
    }

    if (scene->HasMeshes()) {

        renderer = new Renderer();
        Camera* camera = new Camera();
        renderer->camera = camera;

        auto m = scene->mMeshes[0];
        std::vector<glm::vec4> v;
        std::vector<unsigned int> f;
        for (int i = 0; i < m->mNumVertices; i++)
            v.push_back({m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z, 1.0f});

        for (int i = 0; i < m->mNumFaces; i++)
            for (int j = 0; j < m->mFaces[i].mNumIndices; j++)
                f.push_back(m->mFaces[i].mIndices[j]);

        Shader* shader1 = Shader::loadShader(argv[0], "shader1");
        GLint uniform_variable_location_color1 = glGetUniformLocation(shader1->ID, "u_color");
        GLint uniform_variable_location_model_matrix1 = glGetUniformLocation(shader1->ID, "model_matrix");
        GLint uniform_variable_location_view_matrix1 = glGetUniformLocation(shader1->ID, "view_matrix");
        GLint uniform_variable_location_projection_matrix1 = glGetUniformLocation(shader1->ID, "projection_matrix");
        GLint uniform_variable_location_eye1 = glGetUniformLocation(shader1->ID, "eye");

        Mesh* mesh1 = new Mesh(v, f, shader1->ID, uniform_variable_location_color1, color, uniform_variable_location_model_matrix1, uniform_variable_location_view_matrix1, uniform_variable_location_projection_matrix1, uniform_variable_location_eye1);
        Transform* transform1 = new Transform(mesh1);
        Object* object1 = new Object(mesh1, transform1);

        Shader* shader2 = Shader::loadShader(argv[0], "shader2");
        GLint uniform_variable_location_color2 = glGetUniformLocation(shader2->ID, "u_color");
        GLint uniform_variable_location_model_matrix2 = glGetUniformLocation(shader2->ID, "model_matrix");
        GLint uniform_variable_location_view_matrix2 = glGetUniformLocation(shader2->ID, "view_matrix");
        GLint uniform_variable_location_projection_matrix2 = glGetUniformLocation(shader2->ID, "projection_matrix");
        GLint uniform_variable_location_eye2 = glGetUniformLocation(shader2->ID, "eye");

        Mesh* mesh2 = new Mesh(v, f, shader2->ID, uniform_variable_location_color2, color, uniform_variable_location_model_matrix2, uniform_variable_location_view_matrix2, uniform_variable_location_projection_matrix2, uniform_variable_location_eye2);
        Transform* transform2 = new Transform(mesh2);
        Object* object2 = new Object(mesh2, transform2);

        Shader* shader3 = Shader::loadShader(argv[0], "shader3");
        GLint uniform_variable_location_color3 = glGetUniformLocation(shader3->ID, "u_color");
        GLint uniform_variable_location_model_matrix3 = glGetUniformLocation(shader3->ID, "model_matrix");
        GLint uniform_variable_location_view_matrix3 = glGetUniformLocation(shader3->ID, "view_matrix");
        GLint uniform_variable_location_projection_matrix3 = glGetUniformLocation(shader3->ID, "projection_matrix");

        Mesh* mesh3 = new Mesh(v, f, shader3->ID, uniform_variable_location_color3, color, uniform_variable_location_model_matrix3, uniform_variable_location_view_matrix3, uniform_variable_location_projection_matrix3, 0);
        Transform* transform3 = new Transform(mesh3);
        Object* object3 = new Object(mesh3, transform3);

        renderer->objects.push_back(object1);
        renderer->objects.push_back(object2);
        renderer->objects.push_back(object3);


        for (auto obj : renderer->objects) {
            obj->normalize();
            obj->buffer_data();
        }

        transform1->setSize(0.5);
        transform1->setPosition(glm::vec3(-1.5, 0, -5.0f));

        transform2->setSize(0.5);
        transform2->setPosition(glm::vec3(0, 0, -5.0f));

        transform3->setSize(0.5);
        transform3->setPosition(glm::vec3(1.5, 0, -5.0f));
    }

    while (glfwWindowShouldClose(window) == false) {

        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return EXIT_SUCCESS;
}