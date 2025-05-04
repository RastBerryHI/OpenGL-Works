
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
    Texture ukraineFlagTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    ukraineFlagTex.SetTexUni(shaderProgram, "tex0", GL_TEXTURE0);
   
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the backgroound
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean back buffer and assign new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        int projlLoc = glGetUniformLocation(shaderProgram.ID, "proj");        

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        // Move whole world in derection of vec3
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        // Bounds of closest and farest point can be seen
        proj = glm::perspective(glm::radians(45.f), (float)(WIDTH / HEIGHT), 0.1f, 100.f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projlLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Binding texture to render
        ukraineFlagTex.Bind();
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
    ukraineFlagTex.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
