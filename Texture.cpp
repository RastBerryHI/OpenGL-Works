
#include "Texture.h"
#include "ShaderClass.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
    type = texType;

    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE + slot);
    textureSlot = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // float flatColor[] = {1,1,1,1};
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    if (numColCh == 4) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if (numColCh == 3) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if (numColCh == 1) {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else {
        throw std::invalid_argument("(!) Auto texture type recognition failed");
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetTexUni(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();

    // unit - texture slot GL_TEXTURENUMBER
    glUniform1i(tex0Uni, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

GLuint Texture::GetID() const
{
    return ID;
}

GLuint Texture::GetSlot() const
{
    return textureSlot;
}

const char* Texture::GetType() const
{
    return type;
}
