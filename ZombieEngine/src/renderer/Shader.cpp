#include "Shader.hpp"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace ZE {

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Quelltexte laden
    std::string vertSrc = readFile(vertexPath);
    std::string fragSrc = readFile(fragmentPath);

    // Kompilieren
    unsigned int vert = compileShader(GL_VERTEX_SHADER,   vertSrc);
    unsigned int frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    // Linken
    m_id = glCreateProgram();
    glAttachShader(m_id, vert);
    glAttachShader(m_id, frag);
    glLinkProgram (m_id);
    checkLinkError(m_id);

    // Einzelne Shader-Objekte können nach dem Linken gelöscht werden
    glDeleteShader(vert);
    glDeleteShader(frag);

    std::cout << "[Shader] Erfolgreich gelinkt (ID=" << m_id << "): "
              << vertexPath << " + " << fragmentPath << "\n";
}

Shader::~Shader() {
    if (m_id) {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}

Shader::Shader(Shader&& other) noexcept : m_id(other.m_id) {
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteProgram(m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

// ── Bind / Unbind ─────────────────────────────────────────────

void Shader::bind()   const { glUseProgram(m_id); }
void Shader::unbind() const { glUseProgram(0); }

// ── Uniforms ──────────────────────────────────────────────────

void Shader::setInt  (const std::string& n, int v)             const { glUniform1i   (getUniformLoc(n), v); }
void Shader::setFloat(const std::string& n, float v)           const { glUniform1f   (getUniformLoc(n), v); }
void Shader::setBool (const std::string& n, bool v)            const { glUniform1i   (getUniformLoc(n), static_cast<int>(v)); }
void Shader::setVec2 (const std::string& n, const glm::vec2& v)const { glUniform2fv  (getUniformLoc(n), 1, glm::value_ptr(v)); }
void Shader::setVec3 (const std::string& n, const glm::vec3& v)const { glUniform3fv  (getUniformLoc(n), 1, glm::value_ptr(v)); }
void Shader::setVec4 (const std::string& n, const glm::vec4& v)const { glUniform4fv  (getUniformLoc(n), 1, glm::value_ptr(v)); }
void Shader::setMat3 (const std::string& n, const glm::mat3& m)const { glUniformMatrix3fv(getUniformLoc(n), 1, GL_FALSE, glm::value_ptr(m)); }
void Shader::setMat4 (const std::string& n, const glm::mat4& m)const { glUniformMatrix4fv(getUniformLoc(n), 1, GL_FALSE, glm::value_ptr(m)); }

// ── Private ───────────────────────────────────────────────────

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char*  src    = source.c_str();
    glShaderSource (shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        const std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        throw std::runtime_error(
            "[Shader] " + typeStr + "-Kompilierungsfehler:\n" + log
        );
    }
    return shader;
}

void Shader::checkLinkError(unsigned int program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        throw std::runtime_error(
            "[Shader] Link-Fehler:\n" + std::string(log)
        );
    }
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("[Shader] Datei nicht gefunden: " + path);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int Shader::getUniformLoc(const std::string& name) const {
    int loc = glGetUniformLocation(m_id, name.c_str());
    if (loc == -1) {
        // Kein Fehler – Uniform könnte durch Compiler-Optimierung entfernt worden sein
        std::cerr << "[Shader] Warnung: Uniform '" << name << "' nicht gefunden (ID=" << m_id << ")\n";
    }
    return loc;
}

} // namespace ZE
