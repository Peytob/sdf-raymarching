#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/Canvas.hpp>
#include <sdfraymarching/render/ShaderStorageBuffer.hpp>
#include <sdfraymarching/render/OpenGLRenderCreatingException.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/Camera.hpp>
#include <sdfraymarching/render/ShaderStorageBuffer.hpp>

#include <sdfraymarching/scene/Scene.hpp>

#include "OpenGLRender.hpp"

void initializeGlfw() {
    if (glfwInit() != GLFW_TRUE) {
        const char* errorDescription;
        const int errorCode = glfwGetError(&errorDescription);

        std::string message =
            "Error while GLFW initializating! Code: " + std::to_string(errorCode) +
            "; description: " + std::string(errorDescription);

        throw OpenGLRenderCreatingException(message);
    }
}

GLFWwindow* createWindow(int width, int height, const std::string& title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (nullptr == window) {
        const char* description;
        glfwGetError(&description);

        std::string message = "Error while creating GLFW window. " + std::string(description);

        throw OpenGLRenderCreatingException(message);
    }

    return window;
}

void initializeGlew() {
    glewExperimental = GL_TRUE;
    const GLenum glewInitializationCode = glewInit();
    if (glewInitializationCode != GLEW_OK) {
        std::string message =
            "Error while GLEW initialzing!. "
            + std::string((const char*) glewGetErrorString(glewInitializationCode));

        throw OpenGLRenderCreatingException(message);
    }
}

OpenGLRender::OpenGLRender(int width, int height, const std::string& title) :
    window(nullptr),
    defaultCursorPosition(width / 2, height / 2) {

    Logger::info("Initializing OpenGL rendering engine.");

    Logger::info("Initializing GLFW library.");
    initializeGlfw();
    Logger::info("GLFW library initialized.");

    Logger::info("Creating new GLFW window.");
    this->window = createWindow(width, height, title);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    Logger::info("New GLFW window created.");

    Logger::info("Initializing GLEW library.");
    initializeGlew();
    Logger::info("GLEW library initialized.");

    canvas = new Canvas();
    sceneBuffer = new ShaderStorageBuffer(2);
}

OpenGLRender::~OpenGLRender() {
    Logger::info("Destroying OpenGL rendering engine.");

    Logger::info("Destroying GLFW window");
    glfwDestroyWindow(window);
    Logger::info("GLFW window destroyed.");

    Logger::info("Terminating GLFW library.");
    glfwTerminate();
    Logger::info("GLFW library terminated.");

    delete canvas;
    delete sceneBuffer;
}

glm::vec2 OpenGLRender::getCursorDelta() const {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos) - defaultCursorPosition;
}

bool OpenGLRender::isClosed() {
    return glfwWindowShouldClose(window);
}

void OpenGLRender::close() {
    glfwSetWindowShouldClose(window, true);
}

void OpenGLRender::setKeyCallback(GLFWkeyfun callback) {
    Logger::info("Setup window key callback");
    glfwSetKeyCallback(window, callback);
}

glm::ivec2 OpenGLRender::getResolution() const {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { width, height };
}

void OpenGLRender::updateSdfScene(Scene* scene) {
    FigureDetails box;
    box.asBox.sizes = glm::vec3(1, 1, 2);

    FigureDetails torus;
    torus.asTorus.smallRadius = 1;
    torus.asTorus.largeRadius = 2;

    SceneNode::Plain plainData[] = {
        {1, 2, SUBSTRACTION, {0, 0, 0}},
        {-1, -1, LEAF, {0, 0, 0}, 0, Figure(BOX, box)},
        {-1, -1, LEAF, {0, 0, 0}, 1, Figure(TORUS, torus)}
    };

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->getId());
    sceneBuffer->writeData(sizeof(plainData), &plainData, GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void OpenGLRender::draw(const OpenGLRenderContext& renderContext) {
    glUseProgram(renderContext.getShaderProgram()->getId());
    glBindVertexArray(canvas->getVao());
    glDrawElements(GL_TRIANGLES, canvas->getIndexesCount(), GL_UNSIGNED_INT, 0);
}

void OpenGLRender::resetCursor() {
    glfwSetCursorPos(window, defaultCursorPosition.x, defaultCursorPosition.y);
}

void OpenGLRender::pullEvents() {
    glfwPollEvents();
}

void OpenGLRender::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRender::display() {
    glfwSwapBuffers(window);
}

int OpenGLRender::getKeyStatus(int key) {
    return glfwGetKey(window, key);
}

void OpenGLRender::updateDynamicUniforms(const OpenGLRenderContext& renderContext) {
    ShaderProgram* shaderProgram = renderContext.getShaderProgram();

    glUseProgram(shaderProgram->getId());
    Camera* camera = renderContext.getCamera();
    renderContext.getShaderProgram()->setUniform("u_view", camera->computeLookAtMatrix());
    renderContext.getShaderProgram()->setUniform("u_cameraPosition", camera->getPosition());
}

void OpenGLRender::updateStaticUniforms(const OpenGLRenderContext& renderContext) {
    ShaderProgram* shaderProgram = renderContext.getShaderProgram();

    glUseProgram(shaderProgram->getId());
    renderContext.getShaderProgram()->setUniform("u_resolution", renderContext.getResolution());
}
