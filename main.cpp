
#include"Mesh.h"
#include "Model.h"

int WIDTH = 800;
int HEIGHT = 800;

struct LightControl {
    float* outerCone;
    float* innerCone;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void utility_inputs(GLFWwindow* window)
{
    if (!window) {
        std::cout << "(!) window is NULL in utility_inputs" << std::endl;
        throw(errno);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwDestroyWindow(window);
        exit(0);
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
    LightControl lightControl = {};
    glfwSetWindowUserPointer(window, &lightControl);
    glfwSetScrollCallback(window, scroll_callback);

    Shader shaderProgram("default.vert", "default.frag");    


    // Take care of all the light related things
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    shaderProgram.Activate();
    
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
   
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
   
    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    
    Model model("gltf/bunny/scene.gltf");

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

        utility_inputs(window);
        camera.Inputs(window, false);
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

        model.Draw(shaderProgram, camera);

        // Swap buffer with the from one
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
