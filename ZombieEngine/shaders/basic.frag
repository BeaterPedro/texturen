#version 410 core

// ─────────────────────────────────────────────────────────────
//  Fragment-Shader – basic.frag
//  Läuft einmal pro Pixel auf der GPU.
//  Berechnet Endfarbe mit einfachem Lambert-Diffuse-Licht.
// ─────────────────────────────────────────────────────────────

// ── Eingabe (interpoliert vom Vertex-Shader) ─────────────────
in vec3 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

// ── Uniforms ─────────────────────────────────────────────────
uniform vec3  uColor;     // Basis-Farbe des Objekts (per drawMesh gesetzt)
uniform float uTime;      // Laufzeit (für spätere Shader-Effekte)

// ── Ausgabe ──────────────────────────────────────────────────
out vec4 FragColor;

void main() {
    // ── Einfache Lambert-Beleuchtung ─────────────────────────
    // Direktionales Licht von oben-rechts-vorne
    vec3  lightDir  = normalize(vec3(1.0, 2.5, 1.5));
    vec3  normal    = normalize(vNormal);

    // Lambert: Je senkrechter die Normale zum Licht, desto heller
    float diffuse   = max(dot(normal, lightDir), 0.0);
    float ambient   = 0.20; // Grundhelligkeit (kein totales Schwarz)

    // Gesamthelligkeit: Ambient + Diffuse-Anteil
    float lighting  = ambient + diffuse * 0.80;

    // Basis-Farbe mit Beleuchtung kombinieren
    vec3 litColor   = uColor * lighting;

    // Vertex-Farben leicht einmischen (sichtbar bei Dreieck)
    vec3 finalColor = mix(litColor, litColor * vColor, 0.25);

    FragColor = vec4(finalColor, 1.0);

    // ── Für spätere Erweiterungen (auskommentiert): ──────────
    // Textur-Sampling:  vec4 tex = texture(uTexture, vTexCoord);
    // Specular-Licht:   float spec = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0);
    // Fog:              float fogFactor = exp(-fogDensity * length(vWorldPos - uCameraPos));
}
