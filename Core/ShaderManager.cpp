#include "ShaderManager.h"

ShaderManager::~ShaderManager() {
	releaseShaders();
}
GLuint ShaderManager::getShader(const std::string& name) {

	for (int i = 0; i < _shaderList.size(); i++) {
		if (_shaderList[i]->getShaderName() == name)
			return _shaderList[i]->getShader();
	}
	return NULL;
}
GLuint ShaderManager::newShader(const std::string& shaderPath, const std::string& shaderName,
	const std::string& dataLocation0, const std::string& dataLocation1) {
	Shader* placeHolder = new Shader(shaderPath, shaderName, dataLocation0, dataLocation1);
	_shaderList.push_back(placeHolder);
	return placeHolder->getShader();
}
GLint ShaderManager::getUniform(const std::string & shaderName, const std::string & uniform)
{
	const GLchar* cUniform = uniform.c_str();
	return glGetUniformLocation(getShader(shaderName), cUniform);
}
GLint ShaderManager::getAttrib(const std::string & shaderName, const std::string & uniform)
{
	const GLchar* cUniform = uniform.c_str();
	return glGetAttribLocation(getShader(shaderName), cUniform);
}
void ShaderManager::useShader(const std::string & name)
{
	glUseProgram(getShader(name));
}
void ShaderManager::releaseShaders() {
	for (int i = 0; i < _shaderList.size(); i++) {
		delete _shaderList[i];
		_shaderList[i] = NULL;
	}

	_shaderList.clear();
}