#pragma once
#include <glm/glm.hpp>

namespace ZE {

class Mesh;
class Shader;

struct RenderStats {
    unsigned int drawCalls = 0;
    float        frameTimeMs = 0.0f;
};

// ─────────────────────────────────────────────────────────────
//  Renderer – OpenGL-Zustandsverwaltung & Zeichnen
//
//  Zentralisiert alle glEnable/glDisable-Aufrufe und bietet
//  eine saubere beginFrame/endFrame-Schnittstelle.
//  In Phase 2 kommt hier eine Render-Queue hinzu.
// ─────────────────────────────────────────────────────────────
class Renderer {
public:
    Renderer()  = default;
    ~Renderer() = default;

    void initialize();

    void beginFrame(); // glClear etc.
    void endFrame();   // Statistiken, Flush

    // ── Einstellungen ─────────────────────────────────────────
    void setClearColor(const glm::vec4& color);
    void setDepthTest (bool enable);
    void setWireframe (bool enable);
    void setFaceCulling(bool enable);

    // ── Zeichnen ──────────────────────────────────────────────
    // transform = Model-Matrix (Position/Rotation/Skalierung im Weltkoordinatensystem)
    void drawMesh(const Mesh& mesh, Shader& shader, const glm::mat4& transform);

    const RenderStats& getStats() const { return m_stats; }

private:
    glm::vec4   m_clearColor {0.08f, 0.08f, 0.12f, 1.0f}; // dunkles Lila
    RenderStats m_stats;
};

} // namespace ZE
