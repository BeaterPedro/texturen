#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include <glad/gl.h>
#include <iostream>

namespace ZE {

void Renderer::initialize() {
    // ── Depth-Test ───────────────────────────────────────────
    // Stellt sicher, dass entfernte Objekte von näheren verdeckt werden
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // ── Face-Culling ─────────────────────────────────────────
    // Rückseiten (back-faces) nicht zeichnen → ~50% weniger Fragment-Shader-Aufrufe
    // Achtung: Vertices müssen gegen den Uhrzeigersinn (CCW) definiert sein!
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // ── Multisampling (MSAA 4x) ──────────────────────────────
    glEnable(GL_MULTISAMPLE);

    std::cout << "[Renderer] Initialisiert.\n";
    std::cout << "[Renderer] OpenGL Version : " << glGetString(GL_VERSION) << "\n";
    std::cout << "[Renderer] Grafikkarte    : " << glGetString(GL_RENDERER) << "\n";
    std::cout << "[Renderer] GLSL Version   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";
}

void Renderer::beginFrame() {
    // Statistiken zurücksetzen
    m_stats.drawCalls = 0;

    // Farb- und Tiefen-Buffer leeren
    glClearColor(
        m_clearColor.r, m_clearColor.g,
        m_clearColor.b, m_clearColor.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Placeholder – in Phase 2: Render-Queue flushen, Stats protokollieren
}

// ── Einstellungen ─────────────────────────────────────────────

void Renderer::setClearColor(const glm::vec4& color) {
    m_clearColor = color;
}

void Renderer::setDepthTest(bool enable) {
    enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::setWireframe(bool enable) {
    // GL_FILL = normales Rendering, GL_LINE = Drahtgitter
    glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}

void Renderer::setFaceCulling(bool enable) {
    enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

// ── Zeichnen ──────────────────────────────────────────────────

void Renderer::drawMesh(const Mesh& mesh, Shader& shader, const glm::mat4& transform) {
    shader.bind();
    shader.setMat4("uModel", transform);
    mesh.draw();
    shader.unbind();
    m_stats.drawCalls++;
}

} // namespace ZE
