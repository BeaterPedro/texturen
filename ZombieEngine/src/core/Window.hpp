#pragma once
#include <string>

struct GLFWwindow; // Forward-Declaration

namespace ZE {

struct WindowConfig {
    int         width     = 1280;
    int         height    = 720;
    std::string title     = "ZombieEngine";
    bool        vsync     = true;
    bool        resizable = true;
};

// ─────────────────────────────────────────────────────────────
//  Window – GLFW-Fenster + OpenGL-Kontext-Erstellung
//
//  Kapselt alles, was mit dem Betriebssystem-Fenster zu tun hat:
//  - GLFW initialisieren
//  - OpenGL 4.1 Core-Profil-Kontext erstellen
//  - VSync ein-/ausschalten
//  - Framebuffer-Größe bei Fenster-Resize aktualisieren
// ─────────────────────────────────────────────────────────────
class Window {
public:
    explicit Window(const WindowConfig& config = {});
    ~Window();

    // Nicht kopierbar (nur ein Fenster)
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    // ── Game-Loop-Interface ──────────────────────────────────
    bool shouldClose()  const;
    void pollEvents()   const;
    void swapBuffers()  const;

    // ── Abfragen ─────────────────────────────────────────────
    int   getWidth()  const { return m_width; }
    int   getHeight() const { return m_height; }
    float getAspect() const {
        return static_cast<float>(m_width) / static_cast<float>(m_height);
    }

    GLFWwindow* getNativeWindow() const { return m_window; }

    // ── Einstellungen ─────────────────────────────────────────
    void setTitle (const std::string& title);
    void setVSync (bool enable);

private:
    void initialize(const WindowConfig& config);

    // Callback bei Fenster-Resize → passt glViewport an
    static void framebufferSizeCallback(GLFWwindow* window, int w, int h);

    GLFWwindow* m_window = nullptr;
    int         m_width  = 0;
    int         m_height = 0;
};

} // namespace ZE
