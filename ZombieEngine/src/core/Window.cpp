// glad MUSS vor GLFW includiert werden!
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include <iostream>
#include <stdexcept>

namespace ZE {

Window::Window(const WindowConfig& config) {
    initialize(config);
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Window::initialize(const WindowConfig& config) {
    if (!glfwInit()) {
        throw std::runtime_error("[Window] GLFW-Initialisierung fehlgeschlagen!");
    }

    // ── OpenGL 4.1 Core Profile ──────────────────────────────
    // 4.1 ist die höchste Version, die macOS nativ unterstützt.
    // Auf Windows/Linux kannst du auf 4.6 erhöhen.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pflicht auf macOS

    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

    // 4× Anti-Aliasing (kann später in Renderer-Einstellungen ausgelagert werden)
    glfwWindowHint(GLFW_SAMPLES, 4);

    // ── Fenster erstellen ────────────────────────────────────
    m_window = glfwCreateWindow(
        config.width,
        config.height,
        config.title.c_str(),
        nullptr,  // kein Fullscreen-Monitor
        nullptr   // kein Context-Sharing
    );

    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("[Window] GLFW-Fenster-Erstellung fehlgeschlagen!");
    }

    // ── OpenGL-Kontext aktivieren ────────────────────────────
    glfwMakeContextCurrent(m_window);

    // Window-Zeiger im GLFW-Userdata speichern → für Callbacks
    glfwSetWindowUserPointer(m_window, this);

    // VSync (swap interval 1 = 60 FPS max, 0 = kein Limit)
    glfwSwapInterval(config.vsync ? 1 : 0);

    // Framebuffer-Resize-Callback registrieren
    glfwSetFramebufferSizeCallback(m_window, Window::framebufferSizeCallback);

    // Tatsächliche Framebuffer-Größe abfragen
    // (kann von config.width/height abweichen, z.B. auf HiDPI-Displays)
    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    std::cout << "[Window] Erstellt: "
              << m_width << " x " << m_height
              << " | Titel: '" << config.title << "'"
              << " | VSync: " << (config.vsync ? "an" : "aus") << "\n";
}

// ── Game-Loop-Interface ───────────────────────────────────────

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window) != 0;
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(m_window);
}

// ── Einstellungen ─────────────────────────────────────────────

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setVSync(bool enable) {
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(enable ? 1 : 0);
}

// ── Statischer Callback ───────────────────────────────────────

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // OpenGL-Viewport an neue Fenstergröße anpassen
    glViewport(0, 0, width, height);

    // Interne Größe aktualisieren (für getAspect() etc.)
    if (auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
        win->m_width  = width;
        win->m_height = height;
    }
}

} // namespace ZE
