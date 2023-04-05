#include <stdexcept>
#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/OpenGLWrapper.hpp>
#include <sdfraymarching/render/ShaderStorageBuffer.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>
#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/PlainScene.hpp>
#include <sdfraymarching/scene/SceneToPlainConverter.hpp>

#include "SceneTreeDrawer.hpp"

namespace {

ShaderProgram* loadStaticSsboProgram() {
    ShaderProgram* shaderProgram = ShaderProgram::loadShaderProgram("./resources/vertex.glsl", "./resources/fragment_ssbo.glsl");
    glUseProgram(shaderProgram->getId());
    return shaderProgram;
}

};

SceneTreeDrawer::SceneTreeDrawer(OpenGLWrapper* openGLWrapper) :
    IDrawer(),
    openGLWrapper(openGLWrapper) {

    if (nullptr == openGLWrapper) {
        Logger::error("OpenGL wrapper should not be null!");
        throw std::invalid_argument("Wrong openglwrapper argument: nullptr");
    }

    this->worldShaderProgram = loadStaticSsboProgram();
    this->sceneBuffer = new ShaderStorageBuffer(1);
    this->materialBuffer = new ShaderStorageBuffer(2);
};

SceneTreeDrawer::~SceneTreeDrawer() {
    Logger::error("Destroying scene drawer resources");
    delete sceneBuffer;
    delete materialBuffer;
}

void SceneTreeDrawer::drawScene(const Scene* scene, const OpenGLRenderContext& renderContext) {
    openGLWrapper->updateDynamicUniforms(renderContext, worldShaderProgram);
    openGLWrapper->updateStaticUniforms(renderContext, worldShaderProgram);
    openGLWrapper->draw(renderContext);
}

void SceneTreeDrawer::onSceneLoaded(const Scene* scene) {
    onSceneUpdated(scene);
}

void SceneTreeDrawer::onSceneUpdated(const Scene* scene) {
    Logger::info("Updating SDF scene");

    SceneToPlainConverter converter;
    PlainScene plainScene = converter.toPlainData(scene);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneBuffer->getId());
    sceneBuffer->writeData(sizeof(SceneNode::Plain) * plainScene.nodes.size(), static_cast<void*>(plainScene.nodes.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer->getId());
    materialBuffer->writeData(sizeof(Material::Plain) * plainScene.materials.size(), static_cast<void*>(plainScene.materials.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
