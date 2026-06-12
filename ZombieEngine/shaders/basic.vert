#version 410 core

// ─────────────────────────────────────────────────────────────
//  Vertex-Shader – basic.vert
//  Läuft einmal pro Vertex auf der GPU.
//  Transformiert Positionen von Modell-Raum → Clip-Raum.
// ─────────────────────────────────────────────────────────────

// ── Eingabe (aus VBO, Attribute-Locations aus Buffer.cpp) ────
layout(location = 0) in vec3 aPosition;  // Weltkoordinaten
layout(location = 1) in vec3 aColor;     // Vertex-Farbe
layout(location = 2) in vec2 aTexCoord;  // UV-Koordinaten
layout(location = 3) in vec3 aNormal;    // Oberflächen-Normale

// ── Uniforms (per Draw-Call von CPU gesetzt) ─────────────────
uniform mat4  uModel;      // Objekt → Weltkoordinaten
uniform mat4  uView;       // Welt → Kamera-Raum
uniform mat4  uProjection; // Kamera → Clip-Raum (Perspektive)
uniform float uTime;       // Laufzeit in Sekunden (für Animationen)

// ── Ausgabe an Fragment-Shader ───────────────────────────────
out vec3 vColor;      // Weitergereichte Vertex-Farbe
out vec2 vTexCoord;   // Weitergereichte UV
out vec3 vNormal;     // Transformierte Normale (für Beleuchtung)
out vec3 vWorldPos;   // Weltposition (für Lichtberechnungen)

void main() {
    // Model → Welt
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vWorldPos  = worldPos.xyz;

    // Normale transformieren:
    // Transpose(Inverse(Model))-Matrix damit Skalierungen die Normale
    // nicht verzerren. In Phase 2 als eigene Uniform übergeben (Performance).
    vNormal   = mat3(transpose(inverse(uModel))) * aNormal;

    // Direkt weitergeben (kein Wert, der die Transform braucht)
    vColor    = aColor;
    vTexCoord = aTexCoord;

    // Finale Clip-Raum-Position
    gl_Position = uProjection * uView * worldPos;
}
