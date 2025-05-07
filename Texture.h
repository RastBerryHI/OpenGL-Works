#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb_image.h>

class Shader;

class Texture
{
	const char* type;
	GLuint ID;
	GLuint textureSlot;

public:
	Texture(const char* image, const char* texType, GLuint slot);

	void SetTexUni(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	GLuint GetID() const;
	GLuint GetSlot() const;
	const char* GetType() const;
};

#endif

