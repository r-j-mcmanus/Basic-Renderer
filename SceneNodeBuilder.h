#pragma once

#include <memory>
#include <unordered_map>
#include "SceneNode.h"
#include "Component.h"

#include "Material.h"


class SceneNode;



class SceneNodeBuilder {
private:
    std::unique_ptr<SceneNode> node;

public:
    SceneNodeBuilder();

    template <typename T, typename... Args>
    SceneNodeBuilder& addComponent(Args&&... args) {
        node->addComponent(getNodeTypeBitmask<T>(), std::make_shared<T>(std::forward<Args>(args)...));
        return *this;
    }    
    
    SceneNodeBuilder& setTransform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl) {
        node->setTransform(pos, rot, scl);
        return *this;
    }

    std::unique_ptr<SceneNode> build();

private:
    template <typename T>
    uint64_t getNodeTypeBitmask() {
        return 1;
    }
};