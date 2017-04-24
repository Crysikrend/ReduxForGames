#ifndef MESHH
#define MESHH

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

class Mesh {
public:
	Mesh(const std::string& meshName, const std::vector<GLfloat>& vertices, int valuesPerPrim, int verticesPerPrim, bool hasTexture);

	void buffer(GLuint& vao, GLuint& vbo) const;
	int getNoOfVerticesTotal() const;
	int getValuesPerPrim() const;
	int getVerticesPerPrim() const;
	bool getHasTexture() const;

private:
	const std::string _meshName;
	std::vector<GLfloat> _vertices;
	int _valuesPerPrim, _verticesPerPrim;
	bool _hasTexture;
};
#endif