#include "Shader.h"
#include "../../utils/Math.h"
#include "utils/Logger.h"
#include <fstream>
#include <vector>
#include <iostream>

void Shader::Enable() {
    glUseProgram(mProg);
    /*for (int i = 0; i < mNumAttribs; i++)
        glEnableVertexAttribArray(i);*/
}

void Shader::Disable() {
    glUseProgram(0);
    /*for (int i = 0; i < mNumAttribs; i++)
        glDisableVertexAttribArray(i);*/
}

Shader* Shader::Attach(const std::string& filename, bool verbose) {
    // Load GLSL Shader Source from File
	std::string path;
	if (verbose)
	{
		//path = FileSystem::GetPath()
		path = filename;
	}
	else
	{
		path = FileSystem::GetPath("resources/shaders/" + filename);
	}
    std::ifstream fd(path);
    auto src = std::string(std::istreambuf_iterator<char>(fd),
                           (std::istreambuf_iterator<char>()));

    // Create a Shader Object
    const char* source = src.c_str();
    auto shader = Create(filename);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &mStatus);

    // Display the Build Log on Error
    if (mStatus == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &mLength);
        std::unique_ptr<char[]> buffer(new char[mLength]);
        glGetShaderInfoLog(shader, mLength, nullptr, buffer.get());

		if (mDebugEnabled)
			Logger::Log(filename + " " + std::string(buffer.get()), ERROR);
    }

    // Attach the Shader and Free Allocated Memory
    glAttachShader(mProg, shader);
    glDeleteShader(shader);

    return this;
}

Shader* Shader::Link() {
    glLinkProgram(mProg);
    glGetProgramiv(mProg, GL_LINK_STATUS, &mStatus);

    if (mStatus == GL_FALSE) {
        glGetProgramiv(mProg, GL_INFO_LOG_LENGTH, &mLength);
        std::unique_ptr<char[]> buffer(new char[mLength]);
        glGetProgramInfoLog(mProg, mLength, nullptr, buffer.get());

		//if (mDebugEnabled)
			Logger::Log(std::string(buffer.get()), ERROR);
    }
    assert(mStatus == true);

    return this;
}

GLuint Shader::Create(const std::string& filename) {
    auto index = filename.rfind(".");
    auto ext = filename.substr(index + 1);
    if (ext == "comp") return glCreateShader(GL_COMPUTE_SHADER);
    else if (ext == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
    else if (ext == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
    else if (ext == "vert") return glCreateShader(GL_VERTEX_SHADER);
    else                    return false;
}

GLint Shader::GetUniformLocation(const std::string& uniformName) {
    GLint location = glGetUniformLocation(mProg, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		if (mDebugEnabled)
			Logger::Log("Could not find uniform: " + uniformName, ERROR);
	}
    return location;
}

void Shader::SetUniform(const std::string& name, GLint data) {
    GLint location = GetUniformLocation(name);
    glUniform1i(location, data);
}

void Shader::SetUniform(const std::string& name, GLint* data, GLsizei count) {
    GLint location = GetUniformLocation(name);
    glUniform1iv(location, count, data);
}

void Shader::SetUniform(const std::string& name, GLfloat data) {
    GLint location = GetUniformLocation(name);
    glUniform1f(location, data);
}

void Shader::SetUniform(const std::string& name, GLfloat* data, GLsizei count) {
    GLint location = GetUniformLocation(name);
    glUniform1fv(location, count, data);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& vector) {
    GLint location = GetUniformLocation(name);
    glUniform2f(location, vector.x, vector.y);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& vector) {
    GLint location = GetUniformLocation(name);
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& vector) {
    GLint location = GetUniformLocation(name);
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& matrix) {
    GLint location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}