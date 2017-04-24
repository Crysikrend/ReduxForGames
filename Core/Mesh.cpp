#include "Mesh.h"

Mesh::Mesh(const std::string& meshName, const std::vector<GLfloat>& vertices, int valuesPerPrim, int verticesPerPrim, bool hasTexture)
	:_meshName(meshName), _vertices(vertices), _valuesPerPrim(valuesPerPrim), _verticesPerPrim(verticesPerPrim), _hasTexture(hasTexture) {}

void Mesh::buffer(GLuint& vao, GLuint& vbo) const{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
}

int Mesh::getNoOfVerticesTotal() const {
	return _vertices.size() / _valuesPerPrim;
}

int Mesh::getValuesPerPrim() const {
	return _valuesPerPrim;
}

int Mesh::getVerticesPerPrim() const {
	return _verticesPerPrim;
}

bool Mesh::getHasTexture() const {
	return _hasTexture;
}
