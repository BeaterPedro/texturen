#include "Buffer.hpp"
#include <glad/gl.h>

namespace ZE {

// ── VertexBuffer ──────────────────────────────────────────────

VertexBuffer::VertexBuffer(const void* data, unsigned int sizeBytes) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    // GL_STATIC_DRAW = Daten werden einmal hochgeladen, oft gelesen
    // GL_DYNAMIC_DRAW = Daten ändern sich häufig (z.B. Partikel)
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind()   const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// ── IndexBuffer ───────────────────────────────────────────────

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
    : m_count(count)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(count * sizeof(unsigned int)),
        indices,
        GL_STATIC_DRAW
    );
}

IndexBuffer::~IndexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

void IndexBuffer::bind()   const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

// ── VertexArray ───────────────────────────────────────────────

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
    if (m_id) glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind()   const { glBindVertexArray(m_id); }
void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::addAttribute(const VertexAttribute& attr) {
    glVertexAttribPointer(
        attr.index,
        attr.size,
        attr.type,
        attr.normalized ? GL_TRUE : GL_FALSE,
        static_cast<GLsizei>(attr.stride),
        // Offset als Zeiger – das ist die offizielle OpenGL-API
        reinterpret_cast<const void*>(static_cast<std::uintptr_t>(attr.offset))
    );
    glEnableVertexAttribArray(attr.index);
}

} // namespace ZE
