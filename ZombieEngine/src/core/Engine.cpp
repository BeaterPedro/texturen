#include "Engine.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "../renderer/Renderer.hpp"

// glad vor GLFW includieren
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace ZE {

Engine* Engine::s_instance = nullptr;

Engine::Engine(const EngineConfig& config)
    : m_config(config)
{
    if (s_instance) {
        throw std::runtime_error(
            "[Engine] Nur eine Engine-Instanz gleichzeitig erlaubt!"
        );
    }
    s_instance = this;
}

Engine::~Engine() {
    s_instance = nullptr;
}

// ─────────────────────────────────────────────────────────────
//  Öffentliche Schnittstelle
// ─────────────────────────────────────────────────────────────

void Engine::run() {
    initialize();
    onInit();
    mainLoop();
    onShutdown();
    shutdown();
}

// ─────────────────────────────────────────────────────────────
//  Private Implementierung
// ─────────────────────────────────────────────────────────────

void Engine::initialize() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════╗\n";
    std::cout << "║   ZombieEngine v0.1  –  Starte Engine...  ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n\n";

    // ── 1. Fenster erstellen (initialisiert GLFW + OpenGL-Kontext) ──
    WindowConfig winCfg;
    winCfg.width     = m_config.width;
    winCfg.height    = m_config.height;
    winCfg.title     = m_config.title;
    winCfg.vsync     = m_config.vsync;
    m_window = std::make_unique<Window>(winCfg);

    // ── 2. GLAD – OpenGL-Funktionszeiger laden ──────────────────────
    // Muss nach glfwMakeContextCurrent() aufgerufen werden!
    if (!gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress))) {
        throw std::runtime_error("[Engine] GLAD-Initialisierung fehlgeschlagen!");
    }
    std::cout << "[Engine] OpenGL " << glGetString(GL_VERSION) << "\n";
    std::cout << "[Engine] GPU: "   << glGetString(GL_RENDERER) << "\n\n";

    // ── 3. Sub-Systeme initialisieren ───────────────────────────────
    m_input    = std::make_unique<Input>();
    m_renderer = std::make_unique<Renderer>();
    m_time     = std::make_unique<Time>();

    m_input->initialize   (m_window->getNativeWindow());
    m_renderer->initialize();

    // Initialen Viewport setzen
    glViewport(0, 0, m_config.width, m_config.height);

    m_running = true;

    std::cout << "[Engine] Bereit. Starte Game-Loop.\n";
    std::cout << "─────────────────────────────────────────────\n\n";
}

void Engine::mainLoop() {
    // ─────────────────────────────────────────────────────────
    //  THE GAME LOOP
    //  Reihenfolge ist entscheidend:
    //  Zeit → Events → Update → Render → Swap → Input-Reset
    // ─────────────────────────────────────────────────────────
    while (m_running && !m_window->shouldClose()) {

        // ── Zeit ────────────────────────────────────────────
        double now = glfwGetTime();
        m_time->tick(now);
        float dt = m_time->getDeltaTimeCapped(); // capped → kein Zeitsprung

        // ── Events (OS: Resize, Close-Button, etc.) ──────────
        m_window->pollEvents();

        // ESC schließt standardmäßig das Fenster
        if (m_input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            m_running = false;
            continue;
        }

        // ── Update (Spiellogik) ───────────────────────────────
        onUpdate(dt);

        // ── Render ────────────────────────────────────────────
        m_renderer->beginFrame();
        onRender();
        m_renderer->endFrame();

        // ── Swap (fertigen Frame anzeigen) ────────────────────
        m_window->swapBuffers();

        // ── Input-Zustand für nächsten Frame vorbereiten ──────
        m_input->endFrame();
    }
}

void Engine::shutdown() {
    std::cout << "\n[Engine] Wird heruntergefahren...\n";

    // Umgekehrte Reihenfolge der Initialisierung!
    m_renderer.reset(); // OpenGL-Ressourcen zuerst freigeben
    m_input.reset();
    m_time.reset();
    m_window.reset();   // glfwTerminate() passiert im Window-Destruktor

    std::cout << "[Engine] Auf Wiedersehen!\n";
}

} // namespace ZE
