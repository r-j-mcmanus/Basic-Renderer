#pragma once

#include <memory>
#include <unordered_map>
#include "SceneNode.h"
#include "Component.h"
#include "RenderingControler.h"

#include "CameraComponent.h"
#include "LightComponent.h"
#include "RenderingComponent.h"

#include "Material.h"


class SceneNode;

class NodeBuilder {
private:
    int bitmask;
    std::unordered_map<int, std::shared_ptr<Component>> components;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    // Constructor with node name
    NodeBuilder() : bitmask(NodeType::NONE) {
    }

    NodeBuilder& addRenderableType(unsigned int modelId, Material material, std::unique_ptr<RenderingController> renderingControler) {
        RenderingComponent component;
        components[NodeType::RENDERABLE] = std::make_shared<Component>(component);
        bitmask |= NodeType::RENDERABLE;
        return *this;
    }

    NodeBuilder& setTranformationData(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        return *this;
    }

    NodeBuilder& addLightType(Light light) {
        bitmask |= NodeType::LIGHT;
        LightComponent component(light);
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
        bitmask = 0;
        components = std::unordered_map<int, std::shared_ptr<Component>>();
        position = { 0,0,0 };
        rotation = { 0,0,0 };
        scale = { 0,0,0 };
        return node;
    }
};