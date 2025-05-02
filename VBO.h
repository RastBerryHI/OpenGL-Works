#include <glad/glad.h>
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* verticies, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif 