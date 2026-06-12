#pragma once
#include <memory>
#include <string>

namespace ZE {

class Window;
class Input;
class Renderer;
class Time;

struct EngineConfig {
    int         width  = 1280;
    int         height = 720;
    std::string title  = "ZombieEngine v0.1";
    bool        vsync  = true;
};

// ─────────────────────────────────────────────────────────────
//  Engine – Herzstück der ZombieEngine
//
//  Koordiniert alle Sub-Systeme und implementiert die Game-Loop:
//
//  ┌─ initialize() ──────────────────────────────────────────┐
//  │  Window erstellen → GLAD laden → Sub-Systeme starten    │
//  └──────────────────────────────────────────────────────────┘
//  ┌─ mainLoop() [bis shouldClose()] ────────────────────────┐
//  │  Time::tick()        ← deltaTime berechnen              │
//  │  Window::pollEvents()← OS-Events verarbeiten            │
//  │  onUpdate(dt)        ← Spiellogik (überschreibbar)      │
//  │  Renderer::beginFrame()                                  │
//  │  onRender()          ← Zeichnen (überschreibbar)        │
//  │  Renderer::endFrame()                                    │
//  │  Window::swapBuffers()← Frame anzeigen                  │
//  │  Input::endFrame()   ← "Pressed"-Flags zurücksetzen     │
//  └──────────────────────────────────────────────────────────┘
//
//  Um eine eigene App zu erstellen, Engine ableiten und
//  onInit/onUpdate/onRender/onShutdown überschreiben.
// ─────────────────────────────────────────────────────────────
class Engine {
public:
    explicit Engine(const EngineConfig& config = {});
    virtual ~Engine();

    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

    // Startet Game-Loop (blockiert bis Fenster geschlossen)
    void run();

    // ── Sub-System-Zugriff ───────────────────────────────────
    Window&   getWindow()   { return *m_window; }
    Input&    getInput()    { return *m_input; }
    Renderer& getRenderer() { return *m_renderer; }
    Time&     getTime()     { return *m_time; }

    // ── Globaler Singleton-Zugriff ───────────────────────────
    static Engine& get() { return *s_instance; }

    bool isRunning() const { return m_running; }
    void quit()            { m_running = false; }

protected:
    // ── Überschreibbare Hooks ────────────────────────────────
    virtual void onInit()           {}             // Einmalig nach Init
    virtual void onUpdate(float dt) { (void)dt; }  // Jeden Frame (Logik)
    virtual void onRender()         {}             // Jeden Frame (Grafik)
    virtual void onShutdown()       {}             // Vor Aufräumen

private:
    void initialize();
    void mainLoop();
    void shutdown();

    EngineConfig m_config;
    bool         m_running = false;

    std::unique_ptr<Window>   m_window;
    std::unique_ptr<Input>    m_input;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Time>     m_time;

    static Engine* s_instance;
};

} // namespace ZE
