#ifndef STATICDRAWER_HPP
#define STATICDRAWER_HPP

#pragma once

#include <sdfraymarching/render/OpenGLWrapper.hpp>
#include <sdfraymarching/render/drawer/IDrawer.hpp>

class ShaderProgram;

class StaticDrawer : public IDrawer {
public:
    explicit StaticDrawer(OpenGLWrapper* openGLWrapper);
    ~StaticDrawer() override;

    void drawScene(const Scene* scene, const OpenGLRenderContext& renderContext) override;

    void onSceneLoaded(const Scene* scene) override;
    void onSceneUpdated(const Scene* scene) override;

private:
    OpenGLWrapper* openGLWrapper;
    ShaderProgram* worldShaderProgram;
    ShaderStorageBuffer* materialBuffer;

    void reloadShaderProgram(const Scene* scene, const std::map<std::string, size_t>& materialIndexes);
};

#endif