#ifndef SHADERH
#define SHADERH

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

class Shader {
public:
	Shader(const std::string& shaderPath, const std::string& shaderName,
		const std::string& dataLocation0 = "", const std::string& dataLocation1 = "");
	~Shader();

	GLuint getShader() const;
	std::string getShaderName() const;

private:
	bool printShaderInfo(GLuint s);

	std::string _shaderName;
	GLuint _shaderProgram;
};
#endif