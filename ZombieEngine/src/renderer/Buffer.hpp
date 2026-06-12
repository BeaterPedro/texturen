#pragma once
#include <cstddef>

namespace ZE {

// ─────────────────────────────────────────────────────────────
//  VertexBuffer (VBO) – Vertex-Daten auf der GPU
//
//  Speichert Rohdaten (Positionen, Farben, UVs, Normalen) im
//  GPU-Speicher. Muss an einen VertexArray gebunden werden.
// ─────────────────────────────────────────────────────────────
class VertexBuffer {
public:
    VertexBuffer() = default;
    VertexBuffer(const void* data, unsigned int sizeBytes);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void bind()   const; // GL_ARRAY_BUFFER
    void unbind() const;

    unsigned int getID() const { return m_id; }

private:
    unsigned int m_id = 0;
};

// ─────────────────────────────────────────────────────────────
//  IndexBuffer (EBO/IBO) – Indizes für indexed Drawing
//
//  Vermeidet doppelte Vertices: Jedes Dreieck eines Würfels
//  kann Eckpunkte wiederverwenden.
// ─────────────────────────────────────────────────────────────
class IndexBuffer {
public:
    IndexBuffer() = default;
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)            = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    void bind()   const; // GL_ELEMENT_ARRAY_BUFFER
    void unbind() const;

    unsigned int getCount() const { return m_count; }
    unsigned int getID()    const { return m_id; }

private:
    unsigned int m_id    = 0;
    unsigned int m_count = 0;
};

// ─────────────────────────────────────────────────────────────
//  VertexAttribute – beschreibt ein Feld im Vertex-Layout
//
//  Beispiel für unsere Vertex-Struktur:
//    { position[3], color[3], texCoord[2], normal[3] }
//    → 4 Attribute an layout-locations 0, 1, 2, 3
// ─────────────────────────────────────────────────────────────
struct VertexAttribute {
    unsigned int index;       // layout(location = N) im Shader
    int          size;        // Anzahl Komponenten (1–4)
    unsigned int type;        // GL_FLOAT, GL_INT, etc.
    bool         normalized;  // Integer → Float normalisieren?
    unsigned int stride;      // Bytes pro Vertex (gesamtes Struct)
    unsigned int offset;      // Byte-Offset im Struct
};

// ─────────────────────────────────────────────────────────────
//  VertexArray (VAO) – "Erinnerung" für Vertex-Layout
//
//  Merkt sich, welche VBOs gebunden sind und wie deren Daten
//  interpretiert werden sollen. Beim Zeichnen reicht ein
//  einziges VAO.bind() statt aller Einzeleinstellungen.
// ─────────────────────────────────────────────────────────────
class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&)            = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    void bind()   const;
    void unbind() const;

    // Muss aufgerufen werden, während das zugehörige VBO gebunden ist!
    void addAttribute(const VertexAttribute& attr);

    unsigned int getID() const { return m_id; }

private:
    unsigned int m_id = 0;
};

} // namespace ZE
