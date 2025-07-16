#pragma once

#include <memory>
#include <unordered_map>
#include "SceneNode.h"
#include "EventHandler.h"
#include "Component.h"

#include "Material.h"


class SceneNode;



class SceneNodeBuilder {
private:
    std::unique_ptr<SceneNode> node;
    EventHandler* eventHandler;

public:
    SceneNodeBuilder(EventHandler* handler);

    template <typename T, typename... Args>
    SceneNodeBuilder& addComponent(Args&&... args) {
        auto componentPtr = std::make_shared<T>(std::forward<Args>(args)...);
        componentPtr->parent = node.get();
        node->addComponent(getNodeTypeBitmask<T>(), componentPtr);
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