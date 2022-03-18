// Local Headers
#include "Shader.h"
#include "FPSManager.h"
#include "State.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <thread>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <iostream>

State *stanje = new State();

void framebuffer_size_callback(GLFWwindow * window, int Width, int Height) {
    stanje->width = Width;
    stanje->height = Height;

    glViewport(0, 0, stanje->width, stanje->height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_REPEAT || action == GLFW_PRESS) {
        switch(key) {
            // up arrow key
            case 265:
                stanje->povecaj_trenutnu_boju(1);
                break;
            // down arrow key
            case 264:
                stanje->smanji_trenutnu_boju(1);
                break;
            // right arrow key
            case 262:
                stanje->povecaj_trenutnu_boju(10);
                break;
            // left arrow key
            case 263:
                stanje->smanji_trenutnu_boju(10);
                break;
        }
    }

    if (action == GLFW_PRESS) {
        switch(key) {
            // r key
            case 82:
                stanje->promijeni_trenutnu_boju(0);
                break;
            // g key
            case 71:
                stanje->promijeni_trenutnu_boju(1);
                break;
            // b key
            case 66:
                stanje->promijeni_trenutnu_boju(2);
                break;
        }
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        stanje->dodaj_vrh();
    }
}

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    stanje->curr_x = x;
    stanje->curr_y = y;
    stanje->indeksirani_x = (stanje->curr_x - stanje->width/(float)2) / (stanje->width/(float)2);
    stanje->indeksirani_y = (stanje->height/(float)2 - stanje->curr_y) / (stanje->height/(float)2);
}

void napuni_polje(float* polje, bool full) {
    polje[0] = stanje->indeksirani_x;
    polje[1] = stanje->indeksirani_y;
    polje[2] = 0;
    polje[3] = stanje->indeksirani_vrhovi.rbegin()[2];
    polje[4] = stanje->indeksirani_vrhovi.rbegin()[1];
    polje[5] = 0;
    if (full) {
        polje[6] = stanje->indeksirani_vrhovi.rbegin()[5];
        polje[7] = stanje->indeksirani_vrhovi.rbegin()[4];
        polje[8] = 0;
        polje[9] = polje[0];
        polje[10] = polje[1];
        polje[11] = polje[2];
    }
}

int main(int argc, char* argv[]) {

    GLFWwindow* window;
    glfwInit();
    window = glfwCreateWindow(stanje->width, stanje->height, "Zadatak 3", nullptr, nullptr);
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

    glClearColor(0.15, 0.1, 0.1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira
    glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    GLuint VAO[3];
    GLuint VBO[5];
    GLuint EBO[2];

    glGenVertexArrays(3, VAO);
    glGenBuffers(5, VBO);
    glGenBuffers(2, EBO);

    Shader* sjencar[2];
    sjencar[0] = Shader::loadShader(argv[0], "shader");
    sjencar[1] = Shader::loadShader(argv[0], "shader_line");
    GLint lokacijaUniformVarijable = glGetUniformLocation(sjencar[1]->ID, "u_color");
    FPSManager FPSManagerObject(window, 60, 1.0, "Zadatak 3");

    float okvir_za_boju_koordinate[12] = {
        0.93, 0.93, 0,
        1, 0.93, 0,
        0.93, 1, 0,
        1, 1, 0
    };

    float okvir_za_boju_boje[12] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    unsigned int okvir_za_boju[6] = {
        0, 1, 2,
        1, 2, 3
    };

    float zadnji_vrhovi[12] = {0};

    //glavna petlja programa
    while (glfwWindowShouldClose(window) == false) {

        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (stanje->vrhovi.size() >= 3) {

            glUseProgram(sjencar[0]->ID);

            glBindVertexArray(VAO[0]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, stanje->indeksirani_vrhovi.size() * sizeof(float), &stanje->indeksirani_vrhovi.front(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, stanje->indeksirane_boje.size() * sizeof(float), &stanje->indeksirane_boje.front(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, stanje->indeksi.size() * sizeof(unsigned int), &stanje->indeksi.front(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, stanje->indeksi.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        okvir_za_boju_boje[0] = okvir_za_boju_boje[3] = okvir_za_boju_boje[6] = okvir_za_boju_boje[9] = stanje->red_color / (float)255;
        okvir_za_boju_boje[1] = okvir_za_boju_boje[4] = okvir_za_boju_boje[7] = okvir_za_boju_boje[10] = stanje->green_color / (float)255;
        okvir_za_boju_boje[2] = okvir_za_boju_boje[5] = okvir_za_boju_boje[8] = okvir_za_boju_boje[11] = stanje->blue_color / (float)255;

        glUseProgram(sjencar[0]->ID);

        glBindVertexArray(VAO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(okvir_za_boju_koordinate), okvir_za_boju_koordinate, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(okvir_za_boju_boje), okvir_za_boju_boje, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(okvir_za_boju), (void*)&okvir_za_boju[0], GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, sizeof(okvir_za_boju), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        int broj_vrhova = stanje->vrhovi.size();

        if (broj_vrhova >= 1) {
            napuni_polje(zadnji_vrhovi, (broj_vrhova == 1 ? false : true));

            glUseProgram(sjencar[1]->ID);
            glUniform3f(lokacijaUniformVarijable, stanje->red_color / (double)255, stanje->green_color / (double) 255, stanje->blue_color / (double) 255);

            glBindVertexArray(VAO[2]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
            glBufferData(GL_ARRAY_BUFFER, (broj_vrhova == 1 ? 6 : 12) * sizeof(float), zadnji_vrhovi, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glEnableVertexAttribArray(0);

            glDrawArrays(GL_LINE_STRIP, 0, (broj_vrhova == 1 ? 2 : 4));
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete sjencar[0];
    delete sjencar[1];

    glDeleteBuffers(5, VBO);
    glDeleteBuffers(2, EBO);
    glDeleteVertexArrays(3, VAO);

    glfwTerminate();

    delete stanje;

    return EXIT_SUCCESS;
}