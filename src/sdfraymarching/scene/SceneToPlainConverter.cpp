#include "SceneToPlainConverter.hpp"

namespace {
    const int NULL_NODE_INDEX = -1;
}

PlainScene SceneToPlainConverter::toPlainData(Scene* scene) {
    reset();
    processNode(scene->getRootNode(), glm::vec3(0.0f));
    return plainScene;
}

void SceneToPlainConverter::reset() {
    this->plainScene = PlainScene();
}

GLint SceneToPlainConverter::processNode(SceneNode* node, glm::vec3 localPosition) {
    if (nullptr == node) {
        return NULL_NODE_INDEX;
    }

    plainScene.nodes.emplace_back();
    int index = plainScene.nodes.size() - 1;

    glm::vec3 plainNodeLocalPosition = node->getLocalPosition() + localPosition;

    if (node->getOperation() == LEAF) {
        plainScene.nodes[index].localPosition = plainNodeLocalPosition;
    }

    plainScene.nodes[index].operation = node->getOperation();
    plainScene.nodes[index].materialId = 3; // TODO Mock
    plainScene.nodes[index].figureData = node->getFigure();

    auto leftChildIndex = processNode(node->getLeftChild(), plainNodeLocalPosition);
    plainScene.nodes[index].leftChild = leftChildIndex;
    auto rightChildIndex = processNode(node->getRightChild(), plainNodeLocalPosition);
    plainScene.nodes[index].rightChild = rightChildIndex;

    return index;
}
