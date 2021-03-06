#include "Window.h"
#include "../utils/Console.h"
#include "../input/Keyboard.h"
#include "input/Mouse.h"
#include <iostream>

#include "utils/Logger.h"

Window::Window(int width, int height, const char* title) {
    // Initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    // Temporary, in the future probably support different resolutions?
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(width, height, title, nullptr,
                                nullptr);
    if (m_window == nullptr) {
        // TODO: Error handling someday
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    m_width = width;
    m_height = height;

    glfwMakeContextCurrent(m_window);
    if (glewInit() != GLEW_OK) {
        // TODO: Error handling someday
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    std::string glVersion = "OpenGL Version: " +
                            std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    Logger::Log(glVersion);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

    glfwSwapInterval(1);

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::Open() {
    return !glfwWindowShouldClose(m_window);
}

void Window::Update() {
    glfwPollEvents();
    Keyboard::Update();
	Mouse::Update();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SetColor(Color color) {
    glClearColor(color.r / 255.f, color.g / 255.f,
                 color.b / 255.f, color.a / 255.f);
}

static void key_callback(GLFWwindow* window, int key,
                         int scancode, int action, int mods) {
    //if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
    //  Console::ToggleDebug();
    if (action == GLFW_PRESS)
        Keyboard::__hardwareKeyPress(key);
    if (action == GLFW_RELEASE)
        Keyboard::__hardwareKeyRelease(key);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Mouse::SetPosition(glm::vec2(xpos, ypos));
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Mouse::SetLeft(true);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		Mouse::SetLeft(false);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Mouse::SetScrollOffset(glm::vec2(xoffset, yoffset));
}