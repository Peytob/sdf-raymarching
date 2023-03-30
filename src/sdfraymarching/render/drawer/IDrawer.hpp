#ifndef IDRAWER_HPP
#define IDRAWER_HPP

#pragma once

class Scene;
class Camera;

class IDrawer {
public:
    ~IDrawer() = default;

    virtual void drawScene(const Scene* scene, const OpenGLRenderContext& renderContext) = 0;

    virtual void onSceneLoaded(const Scene* scene) = 0;
    virtual void onSceneUpdated(const Scene* scene) = 0;

protected:
    IDrawer() = default;
};

#endif
