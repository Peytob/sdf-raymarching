#ifndef SCENETREE_HPP
#define SCENETREE_HPP

#include <glm/vec3.hpp>
#include <sdfraymarching/scene/SceneNodeOperation.hpp>
#include <sdfraymarching/scene/Figure.hpp>

#pragma once

class SceneNode {
public:

    static SceneNode* substraction();
    static SceneNode* merge();
    static SceneNode* intersection();
    static SceneNode* figure(
        const Figure& figure,
        const glm::vec3& localPosition = glm::vec3(0.0));

    ~SceneNode();

    SceneNode* getLeftChild();
    SceneNode* getRightChild();

    void setLeftChild(SceneNode* node);
    void setRightChild(SceneNode* node);

    void setLocalPosition(const glm::vec3& position);
    const glm::vec3& getLocalPosition();

    const Figure& getFigure();
    const SceneNodeOperation& getOperation();

private:
    SceneNode(SceneNodeOperation operation, const glm::vec3 localPosition, const Figure& figure);
    SceneNode(const SceneNode& node) {}

    SceneNode* leftChild;
    SceneNode* rightChild;

    SceneNodeOperation operation;
    glm::vec3 localPosition;

    // Incompletable with figure static method
    Figure figureData;
};

#endif
