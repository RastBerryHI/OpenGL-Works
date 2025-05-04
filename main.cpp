
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "ShaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"
#include "Camera.h"

const int WIDTH = 800;
const int HEIGHT = 800;

int main(int argc, char* argv[])
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Works", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, WIDTH, HEIGHT);

    GLfloat verticies[] = {
    /*      [COORDINATES]           [COLORS]         [TEXTURE COORDINATS]       */
       -0.5f,   0.0f,   0.5f,   0.83f, 0.7f,  0.44f,    0.0f,  0.0f,
       -0.5f,   0.0f,  -0.5f,   0.83f, 0.7f,  0.44f,    5.0f,  0.0f,
        0.5f,   0.0f,  -0.5f,   0.83f, 0.7f, 0.44f,    0.0f,  0.0f,
        0.5f,   0.0f,   0.5f,   0.83f, 0.7f, 0.44f,    5.0f,  0.0f,
        0.0f,   0.8f,   0.0f,   0.92f, 0.86f, 0.76f,    2.5f,  5.0f
    };

    GLuint indicies[] = {
        0,  1,  2,    
        0,  2,  3,
        0,  1,  4,
        1,  2,  4,
        2,  3,  4,
        3,  0,  4
    };

    Shader shaderProgram("default.vert", "default.frag");    

    VAO VAO1;
    VAO1.Bind();

    // Generating Vertex Buffer and Element Buffer objects
    VBO VBO1(verticies, sizeof(verticies));
    EBO EBO1(indicies, sizeof(indicies));

    // Linking VBO to VAO

    // Linking vertex shader position attributes
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // Linking vertex shader color attributes. Applying read offset to get color data from VBO
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // Linking vertex shader texture attributes. Applying read offset to get texture data from VBO
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Loading Texture
    Texture brickTexture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTexture.SetTexUni(shaderProgram, "tex0", GL_TEXTURE0);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the backgroound
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean color and depth in back buffer and assign new data to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Binding texture to render
        brickTexture.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw the triangle using GL_TRIANGLES primitive
        glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap buffer with the from one
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brickTexture.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
