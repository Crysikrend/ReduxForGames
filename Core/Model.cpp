#include "Model.h"

Model::Model(Mesh* mesh, ShaderManager* shaderManager, const std::string& shader, Texture* texture)
	: _mesh(mesh), _shaders(shaderManager), _shaderToUse(shader), _texture(texture) {

	_sca = glm::vec3(1, 1, 1);
	_col = glm::vec4(1, 1, 1, 1);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	// Bind the vertices of the mesh into buffers
	_mesh->buffer(_vao, _vbo);

	GLint posAttrib = _shaders->getAttrib(_shaderToUse, "position");
	GLint texAttrib = _shaders->getAttrib(_shaderToUse, "texCoord");
	int stride = _mesh->getValuesPerPrim();

	/*glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

	// Position attribute
	glVertexAttribPointer(posAttrib, _mesh->getVerticesPerPrim(), GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(posAttrib);

	// TexCoord attribute
	if (_mesh->getHasTexture()) {
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(_mesh->getVerticesPerPrim() * sizeof(GLfloat)));
		glEnableVertexAttribArray(texAttrib);
	}
	glBindVertexArray(0); // Unbind VAO

	if (_texture != NULL)
		_texture->bind();
}

Model::~Model() {
	release();
}

void Model::initialise(const glm::vec4& persp, const glm::mat4& viewPos) {

	// Reset matrices before setting them to stop multiplying
	_proj = glm::mat4();
	_proj = glm::perspective(persp[0], persp[1], persp[2], persp[3]);

	_view = viewPos;
}

void Model::render() {

	if (_texture != NULL) {
		glActiveTexture(GL_TEXTURE0);

		// Unsure why this won't work. Check Texture::bind()
		glBindTexture(GL_TEXTURE_2D, _texture->getTexture());

		glUniform1i( _shaders->getUniform(_shaderToUse, "ourTexture1"), 0);
	}

	_shaders->useShader(_shaderToUse);

	glBindVertexArray(_vao);
	glm::mat4 model;

	glm::mat4 translation;
	glm::mat4 scale;
	glm::mat4 rotation;

	scale = glm::scale(scale, _sca);

	rotation = glm::rotate(rotation, _rot[0], glm::vec3(1, 0, 0));
	rotation *= glm::rotate(rotation, _rot[1], glm::vec3(0, 1, 0));
	rotation *= glm::rotate(rotation, _rot[2], glm::vec3(0, 0, 1));

	_pos.y *= -1;
	translation = glm::translate(translation, _pos);

	//model = scale * rotation * translation;
	model = translation * rotation * scale;

	GLint modelInt = _shaders->getUniform(_shaderToUse, "model");
	glUniformMatrix4fv(modelInt, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewInt = _shaders->getUniform(_shaderToUse, "view");
	glUniformMatrix4fv(viewInt, 1, GL_FALSE, glm::value_ptr(_view));

	GLint projInt = _shaders->getUniform(_shaderToUse, "projection");
	glUniformMatrix4fv(projInt, 1, GL_FALSE, glm::value_ptr(_proj));

	GLint vertexColorLocation = _shaders->getUniform(_shaderToUse, "colour");
	glUniform4f(vertexColorLocation, _col[0], _col[1], _col[2], _col[3]);

	//glDrawElements(GL_TRIANGLES, _mesh->getNoOfVerticesTotal(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, _mesh->getNoOfVerticesTotal());

	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::release() {
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
}