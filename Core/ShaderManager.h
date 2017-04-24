#ifndef SHADERMANAGERH
#define SHADERMANAGERH

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
#include <string>
#include <vector>
#include "Shader.h"

class ShaderManager {
public:
	~ShaderManager();

	GLuint getShader(const std::string& name);

	GLuint newShader(const std::string& shaderPath, const std::string& shaderName,
		const std::string& dataLocation0 = "", const std::string& dataLocation1 = "");

	GLint getUniform(const std::string& shaderName, const std::string& uniform);
	GLint getAttrib(const std::string& shaderName, const std::string& uniform);

	void useShader(const std::string& name);

	void releaseShaders();

private:
	std::vector<Shader*> _shaderList;
};
#endif