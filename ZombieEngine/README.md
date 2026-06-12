# ZombieEngine 🧟

Leichtgewichtige 3D-Game-Engine in C++17 mit OpenGL 4.1 — gebaut von Grund auf als Fundament für einen First-Person-Shooter (Call of Duty Zombie-Modus-Stil).

## Phase 1 – Was bisher enthalten ist

| System | Status | Beschreibung |
|--------|--------|-------------|
| Window Management | ✅ | GLFW-Fenster, OpenGL 4.1 Core Context |
| Input | ✅ | Tastatur (Down/Pressed/Released), Maus, Scroll |
| FPS-Kamera | ✅ | Maus-Capture, WASD + Q/E Höhe, Scroll-FOV |
| Delta-Time | ✅ | FPS-unabhängige Bewegung, FPS-Anzeige |
| Math-Layer | ✅ | GLM: Vec2/3/4, Mat4, lookAt, perspective |
| Shader | ✅ | Laden/Kompilieren von GLSL-Dateien, alle Uniforms |
| VAO/VBO/IBO | ✅ | Saubere Abstraktion für GPU-Buffer |
| Mesh | ✅ | Dreieck, Würfel, Plane (Factory-Methoden) |
| Renderer | ✅ | Depth-Test, Face-Culling, Wireframe, ClearColor |
| Lambert-Licht | ✅ | Einfache diffuse Beleuchtung im Fragment-Shader |

## Voraussetzungen

- **CMake** >= 3.20
- **C++17**-fähiger Compiler:
  - Windows: Visual Studio 2019/2022 oder MinGW-w64
  - Linux: GCC 9+ oder Clang 10+
  - macOS: Apple Clang (Xcode 12+)
- **Git** (für FetchContent)
- **Python** wird NICHT benötigt

> ℹ️ Abhängigkeiten (GLFW, GLM, GLAD2) werden automatisch von CMake heruntergeladen.

## Build-Anleitung

### Linux / macOS
```bash
git clone https://github.com/DEIN-USERNAME/ZombieEngine.git
cd ZombieEngine
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/bin/ZombieEngine
```

### Windows (Visual Studio)
```powershell
git clone https://github.com/DEIN-USERNAME/ZombieEngine.git
cd ZombieEngine
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\bin\Release\ZombieEngine.exe
```

### Windows (MinGW / MSYS2)
```bash
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/bin/ZombieEngine.exe
```

## Steuerung

| Taste/Aktion | Funktion |
|---|---|
| WASD | Vorwärts / Rückwärts / Links / Rechts |
| Maus | Umschauen (First-Person) |
| Q | Kamera nach unten |
| E | Kamera nach oben |
| Scroll-Rad | FOV vergrößern/verkleinern |
| F | Wireframe-Modus ein/aus |
| ESC | Beenden |

## Projektstruktur

```
ZombieEngine/
├── CMakeLists.txt              ← Build-System (FetchContent)
├── src/
│   ├── main.cpp                ← Entry Point & ZombieApp
│   ├── core/
│   │   ├── Engine.hpp/cpp      ← Haupt-Klasse & Game-Loop
│   │   ├── Window.hpp/cpp      ← GLFW-Fenster-Wrapper
│   │   ├── Input.hpp/cpp       ← Tastatur & Maus
│   │   └── Time.hpp/cpp        ← Delta-Time & FPS
│   ├── renderer/
│   │   ├── Renderer.hpp/cpp    ← OpenGL-Zustandsverwaltung
│   │   ├── Shader.hpp/cpp      ← GLSL laden & kompilieren
│   │   ├── Buffer.hpp/cpp      ← VAO, VBO, IBO
│   │   └── Mesh.hpp/cpp        ← Geometrie-Container
│   └── math/
│       └── Math.hpp            ← GLM-Typen & Hilfsfunktionen
├── shaders/
│   ├── basic.vert              ← Vertex-Shader (MVP-Transform)
│   └── basic.frag              ← Fragment-Shader (Lambert-Licht)
└── assets/
    └── models/                 ← Platz für 3D-Modelle (Phase 2)
```

## Nächste Schritte (Phase 2)

- [ ] **Textur-Loader** — `stb_image`-Integration, `Texture`-Klasse
- [ ] **OBJ-Loader** — Modelle aus deinem GitHub-Repo laden (`tinyobjloader`)
- [ ] **Kamera-Klasse** — saubere Entkopplung vom `onUpdate`
- [ ] **Transform-Klasse** — Position + Rotation + Scale pro Objekt
- [ ] **Entity/Component** — einfaches Szenen-System für Zombies & Spieler
- [ ] **Kollision** — AABB Bounding-Boxes für Schuss-Detection

## Abhängigkeiten (automatisch via CMake)

| Bibliothek | Version | Zweck |
|---|---|---|
| [GLFW](https://www.glfw.org/) | 3.4 | Fenster, Input, OpenGL-Kontext |
| [GLM](https://github.com/g-truc/glm) | 1.0.1 | Mathematik (Vektoren, Matrizen) |
| [GLAD2](https://github.com/Dav1dde/glad) | v2.0.7 | OpenGL 4.1 Function-Loader |
