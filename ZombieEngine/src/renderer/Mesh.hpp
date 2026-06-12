#pragma once
#include "Buffer.hpp"
#include <vector>
#include <memory>

namespace ZE {

// ─────────────────────────────────────────────────────────────
//  Vertex – Layout eines einzelnen Vertex im GPU-Speicher
//
//  Dieses Layout ist abgestimmt auf basic.vert (locations 0-3).
//  Wichtig: Reihenfolge und Typen müssen zum Shader passen!
// ─────────────────────────────────────────────────────────────
struct Vertex {
    float position[3];  // layout(location = 0) → x, y, z
    float color[3];     // layout(location = 1) → r, g, b   (Vertex-Farbe)
    float texCoord[2];  // layout(location = 2) → u, v       (UV-Koordinaten)
    float normal[3];    // layout(location = 3) → nx, ny, nz (für Beleuchtung)
};

// ─────────────────────────────────────────────────────────────
//  Mesh – Container für Geometrie
//
//  Verwaltet VAO/VBO/IBO und bietet eine einfache draw()-Methode.
//  Factory-Methoden erstellen Standard-Geometrie direkt.
//
//  Verwendung:
//    auto cube = Mesh::createCube();
//    cube.draw(); // zeichnet mit aktivem Shader
// ─────────────────────────────────────────────────────────────
class Mesh {
public:
    Mesh(const std::vector<Vertex>&       vertices,
         const std::vector<unsigned int>& indices = {});

    ~Mesh() = default;

    // Nicht kopierbar (GPU-Ressourcen)
    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Verschiebbar
    Mesh(Mesh&&) noexcept            = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    // VAO binden und Dreiecke zeichnen
    void draw() const;

    // ── Factory-Methoden ─────────────────────────────────────
    // Erstellen fertige Test-Geometrien

    // Buntes Dreieck – der klassische OpenGL-Einführungstest
    static Mesh createTriangle();

    // Einheitswürfel (1×1×1) mit 6 Seiten, korrekten Normalen
    static Mesh createCube();

    // Horizontale Ebene der Größe `size` × `size`
    static Mesh createPlane(float size = 1.0f);

private:
    VertexArray                   m_vao;
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer>  m_ibo;

    unsigned int m_vertexCount = 0;
    unsigned int m_indexCount  = 0;
};

} // namespace ZE
