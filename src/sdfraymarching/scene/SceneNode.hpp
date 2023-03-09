#ifndef SCENETREE_HPP
#define SCENETREE_HPP

#include <string>

#include <glm/vec3.hpp>

#include <sdfraymarching/scene/SceneNodeOperation.hpp>
#include <sdfraymarching/scene/Figure.hpp>

#pragma once

// Простейшая и наивнейшая реализация нод сцены с кучей проблем.

class SceneNode {
public:

    static SceneNode* substraction();
    static SceneNode* merge();
    static SceneNode* intersection();
    static SceneNode* figure(
        const Figure& figure,
        const glm::vec3& localPosition = glm::vec3(0.0));

    struct Plain {
        GLint leftChild;
        GLint rightChild;

        SceneNodeOperation operation;
        glm::vec3 localPosition;
        GLint materialId;

        // Incompletable with figure static method
        Figure figureData;
    };

    ~SceneNode();

    SceneNode* getLeftChild();
    SceneNode* getRightChild();

    void setLeftChild(SceneNode* node);
    void setRightChild(SceneNode* node);

    void setLocalPosition(const glm::vec3& position);
    const glm::vec3& getLocalPosition() const;

    const Figure& getFigure() const;
    const SceneNodeOperation& getOperation() const;

    const std::string& getMaterialId() const;
    void setMaterialId(const std::string& materialId);

private:
    SceneNode(SceneNodeOperation operation, const glm::vec3& localPosition, const Figure& figure);
    SceneNode(const SceneNode& node) {}

    SceneNode* leftChild;
    SceneNode* rightChild;

    SceneNodeOperation operation;
    glm::vec3 localPosition;

    std::string materialId;

    // Incompletable with figure static method
    Figure figureData;
};

#endif
