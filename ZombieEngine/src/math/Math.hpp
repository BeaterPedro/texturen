#pragma once

// ─────────────────────────────────────────────────────────────
//  ZombieEngine – Math Layer
//  Zentrale Anlaufstelle für alle Vektor-/Matrixberechnungen.
//  Nutzt GLM (OpenGL Mathematics) als Unterbau.
//
//  Wichtigste Typen für den Shooter-Kontext:
//    Vec3     → Position, Richtung, Geschwindigkeit
//    Mat4     → Model/View/Projection-Matrizen
//    Quat     → Rotationen ohne Gimbal-Lock (wichtig für FPS-Kamera)
// ─────────────────────────────────────────────────────────────

// GLM-GTX-Extensions sind als "experimentell" markiert.
// Dieses Define schaltet sie frei – MUSS vor allen GLM-Includes stehen.
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // translate, rotate, scale, perspective, lookAt
#include <glm/gtc/type_ptr.hpp>          // value_ptr → Übergabe an OpenGL-Uniforms
#include <glm/gtc/constants.hpp>         // pi<float>()
#include <glm/gtx/rotate_vector.hpp>     // rotateX/Y/Z für Vektoren
#include <glm/gtx/quaternion.hpp>        // Quaternion-Operationen
#include <glm/gtx/norm.hpp>              // length2 (schnell, kein Wurzelziehen)

namespace ZE::Math {

// ── Typ-Aliases ──────────────────────────────────────────────
// Kürzel, damit man nicht überall "glm::" schreiben muss

using Vec2 = glm::vec2;   // 2D-Vektor  (z.B. UV, Maus-Position)
using Vec3 = glm::vec3;   // 3D-Vektor  (Position, Farbe, Normale)
using Vec4 = glm::vec4;   // 4D-Vektor  (homogene Koordinaten, RGBA)
using Mat3 = glm::mat3;   // 3×3-Matrix (Normalmatrix)
using Mat4 = glm::mat4;   // 4×4-Matrix (Model/View/Projection)
using Quat = glm::quat;   // Quaternion (Rotation ohne Gimbal-Lock)
using IVec2 = glm::ivec2; // Integer 2D (Fenstergröße, Textur-Koordinaten)

// ── Konstanten ───────────────────────────────────────────────
constexpr float PI     = glm::pi<float>();
constexpr float TWO_PI = glm::two_pi<float>();
constexpr float HALF_PI = glm::half_pi<float>();

// ── Matrix-Fabrik-Funktionen ─────────────────────────────────
// Wrappers um GLM, damit der Aufruf kürzer wird

inline Mat4 translate(const Mat4& m, const Vec3& v) {
    return glm::translate(m, v);
}
inline Mat4 rotate(const Mat4& m, float angleDeg, const Vec3& axis) {
    return glm::rotate(m, glm::radians(angleDeg), axis);
}
inline Mat4 scale(const Mat4& m, const Vec3& v) {
    return glm::scale(m, v);
}

// Perspektivische Projektion (für 3D-Szene)
// fovDeg: vertikales Sichtfeld in Grad (typisch 60°–90°)
inline Mat4 perspective(float fovDeg, float aspect, float nearP, float farP) {
    return glm::perspective(glm::radians(fovDeg), aspect, nearP, farP);
}

// View-Matrix: Kamera schaut von 'eye' auf 'center', 'up' zeigt nach oben
inline Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    return glm::lookAt(eye, center, up);
}

// ── Winkelumrechnung ─────────────────────────────────────────
inline float radians(float deg) { return glm::radians(deg); }
inline float degrees(float rad) { return glm::degrees(rad); }

// ── Vektor-Hilfsfunktionen ───────────────────────────────────
inline Vec3 normalize(const Vec3& v)              { return glm::normalize(v); }
inline Vec3 cross(const Vec3& a, const Vec3& b)   { return glm::cross(a, b); }
inline float dot(const Vec3& a, const Vec3& b)    { return glm::dot(a, b); }
inline float length(const Vec3& v)                { return glm::length(v); }
inline float length2(const Vec3& v)               { return glm::length2(v); }

// Zeiger auf ersten Wert → für glUniformMatrix4fv
inline const float* ptr(const Mat4& m)  { return glm::value_ptr(m); }
inline const float* ptr(const Vec3& v)  { return glm::value_ptr(v); }
inline const float* ptr(const Vec4& v)  { return glm::value_ptr(v); }

} // namespace ZE::Math