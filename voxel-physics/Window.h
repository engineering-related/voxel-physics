#ifndef WINDOW
#define WINDOW

#include "Math.h"
#include "Types.h"
#include "plog/Log.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <functional>
#include <glad/glad.h>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace engine {

const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;

enum class Event {
  MOUSE_LEFT_PRESSED,
  MOUSE_LEFT_RELEASED,
  
  MOUSE_MIDDLE_PRESSED,
  MOUSE_MIDDLE_RELEASED,

  MOUSE_RIGHT_PRESSED,
  MOUSE_RIGHT_RELEASED,

  MOUSE_WHEEL_SCROLLED_UP,
  MOUSE_WHEEL_SCROLLED_DOWN,

  WINDOW_RESIZED,


};

using Callback = std::function<void()>;

class Window {
public:
  Window(uint32_t width, uint32_t height, std::string title,
         bool fullScreen = false, bool vsync = false, bool resizeable = false, int samples = 4)
      : m_Width(width), m_Height(height), m_Title(title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    glfwWindowHint(GLFW_SAMPLES, samples);

    m_glfwWindow = glfwCreateWindow(
        m_Width, m_Height, title.c_str(),
        fullScreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (m_glfwWindow == NULL) {
      glfwTerminate();
      throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(m_glfwWindow);

    registerCallbackFunctions();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSwapInterval(vsync);
    glViewport(0, 0, m_Width, m_Height);
  }
  Window() = default;
  // HACK: Destroying OpenGL context segfaults at the end of the program
  ~Window() { /*glfwDestroyWindow(m_glfwWindow); glfwTerminate();*/ }

  void swapBuffers() { glfwSwapBuffers(m_glfwWindow); }

  void clear(const GLbitfield& clearBitMask) { glClear(clearBitMask); }

  GLFWwindow *getGlfwWindowPtr() const { return m_glfwWindow; }

  void setTitle(const std::string newTitle) {
    m_Title = newTitle;
    glfwSetWindowTitle(m_glfwWindow, m_Title.c_str());
  }

  bool shouldClose() const { return glfwWindowShouldClose(m_glfwWindow); }
  void pollEvents() const { glfwPollEvents(); };
  const uint32_t &getWidth() const { return m_Width; }
  const uint32_t &getHeight() const { return m_Height; }
  const uvec2 getSize() const {return {m_Width, m_Height}; }
  const std::string &getTitle() const { return m_Title; }

  const dvec2 &getCursorPos() {
    glfwGetCursorPos(m_glfwWindow, &m_CursorPos.x, &m_CursorPos.y);
    return m_CursorPos;
  }

  void addEventCallback(Event event, const Callback &callback) {
    m_Callbacks[event].push_back(callback);
  }

  void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height) {
    m_Width = width;
    m_Height = height;
    onWindowResized();
    glViewport(0, 0, m_Width, m_Height);
  }

  void onWindowResized() {
    for (Callback &callback : m_Callbacks[Event::WINDOW_RESIZED])
      callback();
  }

  void onMouseWheelScrolled(double y) {
    Event target = (y > 0) ? Event::MOUSE_WHEEL_SCROLLED_DOWN
                           : Event::MOUSE_WHEEL_SCROLLED_UP;

    for (Callback &callback : m_Callbacks[target])
      callback();
  }

  void onMouseButtonPressed(int button, int action) {
    Event target;
    // Pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
      target = Event::MOUSE_LEFT_PRESSED;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
      target = Event::MOUSE_RIGHT_PRESSED;
    if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
      target = Event::MOUSE_MIDDLE_PRESSED;

    for (Callback &callback : m_Callbacks[target])
      callback();
  }

  bool currently(const Event &targetEvent) {
    switch (targetEvent) {
    case Event::MOUSE_LEFT_PRESSED:
      return glfwGetMouseButton(m_glfwWindow, GLFW_MOUSE_BUTTON_LEFT) ==
             GLFW_PRESS;
      break;
    case Event::MOUSE_RIGHT_PRESSED:
      return glfwGetMouseButton(m_glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) ==
             GLFW_PRESS;
      break;
      case Event::MOUSE_MIDDLE_PRESSED:
      return glfwGetMouseButton(m_glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) ==
             GLFW_PRESS;
      break;
    default:
      assert(!"Event-type not implemented.");
      break;
    }
  }

private:
  void registerCallbackFunctions() {
    glfwSetWindowUserPointer(m_glfwWindow, this);
    auto resizeFunc = [](GLFWwindow *w, int width, int height) {
      static_cast<Window *>(glfwGetWindowUserPointer(w))
          ->framebufferSizeCallback(w, width, height);
    };
    glfwSetFramebufferSizeCallback(m_glfwWindow, resizeFunc);

    auto mScrollFunc = [](GLFWwindow *w, double x, double y) {
      static_cast<Window *>(glfwGetWindowUserPointer(w))
          ->onMouseWheelScrolled(y);
    };
    glfwSetScrollCallback(m_glfwWindow, mScrollFunc);

    auto mPressedFunc = [](GLFWwindow *w, int button, int action, int mods) {
      static_cast<Window *>(glfwGetWindowUserPointer(w))
          ->onMouseButtonPressed(button, action);
    };
    glfwSetMouseButtonCallback(m_glfwWindow, mPressedFunc);
  }

  uint32_t m_Width;
  uint32_t m_Height;
  std::string m_Title;
  GLFWwindow *m_glfwWindow;
  dvec2 m_CursorPos;
  std::map<Event, std::vector<Callback>> m_Callbacks;
};
} // namespace engine

#endif // !WINDOW
