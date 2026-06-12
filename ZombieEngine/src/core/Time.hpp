#pragma once

namespace ZE {

// ─────────────────────────────────────────────────────────────
//  Time – Zeitmanagement der Engine
//
//  Warum deltaTime?
//  Ohne deltaTime würde sich ein Charakter auf einem 30-FPS-
//  Rechner halb so schnell bewegen wie auf einem 60-FPS-Rechner.
//  Durch Multiplikation aller Bewegungen mit deltaTime (Sekunden
//  pro Frame) ist die Geschwindigkeit FPS-unabhängig.
//
//  Verwendung:
//    float speed = 5.0f;           // Einheiten pro Sekunde
//    pos += direction * speed * dt; // dt = deltaTime
// ─────────────────────────────────────────────────────────────
class Time {
public:
    // Einmal pro Frame aufrufen, mit glfwGetTime() als Argument
    void tick(double currentTime);

    // Sekunden seit letztem Frame (z.B. 0.016 bei 60 FPS)
    float getDeltaTime() const { return m_deltaTime; }

    // Geglättete Frames-pro-Sekunde (Update jede Sekunde)
    float getFPS() const { return m_fps; }

    // Gesamte Laufzeit seit Engine-Start (in Sekunden)
    double getTotalTime() const { return m_totalTime; }

    // Capped deltaTime – verhindert riesige Sprünge nach Pausen/Debugging
    float getDeltaTimeCapped() const;

private:
    double m_lastFrameTime = 0.0;
    float  m_deltaTime     = 0.0f;
    double m_totalTime     = 0.0;
    float  m_fps           = 0.0f;

    // FPS-Zähler
    double m_fpsTimer = 0.0;
    int    m_fpsCount = 0;

    // Maximal erlaubte deltaTime (verhindert "Zeitsprünge")
    static constexpr float MAX_DELTA = 0.05f; // ~20 FPS minimum
};

} // namespace ZE
