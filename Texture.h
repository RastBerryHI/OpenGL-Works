#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb_image.h>

class Shader;

class Texture
{
	GLenum type;
	GLuint ID;
	GLuint textureSlot;

public:
	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void SetTexUni(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	GLuint GetID() const;
	GLuint GetSlot() const;
};

#endif

