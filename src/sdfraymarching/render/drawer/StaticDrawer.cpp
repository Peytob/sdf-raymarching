#include <stdexcept>
#include <map>
#include <GL/glew.h>

#include <sdfraymarching/utils/Logger.hpp>
#include <sdfraymarching/render/OpenGLWrapper.hpp>
#include <sdfraymarching/render/ShaderStorageBuffer.hpp>
#include <sdfraymarching/render/OpenGLRenderContext.hpp>
#include <sdfraymarching/render/ShaderProgram.hpp>
#include <sdfraymarching/scene/Scene.hpp>
#include <sdfraymarching/scene/PlainScene.hpp>
#include <sdfraymarching/scene/SceneToPlainConverter.hpp>
#include <sdfraymarching/scene/SceneSdfMethodGenerator.hpp>

#include "StaticDrawer.hpp"
#include <iostream>
namespace {

ShaderProgram* generateStaticProgram(const Scene* scene) {
    std::map<std::string, std::string> generatedIncludes;
    generatedIncludes["GENERATED_SCENE_DATA"] = SceneSdfMethodGenerator::generateSceneSdfMethod(scene);
    std::cout << generatedIncludes["GENERATED_SCENE_DATA"] << std::endl;
    ShaderProgram* shaderProgram = ShaderProgram::loadShaderProgram(
            "./resources/vertex.glsl",
            "./resources/fragment_static.glsl",
            generatedIncludes);

    glUseProgram(shaderProgram->getId());
    return shaderProgram;
}

};

StaticDrawer::StaticDrawer(OpenGLWrapper* openGLWrapper) :
    IDrawer(),
    openGLWrapper(openGLWrapper) {

    if (nullptr == openGLWrapper) {
        Logger::error("OpenGL wrapper should not be null!");
        throw std::invalid_argument("Wrong openglwrapper argument: nullptr");
    }

    this->materialBuffer = new ShaderStorageBuffer(2);
    this->worldShaderProgram = nullptr;
};

StaticDrawer::~StaticDrawer() {
    Logger::error("Destroying scene drawer resources");
    delete materialBuffer;
}

void StaticDrawer::drawScene(const Scene* scene, const OpenGLRenderContext& renderContext) {
    openGLWrapper->updateDynamicUniforms(renderContext, worldShaderProgram);
    openGLWrapper->updateStaticUniforms(renderContext, worldShaderProgram);
    openGLWrapper->draw(renderContext);
}

void StaticDrawer::onSceneLoaded(const Scene* scene) {
    onSceneUpdated(scene);
}

void StaticDrawer::onSceneUpdated(const Scene* scene) {
    Logger::info("Updating SDF scene");

    SceneToPlainConverter converter;
    PlainScene plainScene = converter.toPlainData(scene);

    reloadShaderProgram(scene);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, materialBuffer->getId());
    materialBuffer->writeData(sizeof(Material::Plain) * plainScene.materials.size(), static_cast<void*>(plainScene.materials.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void StaticDrawer::reloadShaderProgram(const Scene* scene) {
    delete worldShaderProgram;
    worldShaderProgram = generateStaticProgram(scene);
}

