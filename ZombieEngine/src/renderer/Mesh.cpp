#include "Mesh.hpp"
#include <glad/gl.h>
#include <cstddef> // offsetof

namespace ZE {

Mesh::Mesh(const std::vector<Vertex>&       vertices,
           const std::vector<unsigned int>& indices)
    : m_vertexCount(static_cast<unsigned int>(vertices.size()))
    , m_indexCount (static_cast<unsigned int>(indices.size()))
{
    // VAO binden – alles was folgt, wird in ihm gespeichert
    m_vao.bind();

    // Vertex-Daten auf die GPU hochladen
    m_vbo = std::make_unique<VertexBuffer>(
        vertices.data(),
        static_cast<unsigned int>(vertices.size() * sizeof(Vertex))
    );

    // Index-Daten hochladen (optional)
    if (!indices.empty()) {
        m_ibo = std::make_unique<IndexBuffer>(
            indices.data(),
            m_indexCount
        );
    }

    // ── Vertex-Attribute konfigurieren ───────────────────────
    // Erklärt OpenGL, wie die rohen Bytes im VBO zu interpretieren sind.
    // stride = sizeof(Vertex) = Abstand von einem Vertex zum nächsten

    const unsigned int stride = sizeof(Vertex);

    // location 0: position (vec3 = 3 floats)
    m_vao.addAttribute({ 0, 3, GL_FLOAT, false, stride,
        static_cast<unsigned int>(offsetof(Vertex, position)) });

    // location 1: color (vec3 = 3 floats)
    m_vao.addAttribute({ 1, 3, GL_FLOAT, false, stride,
        static_cast<unsigned int>(offsetof(Vertex, color)) });

    // location 2: texCoord (vec2 = 2 floats)
    m_vao.addAttribute({ 2, 2, GL_FLOAT, false, stride,
        static_cast<unsigned int>(offsetof(Vertex, texCoord)) });

    // location 3: normal (vec3 = 3 floats)
    m_vao.addAttribute({ 3, 3, GL_FLOAT, false, stride,
        static_cast<unsigned int>(offsetof(Vertex, normal)) });

    // VAO unbinden – "eingefroren" für spätere Nutzung
    m_vao.unbind();
}

void Mesh::draw() const {
    m_vao.bind();

    if (m_ibo) {
        // Indexed Drawing: Indizes aus IBO → weniger Vertex-Duplikate
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(m_indexCount),
            GL_UNSIGNED_INT,
            nullptr
        );
    } else {
        // Array Drawing: Vertices sequenziell
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount));
    }

    m_vao.unbind();
}

// ─────────────────────────────────────────────────────────────
//  Factory: Dreieck
// ─────────────────────────────────────────────────────────────
Mesh Mesh::createTriangle() {
    // position           color (RGB)       texUV       normal
    std::vector<Vertex> v = {
        {{-0.5f,-0.5f, 0.0f}, {1.0f,0.0f,0.0f}, {0.0f,0.0f}, {0,0,1}}, // unten-links  = Rot
        {{ 0.5f,-0.5f, 0.0f}, {0.0f,1.0f,0.0f}, {1.0f,0.0f}, {0,0,1}}, // unten-rechts = Grün
        {{ 0.0f, 0.5f, 0.0f}, {0.0f,0.0f,1.0f}, {0.5f,1.0f}, {0,0,1}}, // oben-mitte   = Blau
    };
    return Mesh(v);
}

// ─────────────────────────────────────────────────────────────
//  Factory: Würfel  (6 Seiten, je 4 Vertices, 2 Dreiecke)
// ─────────────────────────────────────────────────────────────
Mesh Mesh::createCube() {
    // Jede Seite hat eigene Vertices, damit Normalen pro Seite korrekt sind
    // (geteilt Vertices würden für Beleuchtung falsche Normalen ergeben)
    std::vector<Vertex> v = {
        // ── Front (z+, Normal 0,0,1) ──────────────────────────
        {{-0.5f,-0.5f, 0.5f}, {1,1,1}, {0,0}, { 0, 0, 1}},
        {{ 0.5f,-0.5f, 0.5f}, {1,1,1}, {1,0}, { 0, 0, 1}},
        {{ 0.5f, 0.5f, 0.5f}, {1,1,1}, {1,1}, { 0, 0, 1}},
        {{-0.5f, 0.5f, 0.5f}, {1,1,1}, {0,1}, { 0, 0, 1}},
        // ── Back (z-, Normal 0,0,-1) ───────────────────────────
        {{ 0.5f,-0.5f,-0.5f}, {1,1,1}, {0,0}, { 0, 0,-1}},
        {{-0.5f,-0.5f,-0.5f}, {1,1,1}, {1,0}, { 0, 0,-1}},
        {{-0.5f, 0.5f,-0.5f}, {1,1,1}, {1,1}, { 0, 0,-1}},
        {{ 0.5f, 0.5f,-0.5f}, {1,1,1}, {0,1}, { 0, 0,-1}},
        // ── Left (x-, Normal -1,0,0) ───────────────────────────
        {{-0.5f,-0.5f,-0.5f}, {1,1,1}, {0,0}, {-1, 0, 0}},
        {{-0.5f,-0.5f, 0.5f}, {1,1,1}, {1,0}, {-1, 0, 0}},
        {{-0.5f, 0.5f, 0.5f}, {1,1,1}, {1,1}, {-1, 0, 0}},
        {{-0.5f, 0.5f,-0.5f}, {1,1,1}, {0,1}, {-1, 0, 0}},
        // ── Right (x+, Normal 1,0,0) ───────────────────────────
        {{ 0.5f,-0.5f, 0.5f}, {1,1,1}, {0,0}, { 1, 0, 0}},
        {{ 0.5f,-0.5f,-0.5f}, {1,1,1}, {1,0}, { 1, 0, 0}},
        {{ 0.5f, 0.5f,-0.5f}, {1,1,1}, {1,1}, { 1, 0, 0}},
        {{ 0.5f, 0.5f, 0.5f}, {1,1,1}, {0,1}, { 1, 0, 0}},
        // ── Top (y+, Normal 0,1,0) ─────────────────────────────
        {{-0.5f, 0.5f, 0.5f}, {1,1,1}, {0,0}, { 0, 1, 0}},
        {{ 0.5f, 0.5f, 0.5f}, {1,1,1}, {1,0}, { 0, 1, 0}},
        {{ 0.5f, 0.5f,-0.5f}, {1,1,1}, {1,1}, { 0, 1, 0}},
        {{-0.5f, 0.5f,-0.5f}, {1,1,1}, {0,1}, { 0, 1, 0}},
        // ── Bottom (y-, Normal 0,-1,0) ─────────────────────────
        {{-0.5f,-0.5f,-0.5f}, {1,1,1}, {0,0}, { 0,-1, 0}},
        {{ 0.5f,-0.5f,-0.5f}, {1,1,1}, {1,0}, { 0,-1, 0}},
        {{ 0.5f,-0.5f, 0.5f}, {1,1,1}, {1,1}, { 0,-1, 0}},
        {{-0.5f,-0.5f, 0.5f}, {1,1,1}, {0,1}, { 0,-1, 0}},
    };

    // Jede der 6 Seiten = 2 Dreiecke (= 6 Indices aus 4 Vertices)
    std::vector<unsigned int> idx;
    idx.reserve(36);
    for (unsigned int face = 0; face < 6; face++) {
        unsigned int b = face * 4;
        // Quad in 2 Dreiecke aufteilen (gegen den Uhrzeigersinn = front-facing)
        idx.insert(idx.end(), { b+0,b+1,b+2,  b+2,b+3,b+0 });
    }

    return Mesh(v, idx);
}

// ─────────────────────────────────────────────────────────────
//  Factory: Plane (Boden)
// ─────────────────────────────────────────────────────────────
Mesh Mesh::createPlane(float size) {
    float h = size * 0.5f;
    std::vector<Vertex> v = {
        {{-h, 0.0f,-h}, {1,1,1}, {    0,    0}, {0,1,0}},
        {{ h, 0.0f,-h}, {1,1,1}, {size/2,    0}, {0,1,0}},
        {{ h, 0.0f, h}, {1,1,1}, {size/2,size/2}, {0,1,0}},
        {{-h, 0.0f, h}, {1,1,1}, {    0,size/2}, {0,1,0}},
    };
    std::vector<unsigned int> idx = { 0,1,2,  2,3,0 };
    return Mesh(v, idx);
}

} // namespace ZE
