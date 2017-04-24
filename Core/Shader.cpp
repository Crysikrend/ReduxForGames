#include "Shader.h"

bool Shader::printShaderInfo(GLuint s) {
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(s, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(s, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}

	GLint success;
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	return success != 0;
}

Shader::Shader(const std::string& shaderPath, const std::string& shaderName, const std::string& dataLocation0, const std::string& dataLocation1) {
	
	_shaderName = shaderName;
	GLuint vertexShader, fragmentShader;

	bool success = true;

	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::ifstream vshader(shaderPath + shaderName + ".vert");
	std::string vsString((std::istreambuf_iterator<char>(vshader)), (std::istreambuf_iterator<char>()));

	const char* vsChars = vsString.c_str();
	glShaderSource(vertexShader, 1, &vsChars, NULL);
	//std::cout << "Compiling vertex shader \"" << shaderPath << shaderName + ".frag" << "\"." << std::endl;
	glCompileShader(vertexShader);
	success &= printShaderInfo(vertexShader);

	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fshader(shaderPath + shaderName + ".frag");
	std::string fsString((std::istreambuf_iterator<char>(fshader)), (std::istreambuf_iterator<char>()));

	const char* fsChars = fsString.c_str();
	glShaderSource(fragmentShader, 1, &fsChars, NULL);
	//std::cout << "Compiling fragment shader \"" << fragmentShaderPath << "\"." << std::endl;
	glCompileShader(fragmentShader);
	success &= printShaderInfo(fragmentShader);

	// Bundle them into a program
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);

	if (dataLocation0.length() > 0)
		glBindFragDataLocation(_shaderProgram, 0, dataLocation0.c_str());
	if (dataLocation1.length() > 0)
		glBindFragDataLocation(_shaderProgram, 1, dataLocation1.c_str());
	glLinkProgram(_shaderProgram);
	//success &= printProgramInfo(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(getShader());
}

GLuint Shader::getShader() const {
	return _shaderProgram;
}

std::string Shader::getShaderName() const {
	return _shaderName;
}