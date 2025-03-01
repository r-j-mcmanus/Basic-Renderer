#pragma once

#include <unordered_map>
#include "SceneNode.h"
#include "Component.h"

#include "CameraComponent.h"
#include "LightComponent.h"


class SceneNode;

class NodeBuilder {
private:
    int bitmask;
    std::unordered_map<int, std::shared_ptr<Component>> components;

public:
    // Constructor with node name
    NodeBuilder() : bitmask(NodeType::NONE) {
    }

    NodeBuilder& addRenderableType() {
        if (bitmask & NodeType::__TRANSFORMER)
        {
            bitmask |= NodeType::__TRANSFORMER;

        }

        bitmask |= NodeType::RENDERABLE;
        return *this;
    }

    NodeBuilder& addLightType() {
        bitmask |= NodeType::LIGHT;
        LightComponent component;
        components[NodeType::CAMERA] = std::make_shared<Component>(component);
        return *this;
    }

    NodeBuilder& addCameraType() {
        bitmask |= NodeType::CAMERA;
        CameraComponent component;
        components[NodeType::CAMERA] = std::make_shared<Component>(component);

        return *this;
    }

    std::shared_ptr<SceneNode> build() {
        auto node = std::make_shared<SceneNode>(bitmask, components);
        return node;
    }
};