#pragma once
#include <array>
#include <glm/glm.hpp>

struct GLFWwindow; // Forward-Declaration – kein #include nötig im Header

namespace ZE {

// ─────────────────────────────────────────────────────────────
//  Input – Tastatur, Maus und Scroll-Rad
//
//  Drei Abfragemodi (analog zu Unity/Unreal):
//
//   isKeyDown(key)      → true, solange Taste gedrückt ist
//   isKeyPressed(key)   → true NUR im ersten Frame des Drückens
//   isKeyReleased(key)  → true NUR im ersten Frame des Loslassens
//
//  FPS-Maus:
//   captureMouse(true)  → Maus wird unsichtbar & mittig gesperrt
//   getMouseDelta()     → Bewegung seit letztem Frame (für Rotation)
//
//  Verwendung (GLFW Key-Konstanten, z.B. GLFW_KEY_W):
//    if (input.isKeyDown(GLFW_KEY_W))     pos += forward * speed * dt;
//    if (input.isKeyPressed(GLFW_KEY_F))  toggleWireframe();
// ─────────────────────────────────────────────────────────────
class Input {
public:
    void initialize(GLFWwindow* window);

    // ── Tastatur ─────────────────────────────────────────────
    bool isKeyDown    (int key) const;
    bool isKeyPressed (int key) const; // nur 1 Frame lang true
    bool isKeyReleased(int key) const; // nur 1 Frame lang true

    // ── Maus-Buttons ─────────────────────────────────────────
    bool isMouseButtonDown(int button) const;
    bool isMouseButtonPressed(int button) const;

    // ── Maus-Position & -Bewegung ────────────────────────────
    glm::vec2 getMousePosition() const { return m_mousePos; }
    glm::vec2 getMouseDelta()    const { return m_mouseDelta; }
    float     getScrollDelta()   const { return m_scrollDelta; }

    // ── FPS-Maus (Cursor sperren) ────────────────────────────
    void captureMouse(bool capture);
    bool isMouseCaptured() const { return m_mouseCaptured; }

    // Am Ende jedes Frames aufrufen (setzt "Pressed"-Flags zurück)
    void endFrame();

    // ── GLFW-Callbacks (intern) ──────────────────────────────
    // Werden von GLFW aufgerufen; leiten an Singleton-Instanz weiter
    static void onKey        (GLFWwindow*, int key, int scancode, int action, int mods);
    static void onMouseButton(GLFWwindow*, int button, int action, int mods);
    static void onMouseMove  (GLFWwindow*, double x, double y);
    static void onScroll     (GLFWwindow*, double xoff, double yoff);

private:
    static constexpr int MAX_KEYS    = 512;
    static constexpr int MAX_BUTTONS = 8;

    GLFWwindow* m_window       = nullptr;
    bool        m_mouseCaptured = false;
    bool        m_firstMouse    = true;

    // Tastatur-Zustände (Index = GLFW_KEY_*)
    std::array<bool, MAX_KEYS> m_keys{};
    std::array<bool, MAX_KEYS> m_keysPressed{};
    std::array<bool, MAX_KEYS> m_keysReleased{};

    // Maus-Button-Zustände
    std::array<bool, MAX_BUTTONS> m_mouseButtons{};
    std::array<bool, MAX_BUTTONS> m_mouseButtonsPressed{};

    // Maus-Positions- und Bewegungsdaten
    glm::vec2 m_mousePos     {0.0f, 0.0f};
    glm::vec2 m_lastMousePos {0.0f, 0.0f};
    glm::vec2 m_mouseDelta   {0.0f, 0.0f};
    float     m_scrollDelta  = 0.0f;
};

} // namespace ZE
