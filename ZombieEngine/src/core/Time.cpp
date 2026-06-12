#include "Time.hpp"
#include <algorithm>

namespace ZE {

void Time::tick(double currentTime) {
    // Rohe deltaTime berechnen
    m_deltaTime    = static_cast<float>(currentTime - m_lastFrameTime);
    m_lastFrameTime = currentTime;
    m_totalTime     = currentTime;

    // FPS-Zähler (wird jede Sekunde aktualisiert)
    m_fpsCount++;
    m_fpsTimer += m_deltaTime;
    if (m_fpsTimer >= 1.0) {
        m_fps      = static_cast<float>(m_fpsCount) / static_cast<float>(m_fpsTimer);
        m_fpsCount = 0;
        m_fpsTimer = 0.0;
    }
}

float Time::getDeltaTimeCapped() const {
    // Wenn der Debugger hält oder das Fenster minimiert war,
    // kann deltaTime sehr groß sein → auf MAX_DELTA begrenzen.
    return std::min(m_deltaTime, MAX_DELTA);
}

} // namespace ZE
