#ifndef JSONSCENELOADER_HPP
#define JSONSCENELOADER_HPP

#include <string>

#pragma once

class Scene;

class JsonSceneLoader {
public:
    JsonSceneLoader() = default;
    ~JsonSceneLoader() = default;

    Scene* load(const std::string& jsonString);
};

#endif