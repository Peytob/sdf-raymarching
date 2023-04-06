#include <sdfraymarching/scene/SceneConvertionException.hpp>

#include "SceneToPlainConverter.hpp"

namespace {
    const int NULL_NODE_INDEX = -1;
}

PlainScene SceneToPlainConverter::toPlainData(const Scene* scene) {
    reset();
    processMaterials(scene->getMaterials());
    processNode(scene->getRootNode(), glm::vec3(0.0f));
    return plainScene;
}

void SceneToPlainConverter::reset() {
    this->plainScene = PlainScene();
}

GLint SceneToPlainConverter::processNode(const SceneNode* node, const glm::vec3& localPosition) {
    if (nullptr == node) {
        return NULL_NODE_INDEX;
    }

    plainScene.nodes.emplace_back();
    int index = plainScene.nodes.size() - 1;

    glm::vec3 plainNodeLocalPosition = node->getLocalPosition() + localPosition;

    if (node->getOperation() == LEAF) {
        plainScene.nodes[index].localPosition = plainNodeLocalPosition;

        if (materialIdsIndexes.find(node->getMaterialId()) == materialIdsIndexes.end()) {
            throw SceneConvertionException("Material with id " + node->getMaterialId() + " not found!");
        } else {
            plainScene.nodes[index].materialId = materialIdsIndexes[node->getMaterialId()];
        }
    }

    plainScene.nodes[index].operation = node->getOperation();

    plainScene.nodes[index].figureData = node->getFigure();

    auto leftChildIndex = processNode(node->getLeftChild(), plainNodeLocalPosition);
    plainScene.nodes[index].leftChild = leftChildIndex;
    auto rightChildIndex = processNode(node->getRightChild(), plainNodeLocalPosition);
    plainScene.nodes[index].rightChild = rightChildIndex;

    return index;
}

void SceneToPlainConverter::processMaterials(const std::vector<Material>& materials) {
    for (size_t index = 0; index < materials.size(); index++) {
        const Material& material = materials[index];

        Material::Plain plainMaterial {
            glm::vec4(material.getColor(), 0.0),
            glm::vec4(material.getSpecColor(), 0.0),
            glm::vec4(material.getShininess())
        };

        materialIdsIndexes[material.getMateialId()] = index;
        plainScene.materials.emplace_back(plainMaterial);
    }
}

const std::map<std::string, size_t>& SceneToPlainConverter::getMaterialsIndexesMap() const {
    return this->materialIdsIndexes;
}
