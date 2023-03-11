#include "SceneToPlainConverter.hpp"

namespace {
    const int NULL_NODE_INDEX = -1;
}

PlainScene SceneToPlainConverter::toPlainData(Scene* scene) {
    reset();
    processNode(scene->getRootNode());
    return plainScene;
}

void SceneToPlainConverter::reset() {
    this->plainScene = PlainScene();
}

int SceneToPlainConverter::processNode(SceneNode* node) {
    if (nullptr == node) {
        return NULL_NODE_INDEX;
    }

    plainScene.nodes.emplace_back();
    int index = plainScene.nodes.size() - 1;

    auto leftChildIndex = processNode(node->getLeftChild());
    plainScene.nodes[index].leftChild = leftChildIndex;
    auto rightChildIndex = processNode(node->getRightChild());
    plainScene.nodes[index].rightChild = rightChildIndex;
    plainScene.nodes[index].operation = node->getOperation();
    plainScene.nodes[index].localPosition = node->getLocalPosition();
    plainScene.nodes[index].materialId = 3; // TODO Mock
    plainScene.nodes[index].figureData = node->getFigure();

    return index;
}
