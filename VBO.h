#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>


// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;


	// Full constructor
	Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec3 col, glm::vec2 uv)
		: position(pos), normal(norm), color(col), texUV(uv) {
	}

	// Minimal constructor (for lightVertices)
	Vertex(glm::vec3 pos)
		: position(pos), normal(glm::vec3(0.0f)), color(glm::vec3(1.0f)), texUV(glm::vec2(0.0f)) {
	}


};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& verticies);

	void Bind();
	void Unbind();
	void Delete();
};

#endif 