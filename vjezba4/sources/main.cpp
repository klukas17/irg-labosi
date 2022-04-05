//
// Created by mihael on 3/23/22.
//

// Local Headers
#include "Renderer.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

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


int main(int argc, char* argv[]) {
    GLFWwindow* window;
    glfwInit();
    window = glfwCreateWindow(width, height, "Zadatak 4", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

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

    Assimp::Importer importer;

    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("/resources"); //za linux pretvoriti u forwardslash
    std::string objPath(resPath);
    objPath.append("/teddy/teddy.obj"); //za linux pretvoriti u forwardslash

    const aiScene* scene = importer.ReadFile(objPath.c_str(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    if (!scene) {
        std::cerr << importer.GetErrorString();
        return false;
    }

    Renderer* renderer = nullptr;

    if (scene->HasMeshes()) {

        renderer = new Renderer();

        auto m = scene->mMeshes[0];
        std::vector<glm::vec4> v;
        std::vector<unsigned int> f;
        for (int i = 0; i < m->mNumVertices; i++)
            v.push_back({m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z, 1.0f});

        for (int i = 0; i < m->mNumFaces; i++)
            for (int j = 0; j < m->mFaces[i].mNumIndices; j++)
                f.push_back(m->mFaces[i].mIndices[j]);

        Shader* shader = Shader::loadShader(argv[0], "shader");
        GLint uniform_variable_location = glGetUniformLocation(shader->ID, "u_color");
        Mesh* mesh = new Mesh(v, f, shader->ID, uniform_variable_location, color);
        Transform* transform = new Transform(mesh);
        Object* object = new Object(mesh, transform);

        renderer->objects.push_back(object);

        renderer->objects[0]->mesh->calculate_bounding_box();
        renderer->objects[0]->transform->translate();
        renderer->objects[0]->transform->scale();

    }

    while (glfwWindowShouldClose(window) == false) {

        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        renderer->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return EXIT_SUCCESS;
}