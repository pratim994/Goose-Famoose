#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::~Shader() {
    if (program_) glDeleteProgram(program_);
}

Shader::Shader(Shader&& other) noexcept : program_(other.program_) {
    other.program_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_) glDeleteProgram(program_);
        program_ = other.program_;
        other.program_ = 0;
    }
    return *this;
}

GLuint Shader::compile(GLenum type, const std::string& src) {
    GLuint shader = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        std::cerr << "[Shader] compile error (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "): " << log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::loadFromSource(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vs = compile(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentSrc);
    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return false;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    GLint linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    glDeleteShader(vs);
    glDeleteShader(fs);

    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(prog, 1024, nullptr, log);
        std::cerr << "[Shader] link error: " << log << std::endl;
        glDeleteProgram(prog);
        return false;
    }

    if (program_) glDeleteProgram(program_);
    program_ = prog;
    return true;
}

static bool readFile(const std::string& path, std::string& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Shader] could not open file: " << path << std::endl;
        return false;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    out = ss.str();
    return true;
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vsrc, fsrc;
    if (!readFile(vertexPath, vsrc)) return false;
    if (!readFile(fragmentPath, fsrc)) return false;
    return loadFromSource(vsrc, fsrc);
}

void Shader::use() const {
    glUseProgram(program_);
}

GLint Shader::locationOf(const std::string& name) const {
    return glGetUniformLocation(program_, name.c_str());
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) const {
    glUniformMatrix4fv(locationOf(name), 1, GL_FALSE, glm::value_ptr(m));
}
void Shader::setVec4(const std::string& name, const glm::vec4& v) const {
    glUniform4fv(locationOf(name), 1, glm::value_ptr(v));
}
void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(locationOf(name), 1, glm::value_ptr(v));
}
void Shader::setInt(const std::string& name, int v) const {
    glUniform1i(locationOf(name), v);
}
void Shader::setFloat(const std::string& name, float v) const {
    glUniform1f(locationOf(name), v);
}