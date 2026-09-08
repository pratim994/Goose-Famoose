#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

// Thin wrapper around a compiled+linked GLSL program.
class Shader {
public:
    Shader() = default;
    ~Shader();

    // Non-copyable (owns a GL object), movable.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // Builds from GLSL source strings already loaded into memory.
    bool loadFromSource(const std::string& vertexSrc, const std::string& fragmentSrc);
    // Builds by reading two files off disk.
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

    void use() const;
    GLuint id() const { return program_; }

    void setMat4(const std::string& name, const glm::mat4& m) const;
    void setVec4(const std::string& name, const glm::vec4& v) const;
    void setVec3(const std::string& name, const glm::vec3& v) const;
    void setInt(const std::string& name, int v) const;
    void setFloat(const std::string& name, float v) const;

private:
    GLuint program_ = 0;
    GLint locationOf(const std::string& name) const;
    static GLuint compile(GLenum type, const std::string& src);
};