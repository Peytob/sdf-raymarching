#ifndef SCENETREEDRAWER_HPP
#define SCENETREEDRAWER_HPP

#pragma once

#include <sdfraymarching/render/drawer/IDrawer.hpp>

class ShaderProgram;

class SceneTreeDrawer : public IDrawer {
public:
    SceneTreeDrawer(OpenGLWrapper* openGLWrapper);
    ~SceneTreeDrawer();

    void drawScene(const Scene* scene, const OpenGLRenderContext& renderContext) override;

    void onSceneLoaded(const Scene* scene) override;
    void onSceneUpdated(const Scene* scene) override;

private:
    OpenGLWrapper* openGLWrapper;
    ShaderProgram* worldShaderProgram;
    ShaderStorageBuffer* sceneBuffer;
    ShaderStorageBuffer* materialBuffer;
};

#endif