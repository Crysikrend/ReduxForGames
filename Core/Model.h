#ifndef MODELH
#define MODELH

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

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "ShaderManager.h"
#include "Texture.h"

class Model {
public:
	Model(Mesh* mesh, ShaderManager* shaderManager, const std::string& shader, Texture* texture = NULL);
	~Model();

	glm::vec3 _pos;
	glm::vec3 _rot;
	glm::vec3 _sca;
	glm::vec4 _col;

	void initialise(const glm::vec4& persp, const glm::mat4& viewPos = glm::mat4());
	void render();
	void release();

private:
	Mesh* _mesh;
	Texture* _texture;
	GLuint _vao, _vbo;

	ShaderManager* _shaders;
	std::string _shaderToUse;

	// Rendering Mats
	glm::mat4 _view, _proj;
};
#endif