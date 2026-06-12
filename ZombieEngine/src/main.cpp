// ─────────────────────────────────────────────────────────────────────────────
//  ZombieEngine – main.cpp  (Phase 1: Foundation)
//
//  Demonstriert alle vier Kern-Systeme:
//   ✓ Fenster & Input   (GLFW, Maus-Capture, Tastatur)
//   ✓ Math-Layer        (GLM: Vec3, Mat4, lookAt, perspective)
//   ✓ Renderer-Core     (Shader laden, VAO/VBO, Dreieck + Würfel zeichnen)
//   ✓ Delta-Time        (FPS-unabhängige Bewegung)
//
//  Steuerung:
//   WASD          → Vorwärts/Seitwärts bewegen
//   Maus          → Umschauen (First-Person)
//   Q / E         → Runter / Rauf
//   F             → Wireframe-Modus umschalten
//   ESC           → Beenden
// ─────────────────────────────────────────────────────────────────────────────

#include "core/Engine.hpp"
#include "core/Input.hpp"
#include "core/Time.hpp"
#include "core/Window.hpp"
#include "math/Math.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Shader.hpp"

#include <GLFW/glfw3.h> // Für GLFW_KEY_*-Konstanten
#include <iostream>
#include <memory>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
//  ZombieApp – unsere konkrete Engine-Subklasse
//  Überschreibt onInit / onUpdate / onRender / onShutdown
// ─────────────────────────────────────────────────────────────────────────────
class ZombieApp : public ZE::Engine {
public:
    explicit ZombieApp(const ZE::EngineConfig& cfg) : Engine(cfg) {}

protected:
    // ── Einmalige Initialisierung ─────────────────────────────────
    void onInit() override {
        // Shader aus Dateien laden
        m_shader = std::make_unique<ZE::Shader>(
            "shaders/basic.vert",
            "shaders/basic.frag"
        );

        // Test-Geometrie erstellen
        m_triangle = std::make_unique<ZE::Mesh>(ZE::Mesh::createTriangle());
        m_cube     = std::make_unique<ZE::Mesh>(ZE::Mesh::createCube());
        m_ground   = std::make_unique<ZE::Mesh>(ZE::Mesh::createPlane(20.0f));

        // Kamera-Startposition
        m_cameraPos   = {0.0f, 1.5f, 6.0f};
        m_cameraFront = {0.0f, 0.0f,-1.0f};

        // Maus für FPS-Steuerung einfangen
        getInput().captureMouse(true);

        // Hintergrundfarbe: dunkles Blau-Grau (Nacht-Atmosphäre)
        getRenderer().setClearColor({0.05f, 0.07f, 0.12f, 1.0f});

        std::cout << "\n[App] Szene bereit.\n";
        std::cout << "[App] Steuerung: WASD = Bewegen | Maus = Umschauen\n";
        std::cout << "[App] Q/E = Höhe | F = Wireframe | ESC = Beenden\n\n";
    }

    // ── Spiellogik (jeden Frame) ──────────────────────────────────
    void onUpdate(float dt) override {
        auto& input = getInput();

        // ── FPS-Kamera berechnen ────────────────────────────────
        const float lookSpeed = 0.12f;
        ZE::Math::Vec2 delta  = input.getMouseDelta();

        // Yaw (links/rechts) und Pitch (oben/unten) aktualisieren
        m_yaw   += delta.x * lookSpeed;
        m_pitch -= delta.y * lookSpeed;
        // Pitch begrenzen: Kamera kann nicht überschlagen
        m_pitch = glm::clamp(m_pitch, -88.0f, 88.0f);

        // Richtungsvektoren aus Euler-Winkeln berechnen
        const float yawRad   = glm::radians(m_yaw);
        const float pitchRad = glm::radians(m_pitch);

        ZE::Math::Vec3 front {
            std::cos(yawRad) * std::cos(pitchRad),
            std::sin(pitchRad),
            std::sin(yawRad) * std::cos(pitchRad)
        };
        m_cameraFront = glm::normalize(front);

        // Rechts-Vektor: Kreuzprodukt von Front und Welt-Oben
        ZE::Math::Vec3 right = glm::normalize(
            glm::cross(m_cameraFront, ZE::Math::Vec3(0.0f, 1.0f, 0.0f))
        );

        // ── Bewegung (WASD + Q/E) ────────────────────────────────
        const float speed = 5.0f * dt; // Einheiten pro Sekunde

        if (input.isKeyDown(GLFW_KEY_W)) m_cameraPos += m_cameraFront * speed;
        if (input.isKeyDown(GLFW_KEY_S)) m_cameraPos -= m_cameraFront * speed;
        if (input.isKeyDown(GLFW_KEY_A)) m_cameraPos -= right          * speed;
        if (input.isKeyDown(GLFW_KEY_D)) m_cameraPos += right          * speed;
        if (input.isKeyDown(GLFW_KEY_E)) m_cameraPos.y += speed; // hoch
        if (input.isKeyDown(GLFW_KEY_Q)) m_cameraPos.y -= speed; // runter

        // ── Scroll-Rad: FOV ändern ────────────────────────────────
        m_fov -= input.getScrollDelta() * 2.0f;
        m_fov  = glm::clamp(m_fov, 20.0f, 110.0f);

        // ── Wireframe-Modus umschalten ────────────────────────────
        if (input.isKeyPressed(GLFW_KEY_F)) {
            m_wireframe = !m_wireframe;
            getRenderer().setWireframe(m_wireframe);
            std::cout << "[App] Wireframe: " << (m_wireframe ? "AN" : "AUS") << "\n";
        }

        // ── Fenstertitel mit FPS aktualisieren (jede Viertelsekunde) ─
        m_titleTimer += dt;
        if (m_titleTimer >= 0.25f) {
            float fps = getTime().getFPS();
            int   ms  = static_cast<int>(dt * 1000.0f);
            getWindow().setTitle(
                "ZombieEngine  |  FPS: " + std::to_string(static_cast<int>(fps)) +
                "  |  " + std::to_string(ms) + " ms  |  F=Wireframe ESC=Exit"
            );
            m_titleTimer = 0.0f;
        }
    }

    // ── Rendering (jeden Frame) ───────────────────────────────────
    void onRender() override {
        float t = static_cast<float>(glfwGetTime());

        // ── View-Matrix: Kameraposition und -richtung ─────────────
        ZE::Math::Mat4 view = ZE::Math::lookAt(
            m_cameraPos,
            m_cameraPos + m_cameraFront,
            ZE::Math::Vec3(0.0f, 1.0f, 0.0f) // Oben = Y-Achse
        );

        // ── Projection-Matrix: Perspektive ────────────────────────
        ZE::Math::Mat4 proj = ZE::Math::perspective(
            m_fov,                    // Vertikales Sichtfeld (Grad)
            getWindow().getAspect(),  // Seitenverhältnis (z.B. 16/9)
            0.01f,                    // Near-Plane (sehr nah → vermeidet Z-Clipping)
            1000.0f                   // Far-Plane
        );

        // View und Projection einmal pro Frame setzen
        m_shader->bind();
        m_shader->setMat4 ("uView",       view);
        m_shader->setMat4 ("uProjection", proj);
        m_shader->setFloat("uTime",       t);

        // ── Rotierendes Dreieck (links) ───────────────────────────
        ZE::Math::Mat4 triModel{1.0f}; // Einheitsmatrix
        triModel = ZE::Math::translate(triModel, {-3.0f, 0.5f, 0.0f});
        triModel = ZE::Math::rotate   (triModel, t * 60.0f, {0.0f, 1.0f, 0.0f});
        m_shader->setVec3("uColor", {1.0f, 0.5f, 0.2f}); // Orange
        getRenderer().drawMesh(*m_triangle, *m_shader, triModel);

        // ── Taumelnder Würfel (rechts) ────────────────────────────
        ZE::Math::Mat4 cubeModel{1.0f};
        cubeModel = ZE::Math::translate(cubeModel, {3.0f, 0.5f, 0.0f});
        cubeModel = ZE::Math::rotate   (cubeModel, t * 40.0f, {1.0f, 1.0f, 0.5f});
        m_shader->setVec3("uColor", {0.3f, 0.6f, 1.0f}); // Hellblau
        getRenderer().drawMesh(*m_cube, *m_shader, cubeModel);

        // ── Zweiter Würfel (Mitte, langsam) ──────────────────────
        ZE::Math::Mat4 cubeModel2{1.0f};
        cubeModel2 = ZE::Math::translate(cubeModel2, {0.0f, 1.0f, -3.0f});
        cubeModel2 = ZE::Math::rotate   (cubeModel2, t * 20.0f, {0.0f, 1.0f, 0.0f});
        cubeModel2 = ZE::Math::scale    (cubeModel2, {1.5f, 1.5f, 1.5f});
        m_shader->setVec3("uColor", {0.9f, 0.85f, 0.3f}); // Gold
        getRenderer().drawMesh(*m_cube, *m_shader, cubeModel2);

        // ── Boden ─────────────────────────────────────────────────
        ZE::Math::Mat4 groundModel{1.0f};
        groundModel = ZE::Math::translate(groundModel, {0.0f, -0.5f, 0.0f});
        m_shader->setVec3("uColor", {0.18f, 0.30f, 0.12f}); // Dunkelgrün
        getRenderer().drawMesh(*m_ground, *m_shader, groundModel);
    }

    // ── Aufräumen ─────────────────────────────────────────────────
    void onShutdown() override {
        // smart-pointers räumen automatisch auf, aber explizit ist sauberer
        m_triangle.reset();
        m_cube.reset();
        m_ground.reset();
        m_shader.reset();
        std::cout << "[App] Ressourcen freigegeben.\n";
    }

private:
    // Rendering-Ressourcen
    std::unique_ptr<ZE::Shader> m_shader;
    std::unique_ptr<ZE::Mesh>   m_triangle;
    std::unique_ptr<ZE::Mesh>   m_cube;
    std::unique_ptr<ZE::Mesh>   m_ground;

    // FPS-Kamera-Zustand
    ZE::Math::Vec3 m_cameraPos  {0.0f, 1.5f, 6.0f};
    ZE::Math::Vec3 m_cameraFront{0.0f, 0.0f,-1.0f};
    float          m_yaw        = -90.0f; // -90° = schaut Richtung -Z
    float          m_pitch      =   0.0f;
    float          m_fov        =  60.0f;

    // Hilfsvariablen
    bool  m_wireframe  = false;
    float m_titleTimer = 0.0f;
};

// ─────────────────────────────────────────────────────────────────────────────
//  main()
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    try {
        ZE::EngineConfig cfg;
        cfg.width  = 1280;
        cfg.height = 720;
        cfg.title  = "ZombieEngine  |  Initialisiere...";
        cfg.vsync  = true;

        ZombieApp app(cfg);
        app.run();

    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL ERROR] " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
