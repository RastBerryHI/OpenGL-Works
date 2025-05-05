#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

    vao.Bind();

    // Generating Vertex Buffer and Element Buffer objects
    VBO VBO1(vertices);
    EBO EBO1(indices);

    vao.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vao.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    vao.LinkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    vao.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

}

void Mesh::Draw(Shader& shader, Camera& camera)
{
    shader.Activate();
    vao.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].GetType();

        if (type == "diffuse") {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular") {
            num = std::to_string(numSpecular++);
        }
        textures[i].SetTexUni(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
