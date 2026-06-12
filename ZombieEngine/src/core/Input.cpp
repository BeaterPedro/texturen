#include "Input.hpp"
#include <GLFW/glfw3.h>

namespace ZE {

// Globaler Zeiger – wird für die statischen GLFW-Callbacks benötigt
static Input* s_inputInstance = nullptr;

void Input::initialize(GLFWwindow* window) {
    m_window         = window;
    s_inputInstance  = this;

    // GLFW-Callbacks registrieren
    glfwSetKeyCallback        (window, Input::onKey);
    glfwSetMouseButtonCallback(window, Input::onMouseButton);
    glfwSetCursorPosCallback  (window, Input::onMouseMove);
    glfwSetScrollCallback     (window, Input::onScroll);
}

// ── Tastatur ─────────────────────────────────────────────────

bool Input::isKeyDown(int key) const {
    if (key < 0 || key >= MAX_KEYS) return false;
    return m_keys[static_cast<size_t>(key)];
}

bool Input::isKeyPressed(int key) const {
    if (key < 0 || key >= MAX_KEYS) return false;
    return m_keysPressed[static_cast<size_t>(key)];
}

bool Input::isKeyReleased(int key) const {
    if (key < 0 || key >= MAX_KEYS) return false;
    return m_keysReleased[static_cast<size_t>(key)];
}

// ── Maus-Buttons ─────────────────────────────────────────────

bool Input::isMouseButtonDown(int button) const {
    if (button < 0 || button >= MAX_BUTTONS) return false;
    return m_mouseButtons[static_cast<size_t>(button)];
}

bool Input::isMouseButtonPressed(int button) const {
    if (button < 0 || button >= MAX_BUTTONS) return false;
    return m_mouseButtonsPressed[static_cast<size_t>(button)];
}

// ── FPS-Maus ─────────────────────────────────────────────────

void Input::captureMouse(bool capture) {
    m_mouseCaptured = capture;
    glfwSetInputMode(
        m_window,
        GLFW_CURSOR,
        capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
    );
    // Nach dem Freigeben/Einfangen: kein Sprung im nächsten Frame
    if (capture) m_firstMouse = true;
}

// ── Frame-Ende ────────────────────────────────────────────────

void Input::endFrame() {
    // "Einmalige" Zustände zurücksetzen
    m_keysPressed.fill(false);
    m_keysReleased.fill(false);
    m_mouseButtonsPressed.fill(false);

    // Mausbewegung pro Frame auf 0 zurücksetzen
    m_mouseDelta  = {0.0f, 0.0f};
    m_scrollDelta = 0.0f;
}

// ── Statische GLFW-Callbacks ──────────────────────────────────

void Input::onKey(GLFWwindow*, int key, int /*scancode*/, int action, int /*mods*/) {
    if (!s_inputInstance) return;
    if (key < 0 || key >= MAX_KEYS) return;

    auto& inp = *s_inputInstance;
    auto  idx = static_cast<size_t>(key);

    if (action == GLFW_PRESS) {
        inp.m_keys[idx]        = true;
        inp.m_keysPressed[idx] = true;
    } else if (action == GLFW_RELEASE) {
        inp.m_keys[idx]         = false;
        inp.m_keysReleased[idx] = true;
    }
    // GLFW_REPEAT ignorieren (wir haben eigene kontinuierliche Abfrage)
}

void Input::onMouseButton(GLFWwindow*, int button, int action, int /*mods*/) {
    if (!s_inputInstance) return;
    if (button < 0 || button >= MAX_BUTTONS) return;

    auto& inp = *s_inputInstance;
    auto  idx = static_cast<size_t>(button);

    inp.m_mouseButtons[idx] = (action == GLFW_PRESS);
    if (action == GLFW_PRESS)
        inp.m_mouseButtonsPressed[idx] = true;
}

void Input::onMouseMove(GLFWwindow*, double x, double y) {
    if (!s_inputInstance) return;
    auto& inp = *s_inputInstance;

    glm::vec2 newPos{static_cast<float>(x), static_cast<float>(y)};

    if (inp.m_firstMouse) {
        // Ersten Frame ignorieren – verhindert den "Teleport"-Sprung beim
        // ersten Maus-Einfangen
        inp.m_lastMousePos = newPos;
        inp.m_firstMouse   = false;
    }

    inp.m_mouseDelta   = newPos - inp.m_lastMousePos;
    inp.m_mousePos     = newPos;
    inp.m_lastMousePos = newPos;
}

void Input::onScroll(GLFWwindow*, double /*xoff*/, double yoff) {
    if (!s_inputInstance) return;
    s_inputInstance->m_scrollDelta = static_cast<float>(yoff);
}

} // namespace ZE
