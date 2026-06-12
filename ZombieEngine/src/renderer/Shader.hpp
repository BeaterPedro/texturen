#pragma once
#include <string>
#include <glm/glm.hpp>

namespace ZE {

// ─────────────────────────────────────────────────────────────
//  Shader – Vertex + Fragment Shader Pipeline
//
//  Lädt GLSL-Quelldateien von Disk, kompiliert und linkt sie.
//  Bietet typisierte Methoden zum Setzen aller gängigen Uniforms.
//
//  Verwendung:
//    Shader shader("shaders/basic.vert", "shaders/basic.frag");
//    shader.bind();
//    shader.setMat4("uModel", modelMatrix);
//    shader.setVec3("uColor", {1.0f, 0.5f, 0.0f});
//    mesh.draw();
//    shader.unbind();
// ─────────────────────────────────────────────────────────────
class Shader {
public:
    Shader() = default;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // Nicht kopierbar (OpenGL-Handle)
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    // Verschiebbar
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void bind()   const; // glUseProgram(id)
    void unbind() const; // glUseProgram(0)

    // ── Uniform-Setter ───────────────────────────────────────
    void setInt  (const std::string& name, int v)               const;
    void setFloat(const std::string& name, float v)             const;
    void setBool (const std::string& name, bool v)              const;
    void setVec2 (const std::string& name, const glm::vec2& v)  const;
    void setVec3 (const std::string& name, const glm::vec3& v)  const;
    void setVec4 (const std::string& name, const glm::vec4& v)  const;
    void setMat3 (const std::string& name, const glm::mat3& m)  const;
    void setMat4 (const std::string& name, const glm::mat4& m)  const;

    unsigned int getID()  const { return m_id; }
    bool         isValid() const { return m_id != 0; }

private:
    unsigned int compileShader (unsigned int type, const std::string& source);
    void         checkLinkError(unsigned int program);
    std::string  readFile      (const std::string& path);
    int          getUniformLoc (const std::string& name) const;

    unsigned int m_id = 0;
};

} // namespace ZE
