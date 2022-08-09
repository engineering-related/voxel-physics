#ifndef CAMERA
#define CAMERA

#include "Math.h"
#include "Window.h"
#include "Easing.h"

// TODO: Infinite grid (antialiasing)
namespace engine {
class Camera {
public:
  Camera(Window* window,  vec3 center, vec3 upVector,
  float radius, float azimuthAngle, float polarAngle,
  mat4x4 projMatrix,
  float FOV, float aspectRatio, float nearPlane, float farPlane
) 
  {
    this->center = center;
    this->upVector = upVector;
    this->radius = radius;
    this->azimuthAngle = azimuthAngle;
    this->polarAngle = polarAngle;
    this->projMatrix = projMatrix;
    this->FOV = FOV;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    m_Window = window;
    registerCallbackFunctions();
    init();
  }

  void init() {
    m_TargetRadius = radius;
    m_TargetAzimut = azimuthAngle;
    m_TargetPolar = polarAngle;
    m_TargetCenter = center;

    m_ZoomEase = Ease<float>(radius, m_TargetRadius, 1.0f,
                             EaseFunction::OUT_EXPO);
    m_AzimutEase = Ease<float>(azimuthAngle, m_TargetAzimut, 1.0f,
                              EaseFunction::OUT_EXPO);
    m_PolarEase = Ease<float>(polarAngle, m_TargetPolar, 1.0f,
                              EaseFunction::OUT_EXPO);
    m_PositionEase = Ease<vec3>(center, m_TargetCenter, 1.0f, 
                              EaseFunction::OUT_EXPO);
  }

  void registerCallbackFunctions() {
    m_Window->addEventCallback(Event::MOUSE_WHEEL_SCROLLED_UP,
                       std::bind(&Camera::onScrollUp, this));
    m_Window->addEventCallback(Event::MOUSE_WHEEL_SCROLLED_DOWN,
                       std::bind(&Camera::onScrollDown, this));
    m_Window->addEventCallback(Event::MOUSE_LEFT_PRESSED,
                       std::bind(&Camera::onPressedLeft, this));
    m_Window->addEventCallback(Event::MOUSE_RIGHT_PRESSED,
                       std::bind(&Camera::onPressedRight, this));
  }

  void update(const float &deltaTime) {
    updateRotation(deltaTime);
    updatePanning(deltaTime);
    updateZoom(deltaTime);

    setViewMatrix();
  }

  void onScrollUp() {
    bool lastZoomPositive = m_IsZoomPositive;

    m_IsZoomPositive = true;
    if (!lastZoomPositive)
      m_ZoomVel = m_ZoomVelMin;
    m_TargetRadius = radius * m_ZoomVel;

    if (m_TargetRadius > farPlane - farPlane / 20.f)
      m_TargetRadius = farPlane - farPlane / 20.f;

    m_ZoomEase.target(m_TargetRadius);
    m_ZoomVel += m_ZoomAcc;
  }

  void onScrollDown() {
    bool lastZoomPositive = m_IsZoomPositive;

    m_IsZoomPositive = false;
    if (lastZoomPositive)
      m_ZoomVel = m_ZoomVelMin;
    m_TargetRadius = radius / m_ZoomVel;

    if (m_TargetRadius < 1.f)
      m_TargetRadius = 1.f;

    m_ZoomEase.target(m_TargetRadius);
    m_ZoomVel += m_ZoomAcc;
  }

  void onPressedLeft() {
    m_RotAnchor = m_Window->getCursorPos();
    m_StartAzimut = azimuthAngle;
    m_StartPolar = polarAngle;
  }

  void onPressedRight() {
    m_PosAncor = m_Window->getCursorPos();
    m_StartPosition = center;
  }

  void updateZoom(const float &deltaTime) {
    radius = m_ZoomEase.step(deltaTime);

    m_ZoomVel = clamp(m_ZoomVel, m_ZoomVelMin, m_ZoomVelMax);
    if (m_ZoomEase.percentCompleted() >= 0.25f)
      m_ZoomVel = m_ZoomVelMin;
    m_TargetRadius = max(0.1f, m_TargetRadius);
  }

  void updateRotation(const float &deltaTime) {
    // TODO: Constrain targetAngle
    if (m_Window->currently(Event::MOUSE_LEFT_PRESSED)) {
      vec2 targetAngle = {m_Window->getCursorPos() - m_RotAnchor};

      m_TargetAzimut = m_StartAzimut + targetAngle.x / 150.f;
      m_TargetPolar = m_StartPolar + targetAngle.y / 150.f;

      const auto polarCap = PI / 2.0f - 0.001f;
      if (m_TargetPolar > polarCap) {
        m_TargetPolar = polarCap;
      }

      if (m_TargetPolar < -polarCap) {
        m_TargetPolar = -polarCap;
      }
    }

    azimuthAngle = m_AzimutEase.targetStep(m_TargetAzimut, deltaTime);
    polarAngle = m_PolarEase.targetStep(m_TargetPolar, deltaTime);
  }

  void updatePanning(const float &deltaTime) {
    // TODO: Correctly map from screen -> world
    if (m_Window->currently(Event::MOUSE_RIGHT_PRESSED)) {
      vec2 targetPos = {m_Window->getCursorPos() - m_PosAncor};
      m_TargetCenter =
          m_StartPosition + (-targetPos.x * getRightVector() +
                             targetPos.y * getUpVector()) *
                                radius / 1080.f;
    }

    center = m_PositionEase.targetStep(m_TargetCenter, deltaTime);
  }

  vec3 getEye() {
    // Calculate sines / cosines of angles
    const auto sineAzimuth = sin(azimuthAngle);
    const auto cosineAzimuth = cos(azimuthAngle);
    const auto sinePolar = sin(polarAngle);
    const auto cosinePolar = cos(polarAngle);

    // Calculate eye position out of them
    const auto x = center.x + radius * cosinePolar * cosineAzimuth;
    const auto y = center.y + radius * sinePolar;
    const auto z = center.z + radius * cosinePolar * sineAzimuth;

    return vec3(x, y, z);
  }

  vec3 getRightVector() {
    const vec3 viewVector = getNormalizedViewVector();
    return normalize(cross(viewVector, upVector));
  }

  vec3 getUpVector() {
    const vec3 viewVector = getNormalizedViewVector();
    return normalize(cross(getRightVector(), viewVector));
  }

  vec3 getNormalizedViewVector() { return normalize(center - getEye()); }

  const mat4x4 getMatrix() const {
    return projMatrix * viewMatrix;
  }

private:
  void setViewMatrix() {
    viewMatrix = lookAt(getEye(), center, upVector);
  }

  Window* m_Window;

  // Data
  vec3 center;   // Center of the orbit camera sphere (the point upon which the
                 // camera looks)
  vec3 upVector; // Up vector of the camera
  float radius;  // Radius of the orbit camera sphere
  float azimuthAngle; // Azimuth angle on the orbit camera sphere
  float polarAngle;   // Polar angle on the orbit camera sphere

  mat4x4 projMatrix;
  float FOV;
  float aspectRatio;
  float nearPlane;
  float farPlane;
  mat4x4 viewMatrix;

  // Zooming
  Ease<float> m_ZoomEase;
  float m_TargetRadius;
  float m_ZoomVelMin = 1.1f;
  float m_ZoomVelMax = 1.8f;
  float m_ZoomVel = m_ZoomVelMin;
  float m_ZoomAcc = 0.2f;
  bool m_IsZoomPositive = true;

  // Rotating
  dvec2 m_RotAnchor;
  Ease<float> m_AzimutEase;
  Ease<float> m_PolarEase;
  float m_TargetAzimut;
  float m_TargetPolar;
  float m_StartAzimut;
  float m_StartPolar;

  // Panning
  Ease<vec3> m_PositionEase;
  dvec2 m_PosAncor;
  vec3 m_TargetCenter;
  vec3 m_StartPosition;
};
} // namespace engine

#endif // !CAMERA