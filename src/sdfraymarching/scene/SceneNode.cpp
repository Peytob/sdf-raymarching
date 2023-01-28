#include "SceneNode.hpp"

SceneNode* SceneNode::substraction() {
    return new SceneNode(SceneNodeOperation::SUBSTRACTION, glm::vec3(), Figure());
}

SceneNode* SceneNode::merge() {
    return new SceneNode(SceneNodeOperation::MERGE, glm::vec3(), Figure());
}

SceneNode* SceneNode::intersection() {
    return new SceneNode(SceneNodeOperation::INTERSECTION, glm::vec3(), Figure());
}

SceneNode* SceneNode::figure(const Figure& figure, const glm::vec3& localPosition) {
    return new SceneNode(SceneNodeOperation::LEAF, glm::vec3(), Figure());
}

SceneNode::SceneNode(SceneNodeOperation operation, const glm::vec3 localPosition, const Figure& figure) :
    leftChild(nullptr),
    rightChild(nullptr),
    operation(operation),
    localPosition(localPosition),
    figureData(figure) { }

SceneNode::~SceneNode() {
    if (nullptr != leftChild) {
        delete leftChild;
    }

    if (nullptr != rightChild) {
        delete rightChild;
    }
}

SceneNode* SceneNode::getLeftChild() {
    return leftChild;
}

SceneNode* SceneNode::getRightChild() {
    return rightChild;
}

void SceneNode::setLeftChild(SceneNode* node) {
    this->leftChild = node;
}

void SceneNode::setRightChild(SceneNode* node) {
    this->rightChild = node;
}

void SceneNode::setLocalPosition(const glm::vec3& position) {
    this->localPosition = position;
}

const glm::vec3& SceneNode::getLocalPosition() {
    return localPosition;
}

const Figure& SceneNode::getFigure() {
    return figureData;
}

const SceneNodeOperation& SceneNode::getOperation() {
    return operation;
}
