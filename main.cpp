
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "ShaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

int WIDTH = 800;
int HEIGHT = 800;



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
    /*      [COORDINATES]           [COLORS]                       */
       -0.5f,  -0.5f,   0.0f,   1.0f, 0.0f, 0.0f,       0.0f,  0.0f,   // Lower left corner
       -0.5f,   0.5f,   0.0f,   0.0f, 1.0f, 0.0f,       0.0f,  1.0f,   // Upper left corner
        0.5f,   0.5f,   0.0f,   0.0f, 0.0f, 1.0f,       1.0f,  1.0f,   // Upper right corner
        0.5f,  -0.5f,   0.0f,   1.0f, 1.0f, 1.0f,       1.0f,  0.0f,   // Lower left corner
    };

    GLuint indicies[] = {
        0, 2, 1,    // Upper triangle
        0, 3, 2     // Lower triangle
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

    // Getting ID of scale uniform variable, to access it's value
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    int widthImg, heightImg, numColCh;
    unsigned char* bytes = stbi_load("UkraineTexture.jpg", &widthImg, &heightImg, &numColCh, 0);

    GLuint textures;
    glGenTextures(1, &textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // float flatColor[] = {1,1,1,1};
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();

    // 0 - texture slot (GL_TEXTURE0)
    glUniform1i(tex0Uni, 0);

    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the backgroound
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean back buffer and assign new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Setting scale to 0.5f
        glUniform1f(uniID, 0.5f);
        glBindTexture(GL_TEXTURE_2D, textures);

        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw the triangle using GL_TRIANGLES primitive
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap buffer with the from one
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &textures);
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
