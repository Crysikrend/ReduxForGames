#include "Texture.h"

Texture::Texture(const std::string& path, int width, int height)
	:_path(path), _width(width), _height(height) {}

Texture::~Texture() {
	release();
}

GLuint Texture::getTexture() const {
	return _texture;
}

void Texture::bind() {
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLenum OGLerror = glGetError();
	if (OGLerror != GL_NO_ERROR) {
		printf("OpenGL error in Texture::bind() - OpenglError: %s\n", glewGetErrorString(OGLerror));
	}

	unsigned char* image = SOIL_load_image(_path.c_str(), &_width, &_height, 0, SOIL_LOAD_RGB);

	if (SOIL_last_result() != "Image loaded")
		printf("SOIL Error: '%s'", SOIL_last_result());

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::release() {

}