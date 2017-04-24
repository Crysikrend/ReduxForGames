#ifndef TEXTUREH
#define TEXTUREH

#if defined(__linux__)
/* Linux */
#include <SDL2/SDL.h>
#else
/* Windows/else */
#include <SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL_opengl.h>
#endif

#include <stdio.h>
#include <fstream>
#include <iostream>

#include <SOIL.h>

class Texture {
public:
	Texture(const std::string& path, int width, int height);
	~Texture();

	void release();

	void bind();
	GLuint getTexture() const;

private:
	int _width, _height;
	GLuint _texture;
	const std::string _path;
};
#endif