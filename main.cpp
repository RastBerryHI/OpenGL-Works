
#include"Mesh.h"

int WIDTH = 800;
int HEIGHT = 800;

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    {glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};


// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};


GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};


struct LightControl {
    float* outerCone;
    float* innerCone;
};

//
//void global_terminate(GLFWwindow* window, const std::vector<VAO>& vaoObjects, const std::vector<VBO>& vboObjects, const std::vector<EBO>& eboObjects, const std::vector<Texture>& textures, const std::vector <Shader>& shaders)
//{
//    for (VAO vao : vaoObjects) {
//        vao.Delete();
//    }
//
//    for (VBO vbo : vboObjects) {
//        vbo.Delete();
//    }
//
//    for (EBO ebo : eboObjects) {
//        ebo.Delete();
//    }
//
//    for (Texture tex : textures) {
//        tex.Delete();
//    }
//
//    for (Shader shader : shaders) {
//        shader.Delete();
//    }
//
//    /*VAO1.Delete();
//    VBO1.Delete();
//    EBO1.Delete();
//
//    lightVAO.Delete();
//    lightVBO.Delete();
//    lightEBO.Delete();
//
//    brickTexture.Delete();
//    shaderProgram.Delete();*/
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void utility_inputs(GLFWwindow* window, float& ambientUniform, float& specularUniform)
{
    if (!window) {
        std::cout << "(!) window is NULL in utility_inputs" << std::endl;
        throw(errno);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwDestroyWindow(window);
        exit(0);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ambientUniform+= 0.01f;
        ambientUniform = glm::clamp(ambientUniform, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ambientUniform -= 0.01f;
        ambientUniform = glm::clamp(ambientUniform, 0.0f, 1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        specularUniform += 0.01f;
        specularUniform = glm::clamp(specularUniform, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        specularUniform -= 0.01f;
        specularUniform = glm::clamp(specularUniform, 0.0f, 1.0f);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    LightControl* control = static_cast<LightControl*>(glfwGetWindowUserPointer(window));
    if (control && control->outerCone && control->innerCone) {
        *(control->outerCone) += yoffset * 0.01f; // adjust sensitivity as needed
        *(control->innerCone) += yoffset * 0.01f;

        // Clamp values between 0.0 and 1.0
        *(control->outerCone) = glm::clamp(*(control->outerCone), 0.0f, 1.0f);
        *(control->innerCone) = glm::clamp(*(control->innerCone), 0.0f, 1.0f);

        std::cout << "Outer cone: " << *(control->outerCone)
            << ", Inner cone: " << *(control->innerCone) << '\n';
    }
}

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Texture textures[] = {
        Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    Shader shaderProgram("default.vert", "default.frag");    
    // Correcting initialization of indices and textures
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));  // Initialize vertices properly
    std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));  // Initialize indices properly
    std::vector<Texture> texs = {
        Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };  // Initialize textures properly

    Mesh floor(verts, inds, texs);  // Create the floor mesh

    Shader lightShader("light.vert", "light.frag");
    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector<GLuint> lightInds(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    Mesh light(lightVerts, lightInds, texs);


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    
    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);
    
    float ambient = 0.2f;
    float specularLight = 0.5f;

    float spotLightOuterCone = 0.90f;
    float spotLightInnerCone = 0.95f;
    
    LightControl lightControl{ &spotLightOuterCone, &spotLightInnerCone };

    glfwSetWindowUserPointer(window, &lightControl);
    glfwSetScrollCallback(window, scroll_callback);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.x, lightColor.w);
    shaderProgram.Activate();
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
   
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
   
    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height)
    {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
        if (cam)
            {
            cam->UpdateWindowProps(win, width, height);
        }
    });

    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the backgroound
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean color and depth in back buffer and assign new data to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        utility_inputs(window, ambient, specularLight);
        
        camera.Inputs(window, false);
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambient"), ambient);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularLight"), specularLight);

        glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambient"), ambient);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularLight"), specularLight);

        glUniform1f(glGetUniformLocation(shaderProgram.ID, "outerCone"), spotLightOuterCone);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "innerCone"), spotLightInnerCone);

        floor.Draw(shaderProgram, camera);
        light.Draw(lightShader, camera);

        // Swap buffer with the from one
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    glfwTerminate();

    /*const std::vector<VAO> VAOs = { VAO1, lightVAO };
    const std::vector<VBO> VBOs = { VBO1, lightVBO };
    const std::vector<EBO> EBOs = { EBO1, lightEBO };
    const std::vector <Texture> Textures = { planksTex, planksSpec};
    const std::vector <Shader> Shaders = { shaderProgram, lightShader };
    global_terminate(window, VAOs, VBOs, EBOs, Textures, Shaders);*/
    return 0;
}
