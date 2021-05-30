//
// Created by pmerku on 29/05/2021.
//

#include "Shader.hpp"
#include "ErrorUtils.hpp"

using namespace opengl;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
	std::ifstream vShaderFile, fShaderFile;
	std::string vShaderCode, fShaderCode;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// read vertex.glsl and fragment.glsl
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderCode = vShaderStream.str();
		fShaderCode = fShaderStream.str();
	} catch (std::ifstream::failure &e) {
		ERROR_THROW(ReadShaderException());
	}

	const char *vertexCode = vShaderCode.c_str();
	const char *fragmentCode = fShaderCode.c_str();

	// compile vertex
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, nullptr);
	glCompileShader(vertex);
	checkErrors(vertex, VERTEX);

	// compile fragment
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, nullptr);
	glCompileShader(fragment);
	checkErrors(fragment, FRAGMENT);

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	checkErrors(id, PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::checkErrors(unsigned int shader, Type type) {
	int success;
	char infoLog[1024];

	if (type == PROGRAM) {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "INFO LOG: " << infoLog << std::endl;
			ERROR_THROW(LinkingException());
		}
	} else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "INFO LOG: " << infoLog << std::endl;
			ERROR_THROW(ShaderException());
		}
	}
}
